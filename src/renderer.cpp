#include "renderer.h"
#include "pipeline/pipeline.h"
#include "pipeline/viewport.h"

namespace cpuRE {
  Renderer::Renderer() : camera_(std::make_shared<Camera>()),
    manipulator_(std::make_shared<Manipulator>()) {
    manipulator_->camera(camera_);
    geometry_ = createTriangleGeometry();
  }

  void Renderer::render() {
    auto& viewport = camera_->viewport();
    if (viewport.z < 2 || viewport.w < 2) {
      return;
    }

    framebuffer_.resize({ viewport.z, viewport.w });
    framebuffer_.apply();

    Context context;
    context.mvp = camera_->mvp();
    context.viewport = viewport;
    context.pixel_scale = computePixelScale(context.viewport);
    context.color_buffer = framebuffer_.colorBuffer().get();
    context.depth_buffer = framebuffer_.depthBuffer().get();
    context.options = options_;
    context.status = status_;

    SimplePipeline::run(*geometry_, context);

    framebuffer_.updateTexture();
  }
}