#include <glm/gtx/transform.hpp>
#include "manipulator.h"
#include "timer.h"

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
    } else {/* On hyperbola */
      t = r / 1.41421356237309504880;
      z = t * t / d;
    }
    return z;
  }

  static void trackball(glm::quat &rotate, glm::vec3 &axis, float &angle,
    float trackballSize, float p1x, float p1y, float p2x, float p2y) {
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
    t = glm::clamp(t, -1.0, 1.0);
    angle = asin(t);
  }

  // a reasonable approximation of cosine interpolation
  double smoothStepInterp(double t) {
    return (t * t) * (3.0 - 2.0 * t);
  }

  // rough approximation of pow(x,y)
  double powFast(double x, double y) {
    return x / (x + y - y * x);
  }

  // accel/decel curve (a < 0 => decel)
  double accelerationInterp(double t, double a) {
    return a == 0.0 ? t : a > 0.0 ? powFast(t, a) : 1.0 - powFast(1.0 - t, -a);
  }
}

namespace cpuRE {
  Manipulator::Viewpoint::Viewpoint() : range_(-1.0f),
    focalPoint_(0.f, 0.f, 0.f),
    quat_(glm::identity<glm::quat>()) {
  }

  Manipulator::Viewpoint::Viewpoint(const Manipulator::Viewpoint &viewpoint) {
    name_ = viewpoint.name_;
    quat_ = viewpoint.quat_;
    range_ = viewpoint.range_;
    focalPoint_ = viewpoint.focalPoint_;
  }

  Manipulator::Viewpoint::Viewpoint(glm::vec3 &focal, float range, glm::vec3 &angle)
    : focalPoint_(focal), range_(range) {
    eulerAngle(angle);
  }

  Manipulator::Viewpoint::Viewpoint(glm::vec3 &focal, float range, glm::quat &quat)
    : focalPoint_(focal), quat_(quat), range_(range) {
  }

  bool Manipulator::Viewpoint::valid() {
    return range_ > 0.f;
  }

  Manipulator::Viewpoint Manipulator::Viewpoint::operator - (Manipulator::Viewpoint &other) {
    glm::vec3 focalPoint = focalPoint_ - other.focalPoint_;
    float range = range_ - other.range_;
    glm::quat quat = quat_ * glm::inverse(other.quat_);

    return Viewpoint(focalPoint, range, quat);
  }

  Manipulator::Viewpoint Manipulator::Viewpoint::operator + (Manipulator::Viewpoint &other) {
    glm::vec3 focalPoint = focalPoint_ + other.focalPoint_;
    float range = range_ + other.range_;
    glm::quat quat = quat_ * other.quat_;

    return Viewpoint(focalPoint, range, quat);
  }

  void Manipulator::Viewpoint::slerp(float t, Viewpoint &to) {
    focalPoint_ = focalPoint_ + (to.focalPoint() - focalPoint_) * t;
    range_ = range_ + (to.range() - range_) * t;
    quat_ = glm::slerp(quat_, to.quat(), t);
  }

  void Manipulator::Viewpoint::eulerAngle(glm::vec3 &eulerAngle) {
    quat_ = glm::quat(eulerAngle);
  }

  glm::vec3 Manipulator::Viewpoint::eulerAngle() const {
    return glm::eulerAngles(quat_);
  }

  bool Manipulator::Flight::valid() {
    return duration_s_ > 0.000001f;
  }

  void Manipulator::Flight::reset() {
    duration_s_ = 0.0f;
  }

  Manipulator::Manipulator() :
    distance_(1.0f),
    center_(0.f, 0.f, 0.f),
    offset_(0.f, 0.f, 0.f),
    rotation_(glm::identity<glm::quat>()),
    trackballSize_(6.0f),
    rotate_speed_(15.f),
    pan_speed_(1.f),
    rotate_center_(false),
    wheelZoomFactor_(0.1f),
    minimumDistance_(0.05f),
    maximumDistance_(300.f),
    pressed_(false) {
    flight_.reset();
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
    if (speed > 0.0001f) {
      rotate_speed_ = speed;
    }
  }

  float Manipulator::rotateSpeed() {
    return rotate_speed_;
  }

  void Manipulator::panSpeed(float speed) {
    if (speed > 0.0001f) {
      pan_speed_ = speed;
    }
  }
  float Manipulator::panSpeed() {
    return pan_speed_;
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
      fly(Timer::instance().time_s());
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

    flight_.reset();
    glm::vec2 curr_point = camera_->windowToProject({ x, y });

    if (rotate) {
      rotateTrackball(curr_point, last_point_);
    } else {
      glm::vec2 e = curr_point - last_point_;
      e *= distance_ * tanf(glm::radians(camera_->fov()) / 2.f);
      e *= pan_speed_;
      pan(e.x, e.y, 0.f);
    }
    last_point_ = curr_point;
  }

  void Manipulator::mouseScroll(bool zoomin) {
    zoom(zoomin ? wheelZoomFactor_ : -wheelZoomFactor_);
  }

  Manipulator::Viewpoint Manipulator::createViewpoint(BoundingSphere &sphere) {
    if (sphere.valid()) {
      if (camera_) {
        float dist = sphere.radius();
        float angle = glm::radians(camera_->fov() / 2.f);
        if (fabs(angle) > 0.00000001f) {
          dist /= tanf(angle);
        }

        glm::quat quat(glm::identity<glm::quat>());
        return Viewpoint(sphere.center(), dist, quat);
      }
    }

    return Viewpoint();
  }

  Manipulator::Viewpoint Manipulator::viewpoint() {
    glm::quat rot = glm::inverse(rotation_);
    return Viewpoint(center_, distance_, rot);
  }

  void Manipulator::viewpoint(Viewpoint &vp, float duration_s) {
    if (!vp.valid()) {
      return;
    }

    if (duration_s > 0.000001f) {
      flight_.start_viewpoint_ = viewpoint();
      flight_.end_viewpoint_ = vp;
      flight_.start_offset_ = offset_;
      flight_.duration_s_ = duration_s;
      flight_.time_s_ = Timer::instance().time_s();
    } else {
      center(vp.focalPoint());
      distance(vp.range());

      offset_ = { 0.f, 0.f, 0.f };

      glm::quat quat(glm::inverse(vp.quat()));
      rotation(quat);
    }
  }

  void Manipulator::fly(float time_s) {
    if (!flight_.valid()) {
      return;
    }

    double t = (time_s - flight_.time_s_) / flight_.duration_s_;
    float tp = t;

    if (t > 1.0f) {
      flight_.duration_s_ = -1.f;
      return;
    }

    tp = accelerationInterp(tp, 0.4);
    // fade-in/out
    tp = smoothStepInterp(tp);
    tp = smoothStepInterp(tp);

    Viewpoint new_vp = flight_.start_viewpoint_;
    new_vp.slerp(tp, flight_.end_viewpoint_);

    viewpoint(new_vp);

    offset_ = flight_.start_offset_ * (1.0f - tp);
  }
}