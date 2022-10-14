#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>

namespace cpuRE {
  class Camera {
  public:
    Camera();

    glm::mat4& mv() { return mv_; }
    glm::mat4& projection() { return projection_; }

    glm::mat4 mvp() { return projection_ * mv_; }

    void viewport(int x, int y, int width, int height);
    const glm::ivec4& viewport() const { return viewport_; }

    void home();

    bool orthogonal() const;

    void orthoViewport(int x, int y, int width, int height);
    void perspectiveViewport(int x, int y, int width, int height);

    void windowToProject(glm::vec2& project, const glm::vec2& window)  const {
      windowToProject(project.x, project.y, window.x, window.y);
    }

    void projectToWindow(const glm::vec2& project, glm::vec2& window)  const {
      projectToWindow(project.x, project.y, window.x, window.y);
    }

    void windowToProject(float &px, float &py, float wx, float wy) const;
    void projectToWindow(float px, float py, float& wx, float& wy)  const;

  protected:
    void perspectiveViewport(int x, int y, int width, int height, float fovy, float near, float far);

    glm::mat4 mv_, projection_;
    glm::ivec4 viewport_;
  };
}

#endif