#ifndef __MANIPULATOR_H__
#define __MANIPULATOR_H__

#include <memory>
#include <glm/ext.hpp>
#include "camera.h"

namespace cpuRE {
  class Manipulator {
  public:
    Manipulator();
    ~Manipulator();

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
    // 绕axis旋转angle弧度
    void rotate(glm::vec3& axis, float angle);
    //四元数旋转
    void rotate(glm::quat& quat);
    // x,y,z三轴平移量
    void pan(float dx, float dy, float dz);

    void home();

    void apply();

    void mousePress(int x, int y);
    void mouseRelease(int x, int y);

    void mouseMove(int x, int y, bool rotate);

    void mouseScroll(bool zoomin);

  private:
    glm::mat4 matrix();
    glm::mat4 inverseMatrix();

    bool rotateTrackball(const glm::vec2& p0, const glm::vec2& p1);

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
  };
}
#endif
