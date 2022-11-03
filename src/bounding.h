#ifndef __BOUNDING_H__
#define __BOUNDING_H__

#include <limits>
#include <glm/glm.hpp>

namespace cpuRE {
  class BoundingBox;

  class BoundingSphere {
    friend BoundingBox;
  private:
    glm::vec3 center_;
    float radius_;

  public:
    BoundingSphere();
    BoundingSphere(const glm::vec3 &center, float radius);
    BoundingSphere(const BoundingSphere &bs);
    BoundingSphere(const BoundingBox &bb);

    void reset();

    bool valid() const { return radius_ >= 0.0; }

    bool operator == (const BoundingSphere &rhs) const {
      return center_ == rhs.center_ && radius_ == rhs.radius_;
    }

    bool operator != (const BoundingSphere &rhs) const {
      return center_ != rhs.center_ || radius_ == rhs.radius_;
    }

    void set(const glm::vec3 &center, float radius) {
      center_ = center;
      radius_ = radius;
    }

    const glm::vec3 &center() const { return center_; }
    glm::vec3 &center() { return center_; }

    const float radius() const { return radius_; }
    const float radius2() const { return radius_ * radius_; }

    void expandBy(const glm::vec3 &v);
    void expandBy(const BoundingSphere &sh);
    void expandBy(const BoundingBox &bb);

    template<typename VA>
    void expandBy(const VA &va) {
      for (auto &v : va) {
        expandBy(v);
      }
    }

    void expandRadiusBy(const glm::vec3 &v);
    void expandRadiusBy(const BoundingSphere &sh);
    void expandRadiusBy(const BoundingBox &bb);

    bool contains(const glm::vec3 &v) const;
    bool intersects(const BoundingSphere &bs) const;
  };

  class BoundingBox {
  private:
    glm::vec3 min_;
    glm::vec3 max_;

  public:
    BoundingBox() { reset(); }
    BoundingBox(const BoundingBox& bb) : min_(bb.min_), max_(bb.max_) {}
    BoundingBox(const glm::vec3& min, const glm::vec3& max) : min_(min), max_(max) {}

    void reset() {
      constexpr float float_min = -std::numeric_limits<float>::max();
      constexpr float float_max =  std::numeric_limits<float>::max();
      min_ = { float_max, float_max, float_max };
      max_ = { float_min, float_min, float_min };
    }

    BoundingBox& operator = (BoundingBox&& rhs) noexcept {
      if (this != &rhs) {
        min_ = std::move(rhs.min_);
        max_ = std::move(rhs.max_);
      }
      return *this;
    }

    BoundingBox& operator = (const BoundingBox& rhs) {
      if (this != &rhs) {
        min_ = rhs.min_;
        max_ = rhs.max_;
      }
      return *this;
    }

    BoundingBox(BoundingBox&& rhs) noexcept {
      if (this != &rhs) {
        min_ = std::move(rhs.min_);
        max_ = std::move(rhs.max_);
      }
    }

    bool operator == (const BoundingBox& rhs) const {
      return min_ == rhs.min_ && max_ == rhs.max_;
    }

    bool operator != (const BoundingBox& rhs) const {
      return min_ != rhs.min_ || max_ != rhs.max_;
    }

    bool valid() const {
      if (max_.x < min_.x) return false;
      if (max_.y < min_.y) return false;
      if (max_.z < min_.z) return false;
      return true;
    }

    void set(const glm::vec3& min, const glm::vec3& max) {
      min_ = min;
      max_ = max;
    }

    const glm::vec3& min() const { return min_; }
    const glm::vec3& max() const { return max_; }

    const float min(unsigned int index) const { return min_[index]; }
    const float max(unsigned int index) const { return max_[index]; }
    float range(unsigned int index) const { return max_[index] - min_[index]; }

    const glm::vec3 center() const { return (min_ + max_) * 0.5f; }

    const glm::vec3 corner(unsigned int pos) const {
      glm::vec3 v;
      for (auto i = 0; i < glm::vec3::length(); ++i, pos = pos >> 1) {
        v[i] = (pos & 1) ? max_[i] : min_[i];
      }
      return v;
    }

    float radius2() const;
    float radius() const { return std::sqrt(radius2()); }

    void expandBy(const glm::vec3& v);
    void expandBy(const BoundingBox& bb);
    void expandBy(const BoundingSphere& sh);

    template<typename VA>
    void expandBy(const VA& va) {
      for (auto& v : va) {
        expandBy(v);
      }
    }

    BoundingBox intersect(const BoundingBox& bb) const;
    bool intersects(const BoundingBox& bb) const;

    bool contains(const glm::vec3& v) const;
    bool contains(const glm::vec3& v, float epsilon) const;
  };


}
#endif