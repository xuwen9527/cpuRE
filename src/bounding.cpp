#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "bounding.h"

namespace cpuRE {
  BoundingSphere::BoundingSphere() :
    center_(0.f, 0.f, 0.f), radius_(-1.f) {}

  BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) :
    center_(center), radius_(radius) {}

  BoundingSphere::BoundingSphere(const BoundingSphere &bs) :
    center_(bs.center_), radius_(bs.radius_) {}

  BoundingSphere::BoundingSphere(const BoundingBox &bb) :
    center_(0.f, 0.f, 0.f), radius_(-1.f) { expandBy(bb); }

  void BoundingSphere::reset() {
    center_ = { 0.f, 0.f, 0.f };
    radius_ = -1.0;
  }

  void BoundingSphere::expandBy(const glm::vec3 &v) {
    if (valid()) {
      glm::vec3 dv = v - center_;
      float r = glm::length(dv);
      if (r > radius_) {
        float dr = (r - radius_) * 0.5f;
        center_ += dv / r * dr;
        radius_ += dr;
      }
    } else {
      center_ = v;
      radius_ = 0.000001f;
    }
  }

  void BoundingSphere::expandBy(const BoundingSphere &sh) {
    if (!sh.valid()) {
      return;
    }
    if (!valid()) {
      center_ = sh.center_;
      radius_ = sh.radius_;
      return;
    }

    glm::vec3 d_vec = center_ - sh.center();
    float d = glm::length(d_vec);
    if (d + sh.radius() <= radius_) {
      return;
    }

    if (d + radius_ <= sh.radius()) {
      center_ = sh.center_;
      radius_ = sh.radius_;
      return;
    }

    float new_radius = (radius_ + d + sh.radius()) * 0.5f;
    float ratio = (new_radius - radius_) / d;

    center_ = sh.center() - center_; 
    center_ *= ratio;
    radius_ = new_radius;
  }

  void BoundingSphere::expandBy(const BoundingBox &bb) {
    if (bb.valid()) {
      if (valid()) {
        BoundingBox newbb(bb);
        for (unsigned int i = 0; i < 8; ++i) {
          glm::vec3 v = bb.corner(i) - center_;
          v = glm::normalize(v);
          v *= -radius_;
          v += center_;
          newbb.expandBy(v);
        }
        center_ = newbb.center();
        radius_ = newbb.radius();
      } else {
        center_ = bb.center();
        radius_ = bb.radius();
      }
    }
  }

  void BoundingSphere::expandRadiusBy(const glm::vec3 &v) {
    if (valid()) {
      float r = glm::distance(v, center_);
      if (r > radius_){
        radius_ = r;
      }
    } else {
      center_ = v;
      radius_ = 0.0;
    }
  }

  void BoundingSphere::expandRadiusBy(const BoundingSphere &sh) {
    if (sh.valid()) {
      if (valid()) {
        float d = glm::distance(sh.center_, center_);
        float r = d + sh.radius_;
        if (r > radius_) {
          radius_ = r;
        }
      } else {
        center_ = sh.center_;
        radius_ = sh.radius_;
      }
    }
  }

  void BoundingSphere::expandRadiusBy(const BoundingBox &bb) {
    if (bb.valid()) {
      if (valid()) {
        for (auto c = 0; c < 8; ++c) {
          expandRadiusBy(bb.corner(c));
        }
      } else {
        center_ = bb.center();
        radius_ = bb.radius();
      }
    }
  }

  bool BoundingSphere::contains(const glm::vec3 &v) const {
    return valid() && (glm::distance2(v, center_) <= radius2());
  }

  bool BoundingSphere::intersects(const BoundingSphere &bs) const {
    if (!valid() || !bs.valid()) return false;
    float length2 = glm::distance2(center_, bs.center_);
    return length2 <= (radius_ + bs.radius_) * (radius_ + bs.radius_);
  }

  float BoundingBox::radius2() const {
    return glm::length2((max_ - min_) / 2.f);
  }

  void BoundingBox::expandBy(const glm::vec3& v) {
    if (v.x < min_.x) min_.x = v.x;
    if (v.y < min_.y) min_.y = v.y;
    if (v.z < min_.z) min_.z = v.z;
    if (v.x > max_.x) max_.x = v.x;
    if (v.y > max_.y) max_.y = v.y;
    if (v.z > max_.z) max_.z = v.z;
  }

  void BoundingBox::expandBy(const BoundingBox& bb) {
    if (!bb.valid()) return;
    if (bb.min_.x < min_.x) min_.x = bb.min_.x;
    if (bb.min_.y < min_.y) min_.y = bb.min_.y;
    if (bb.min_.z < min_.z) min_.z = bb.min_.z;
    if (bb.max_.x > max_.x) max_.x = bb.max_.x;
    if (bb.max_.y > max_.y) max_.y = bb.max_.y;
    if (bb.max_.z > max_.z) max_.z = bb.max_.z;
  }

  void BoundingBox::expandBy(const BoundingSphere& sh) {
    if (!sh.valid()) return;
    auto min = sh.center_ - sh.radius_;
    auto max = sh.center_ + sh.radius_;
    if (min_.x < min_.x) min_.x = min_.x;
    if (min_.y < min_.y) min_.y = min_.y;
    if (min_.z < min_.z) min_.z = min_.z;
    if (max_.x > max_.x) max_.x = max_.x;
    if (max_.y > max_.y) max_.y = max_.y;
    if (max_.z > max_.z) max_.z = max_.z;
  }

  BoundingBox BoundingBox::intersect(const BoundingBox& bb) const {
    glm::vec3 min, max;
    min.x = std::max(min_.x, bb.min_.x);
    min.y = std::max(min_.y, bb.min_.y);
    min.z = std::max(min_.z, bb.min_.z);
    max.x = std::min(max_.x, bb.max_.x);
    max.y = std::min(max_.y, bb.max_.y);
    max.z = std::min(max_.z, bb.max_.z);
    return BoundingBox(min, max);
  }

  bool BoundingBox::intersects(const BoundingBox& bb) const {
    if (std::max(min_.x, bb.min_.x) > std::min(max_.x, bb.max_.x)) return false;
    if (std::max(min_.y, bb.min_.y) > std::min(max_.y, bb.max_.y)) return false;
    if (std::max(min_.z, bb.min_.z) > std::min(max_.z, bb.max_.z)) return false;
    return true;
  }

  bool BoundingBox::contains(const glm::vec3& v) const {
    if (!valid()) return false;
    if (v.x < min_.x || v.x > max_.x) return false;
    if (v.y < min_.y || v.y > max_.y) return false;
    if (v.z < min_.z || v.z > max_.z) return false;
    return true;
  }

  bool BoundingBox::contains(const glm::vec3& v, float epsilon) const {
    if (!valid()) return false;
    if ((v.x + epsilon) < min_.x || (v.x - epsilon) > max_.x) return false;
    if ((v.y + epsilon) < min_.y || (v.y - epsilon) > max_.y) return false;
    if ((v.z + epsilon) < min_.z || (v.z - epsilon) > max_.z) return false;
    return true;
  }
}