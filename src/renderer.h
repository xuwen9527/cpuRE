#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <glm/glm.hpp>
#include "camera.h"
#include "manipulator.h"
#include "framebuffer.h"
#include "scene.h"
#include "pipeline/context.h"

namespace cpuRE {
  class Renderer {
  public:
    Renderer();

    void render();

    auto& camera() { return camera_; }
    auto& manipulator() { return manipulator_; }
    auto& framebuffer() { return framebuffer_; }
    auto& scene() { return scene_; }
    auto& options() { return options_; }
    auto& status() { return status_; }

  private:
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Manipulator> manipulator_;

    std::shared_ptr<Scene> scene_;

    FrameBuffer framebuffer_;

    Options options_;
    Status status_;
	};
}

#endif