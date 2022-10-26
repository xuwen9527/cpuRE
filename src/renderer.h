#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <glm/glm.hpp>
#include "camera.h"
#include "manipulator.h"
#include "framebuffer.h"
#include "pipeline/geometry.h"
#include "pipeline/context.h"

namespace cpuRE {
  class Renderer {
  public:
    Renderer();

    void render();

    auto& camera() { return camera_; }
    auto& manipulator() { return manipulator_; }
    auto& framebuffer() { return framebuffer_; }
    auto& options() { return options_; }

  private:
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Manipulator> manipulator_;

    std::shared_ptr<Geometry> geometry_;

    FrameBuffer framebuffer_;

    Options options_;
	};
}

#endif