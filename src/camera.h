#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>

namespace cpuRE {
  class Camera {
  public:
    Camera();

    void fov(float fov);
    float fov() const { return fov_; }
    
    void near(float near);
    float near() const { return near_; }

    void far(float far);
    float far() const { return far_; }

    glm::mat4& mv() { return mv_; }
    glm::mat4& projection() { return projection_; }

    glm::mat4 mvp() { return projection_ * mv_; }

    void viewport(int x, int y, int width, int height);
    const glm::ivec4& viewport() const { return viewport_; }

    bool orthogonal() const;

    void ortho(int x, int y, int width, int height);
    void perspective(int x, int y, int width, int height);

    glm::vec2 windowToProject(const glm::vec2& window) const;
    glm::vec2 projectToWindow(const glm::vec2& project) const;

  private:
    float fov_, near_, far_;
    glm::mat4 mv_, projection_;
    glm::ivec4 viewport_;
  };
}

#endif