#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include <limits>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
#include "bounding.h"

namespace cpuRE {
  BoundingSphere::BoundingSphere() :
    center_(0.f, 0.f, 0.f), radius_(-1.0) {}

  BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) :
    center_(center), radius_(radius) {}

  BoundingSphere::BoundingSphere(const BoundingSphere &bs) :
    center_(bs.center_), radius_(bs.radius_) {}

  BoundingSphere::BoundingSphere(const BoundingBox &bb) :
    center_(0.0, 0.0, 0.0), radius_(-1.0) { expandBy(bb); }

  void BoundingSphere::reset() {
    center_.x = 0.f;
    center_.y = 0.f;
    center_.z = 0.f;
    radius_ = -1.0;
  }

  void BoundingSphere::expandBy(const glm::vec3 &v) {
    if (valid()) {
      glm::vec3 dv = v - center_;
      float r = 0.0;
      for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
        r += dv[i] * dv[i];
      }
      r = std::sqrt(r);
      if (r > radius_) {
        float dr = (r - radius_) * 0.5;
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
    float d = 0.0;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      d += d_vec[i] * d_vec[i];
    }
    d = sqrtf(d);

    if (d + sh.radius() <= radius_) {
      return;
    }

    if (d + radius_ <= sh.radius()) {
      center_ = sh.center_;
      radius_ = sh.radius_;
      return;
    }

    double new_radius = (radius_ + d + sh.radius()) * 0.5;
    double ratio = (new_radius - radius_) / d;

    center_ = (sh.center() - center_) * glm::vec3(ratio, ratio, ratio);
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
      glm::vec3 d = (v - center_);
      float r = sqrtf(d.x * d.x + d.y + d.y + d.z * d.z);
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
        glm::vec3 d_vec = sh.center_ - center_;
        float d = sqrtf(d_vec.x * d_vec.x + d_vec.y * d_vec.y);
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
        for (unsigned int c = 0; c < 8; ++c) {
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
    float length2 = glm::distance2(center_, bs.center_);
    return valid() && bs.valid() &&
           (length2 <= (radius_ + bs.radius_) * (radius_ + bs.radius_));
  }

  float BoundingBox::radius2() const {
    glm::vec3 e;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      e[i] = (max_[i] - min_[i]) / 2.0;
    }

    float length =  0.0 ;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      length += e[i] * e[i];
    }
    return length;
  }

  void BoundingBox::expandBy(const glm::vec3& v) {
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      if (v[i] < min_[i]) min_[i] = v[i];
      if (v[i] > max_[i]) max_[i] = v[i];
    }
  }

  void BoundingBox::expandBy(const BoundingBox& bb) {
    if (!bb.valid()) return;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      if (bb.min_[i] < min_[i]) min_[i] = bb.min_[i];
      if (bb.max_[i] > max_[i]) max_[i] = bb.max_[i];
    }
  }

  void BoundingBox::expandBy(const BoundingSphere& sh) {
    if (!sh.valid()) return;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      if (sh.center_[i] - sh.radius_ < min_[i]) min_[i] = sh.center_[i] - sh.radius_;
      if (sh.center_[i] + sh.radius_ > max_[i]) max_[i] = sh.center_[i] + sh.radius_;
    }
  }

  BoundingBox BoundingBox::intersect(const BoundingBox& bb) const {
    glm::vec3 min, max;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      min[i] = std::max(min_[i], bb.min_[i]);
      max[i] = std::min(max_[i], bb.max_[i]);
    }
    return BoundingBox(min, max);
  }

  bool BoundingBox::intersects(const BoundingBox& bb) const {
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      if (glm::max(min_[i], bb.min_[i]) > glm::min(max_[i], bb.max_[i]))
        return false;
    }
    return true;
  }

  bool BoundingBox::contains(const glm::vec3& v) const {
    if (!valid()) return false;
    for (glm::vec3::length_type i = 0; i < glm::vec3::length(); ++i) {
      if (v[i] < min_[i] || v[i] > max_[i]) return false;
    }
    return true;
  }

  bool BoundingBox::contains(const glm::vec3& v, float epsilon) const {
    if (!valid()) return false;
    if ((v.x + epsilon) < min_.x || (v.x - epsilon) > max_.x) return false;
    if ((v.y + epsilon) < min_.y || (v.x - epsilon) > max_.y) return false;
    if ((v.z + epsilon) < min_.z || (v.x - epsilon) > max_.z) return false;
    return true;
  }
}

#endif