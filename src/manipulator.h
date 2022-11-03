#ifndef __MANIPULATOR_H__
#define __MANIPULATOR_H__

#include <memory>
#include <string>
#include <glm/ext.hpp>
#include "camera.h"
#include "bounding.h"

namespace cpuRE {
  class Manipulator {
  public:
    class Viewpoint {
    public:
      Viewpoint();
      Viewpoint(const Viewpoint& viewpoint);
      Viewpoint(glm::vec3& focal, float range, glm::vec3& eulerAngle);
      Viewpoint(glm::vec3& focal, float range, glm::quat& quat);

      Viewpoint operator - (Viewpoint& other);
      Viewpoint operator + (Viewpoint& other);

      bool valid();

      void slerp(float t, Viewpoint& to);

      void name(std::string& name) { name_ = name; }
      const std::string& name() const { return name_; }

      void focalPoint(glm::vec3& point) { focalPoint_ = point; }
      const glm::vec3& focalPoint() const { return focalPoint_; }

      void eulerAngle(glm::vec3& eulerAngle);
      glm::vec3 eulerAngle() const;

      const glm::quat& quat() const { return quat_; }
      const void quat(const glm::quat& quat) { quat_ = quat; }

      void range(float range) { range_ = range; }
      float range() const { return range_; }

    protected:
      std::string name_;
      glm::vec3 focalPoint_;
      glm::quat quat_;
      float range_;
    };

    Manipulator();

    void camera(const std::shared_ptr<Camera>& camera);

    bool valid() { return distance_ > 0.f && camera_; }

    void center(const glm::vec3 &center);
    const glm::vec3 &center() const;

    void distance(float distance);
    float distance();

    void rotation(const glm::quat &rotation);
    glm::quat &rotation();

    void rotateSpeed(float speed);
    float rotateSpeed();

    void panSpeed(float speed);
    float panSpeed();

    void zoom(float scale);

    void rotate(glm::vec3& axis, float angle);
    void rotate(glm::quat& quat);

    void pan(float dx, float dy, float dz);

    void home();

    void apply();

    Viewpoint createViewpoint(BoundingSphere& sphere);
    Viewpoint viewpoint();
    void viewpoint(Viewpoint& viewpoint, float duration_s = 0.0);

    void mousePress(int x, int y);
    void mouseRelease(int x, int y);

    void mouseMove(int x, int y, bool rotate);

    void mouseScroll(bool zoomin);

  private:
    glm::mat4 matrix();
    glm::mat4 inverseMatrix();

    bool rotateTrackball(const glm::vec2& p0, const glm::vec2& p1);

    void fly(float time_s);

    float distance_;
    glm::vec3 center_;
    glm::vec3 offset_;
    glm::quat rotation_;

    float trackballSize_;
    float rotate_speed_;
    float pan_speed_;

    float wheelZoomFactor_;
    float minimumDistance_;
    float maximumDistance_;

    //是否绕center旋转
    bool rotate_center_;

    bool pressed_;
    glm::vec2 last_point_;

    std::shared_ptr<Camera> camera_;

    struct Flight {
      Viewpoint start_viewpoint_;
      Viewpoint end_viewpoint_;
      glm::vec3 start_offset_;
      float duration_s_;
      double time_s_;

      bool valid();
      void reset();
    };

    Flight flight_;
  };
}
#endif
