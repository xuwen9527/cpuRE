#include <glm/gtx/transform.hpp>
#include "manipulator.h"

namespace {
  /**
   * Helper trackball method that projects an x,y pair onto a sphere of radius r OR
   * a hyperbolic sheet if we are away from the center of the sphere.
   */
  static double tb_project_to_sphere(double r, double x, double y) {
    double d, t, z;

    d = sqrt(x * x + y * y);
    /* Inside sphere */
    if (d < r * 0.70710678118654752440) {
        z = sqrt(r * r - d * d);
    }                            /* On hyperbola */
    else {
        t = r / 1.41421356237309504880;
        z = t * t / d;
    }
    return z;
  }

  static void trackball(glm::quat &rotate, glm::vec3 &axis, float &angle, float trackballSize, float p1x,
                        float p1y, float p2x, float p2y) {
     /*
      * First, figure out z-coordinates for projection of P1 and P2 to
      * deformed sphere
      */
      // glm::vec3 uv(glm::vec3(0.0f, 1.0f, 0.0f) * rotate);
      // glm::vec3 sv(glm::vec3(1.0f, 0.0f, 0.0f) * rotate);
      // glm::vec3 lv(glm::vec3(0.0f, 0.0f, -1.0f) * rotate);

      // glm::vec3 p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(trackballSize, p1x, p1y);
      // glm::vec3 p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(trackballSize, p2x, p2y);

      glm::dvec3 p1(p1x, p1y, tb_project_to_sphere(trackballSize, p1x, p1y));
      glm::dvec3 p2(p2x, p2y, tb_project_to_sphere(trackballSize, p2x, p2y));

      /*
       * Now, we want the cross product of P1 and P2
       */
      axis = glm::cross(p2, p1);
      axis = glm::normalize(axis);
      /*
       * Figure out how much to rotate around that axis.
       */
      glm::dvec3 e = p2 - p1;
      double d = sqrt(e.x * e.x + e.y * e.y + e.z * e.z);
      double t = d / (2.0f * trackballSize);

      /*
       * Avoid problems with out-of-control values...
       */
      if (t > 1.0) t = 1.0;
      if (t < -1.0) t = -1.0;
      angle = asin(t);
  }
}

namespace cpuRE {
  Manipulator::Manipulator() :
    distance_(1.0f),
    center_(0.f, 0.f, 0.f),
    offset_(0.f, 0.f, 0.f),
    rotation_(glm::identity<glm::quat>()),
    trackballSize_(6.0f),
    rotate_speed_(15.f),
    rotate_center_(true),
    wheelZoomFactor_(0.2f),
    minimumDistance_(0.01f),
    maximumDistance_(300.f),
    pressed_(false) {
  }

  Manipulator::~Manipulator() {

  }

  void Manipulator::camera(const std::shared_ptr<Camera> &camera) {
    camera_ = camera;
  }

  void Manipulator::center(const glm::vec3 &center) {
    center_ = center;
  }

  const glm::vec3 &Manipulator::center() const {
    return center_;
  }

  void Manipulator::distance(float distance) {
    distance_ = distance;

    wheelZoomFactor_ = distance_ * 0.0001f;
    minimumDistance_ = distance_ * 0.01f;
    maximumDistance_ = distance_ * 3.f;
  }

  float Manipulator::distance() {
    return distance_;
  }

  void Manipulator::rotation(const glm::quat &rotation) {
    rotation_ = rotation;
  }

  glm::quat &Manipulator::rotation() {
    return rotation_;
  }

  void Manipulator::rotateSpeed(float speed) {
    if (speed > 0.01f) {
      rotate_speed_ = speed;
    }
  }

  float Manipulator::rotateSpeed() {
    return rotate_speed_;
  }

  bool Manipulator::rotateTrackball(const glm::vec2 &p0, const glm::vec2 &p1) {
    if (fabs(p0.x - p1.x) < 0.0000001f && fabs(p0.y - p1.y) < 0.0000001f) {
      return false;
    }

    glm::vec3 axis;
    float angle;
    trackball(rotation_, axis, angle, trackballSize_, p1.x, p1.y, p0.x, p0.y);
    rotation_ = glm::rotate(rotation_, angle * rotate_speed_, axis);

    return true;
  }

  glm::mat4 Manipulator::matrix() {
    return glm::translate(glm::vec3(0.0, 0.0, -distance_)) *
           glm::translate(-offset_) *
           glm::mat4_cast(glm::inverse(rotation_)) *
           glm::translate(-center_);
  }

  glm::mat4 Manipulator::inverseMatrix() {
    return glm::translate(center_) *
           glm::mat4_cast(rotation_) *
           glm::translate(offset_) *
           glm::translate(glm::vec3(0., 0., distance_));
  }

  void Manipulator::rotate(glm::vec3 &axis, float angle) {
    rotation_ = glm::rotate(rotation_, angle, axis);
  }

  void Manipulator::rotate(glm::quat &quat) {
    rotation_ *= quat;
  }

  void Manipulator::pan(float dx, float dy, float dz) {
    if (rotate_center_) {
      center_.x -= dx;
      center_.y -= dy;
      center_.z -= dz;
    } else {
      offset_.x -= dx;
      offset_.y -= dy;
      offset_.z -= dz;
    }
  }

  void Manipulator::zoom(float dt) {
    float scale = 1.f + dt;

    float newDistance = distance_ * scale;
    if (newDistance > minimumDistance_) {
      if (newDistance < maximumDistance_) {
        distance_ = newDistance;
      } else {
        distance_ = maximumDistance_;
      }
    } else {
      distance_ = minimumDistance_;
    }
  }

  void Manipulator::home() {
    center_ = { 0.f, 0.f, 0.f };
    offset_ = { 0.f, 0.f, 0.f };
    rotation_ = glm::identity<glm::quat>();
    distance_ = 1.0f;
  }

  void Manipulator::apply() {
    if (valid()) {
      camera_->mv() = matrix();
    }
  }

  void Manipulator::mousePress(int x, int y) {
    last_point_ = camera_->windowToProject({ x, y });
    pressed_ = true;
  }

  void Manipulator::mouseRelease(int x, int y) {
    last_point_ = camera_->windowToProject({ x, y });
    pressed_ = false;
  }

  void Manipulator::mouseMove(int x, int y, bool rotate) {
    if (!camera_ || !pressed_) {
      return;
    }

    glm::vec2 curr_point = camera_->windowToProject({ x, y });

    if (rotate) {
      rotateTrackball(curr_point, last_point_);
    } else {
      glm::vec2 e = curr_point - last_point_;
      e *= distance_ * tanf(glm::radians(camera_->fov()) / 2.f);
      pan(e.x, e.y, 0.f);
    }
    last_point_ = curr_point;
  }

  void Manipulator::mouseScroll(bool zoomin) {
    zoom(zoomin ? wheelZoomFactor_ : -wheelZoomFactor_);
  }
}