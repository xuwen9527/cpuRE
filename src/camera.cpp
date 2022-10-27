#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_query.hpp>
#include "camera.h"

namespace cpuRE {
  Camera::Camera() : fov_(90.f), near_(0.1f), far_(1000.0f),
    mv_(glm::identity<glm::mat4>()) {
    perspective(0, 0, 1024, 1024);
  }

  void Camera::fov(float fov) {
    if (!orthogonal()) {
      fov_ = fov;
      perspective(viewport_.x, viewport_.y, viewport_.z, viewport_.w);
    }
  }

  void Camera::near(float near) {
    if (!orthogonal()) {
      near_ = near;
      perspective(viewport_.x, viewport_.y, viewport_.z, viewport_.w);
    }
  }

  void Camera::far(float far) {
    if (!orthogonal()) {
      far_ = far;
      perspective(viewport_.x, viewport_.y, viewport_.z, viewport_.w);
    }
  }

  void Camera::viewport(int x, int y, int width, int height) {
    if (orthogonal()) {
      ortho(x, y, width, height);
    } else {
      perspective(x, y, width, height);
    }
  }

  bool Camera::orthogonal() const {
    return glm::isOrthogonal(projection_, 0.00001f);
  }

  void Camera::ortho(int x, int y, int width, int height) {
    viewport_ = { x, y, width, height };
    projection_ = glm::ortho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, -1000.f, 1000.f);
  }

  void Camera::perspective(int x, int y, int width, int height) {
    viewport_ = { x, y, width, height };
    projection_ = glm::perspective(glm::radians(fov_), (float)width / height, near_, far_);
  }

  glm::vec2 Camera::windowToProject(const glm::vec2& window)  const {
    return {
      viewport_.z < 1 ? 0.f : (window.x - viewport_.x) / viewport_.z * 2.f - 1.f,
      viewport_.w < 1 ? 0.f : (viewport_.w - viewport_.y - window.y) / viewport_.w * 2.f - 1.f
    };
  }

  glm::vec2 Camera::projectToWindow(const glm::vec2& project) const {
    return {
      (project.x + 1.f) / 2.f * viewport_.z + viewport_.x,
      viewport_.w - viewport_.y - (project.y + 1.f) / 2.f * viewport_.w
    };
  }
}