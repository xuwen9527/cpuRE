#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_query.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "camera.h"

namespace cpuRE {
  Camera::Camera() : mv_(glm::identity<glm::mat4>()) {
    perspectiveViewport(0, 0, 1024, 1024);
  }

  void Camera::viewport(int x, int y, int width, int height) {
    if (orthogonal()) {
      orthoViewport(x, y, width, height);
    } else {
      perspectiveViewport(x, y, width, height);
    }
  }

  bool Camera::orthogonal() const {
    return glm::isOrthogonal(projection_, 0.00001f);
  }

  void Camera::orthoViewport(int x, int y, int width, int height) {
    viewport_ = { x, y, width, height };
    projection_ = glm::ortho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, -1000.f, 1000.f);
  }

  void Camera::perspectiveViewport(int x, int y, int width, int height) {
    perspectiveViewport(x, y, width, height, 90.f, 0.1f, 1000.0f);
  }

  void Camera::perspectiveViewport(int x, int y, int width, int height, float fovy, float near, float far) {
    viewport_ = { x, y, width, height };
    projection_ = glm::perspective(glm::radians(fovy), (float)width / height, near, far);
  }

  void Camera::windowToProject(float &px, float &py, float wx, float wy)  const {
    if (viewport_.z < 2 || viewport_.w < 2) {
      px = wx / viewport_.z;
      py = wy / viewport_.w;
      return;
    }

    px = (wx - viewport_.x) / viewport_.z * 2.f - 1.f;
    py = (viewport_.w - viewport_.y - wy) / viewport_.w * 2.f - 1.f;
  }

  void Camera::projectToWindow(float px, float py, float& wx, float& wy)  const {
    if (viewport_.z < 2 || viewport_.w < 2) {
      wx = px * viewport_.z;
      wy = py * viewport_.w;
      return;
    }

    wx = (px + 1.f) / 2.f * viewport_.z + viewport_.x;
    wy = viewport_.w - viewport_.y - (py + 1.f) / 2.f * viewport_.w;
  }
}