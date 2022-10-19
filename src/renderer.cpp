#include "renderer.h"
#include "pipeline/pipeline.h"
#include "pipeline/context.h"
#include "pipeline/viewport.h"

namespace {
  template <typename T>
  void alloc_buffer(const glm::ivec2& size, int components, std::unique_ptr<T>& ptr) {
    ptr.reset(new T[components * size.x * size.y]);
    memset(ptr.get(), 0, sizeof(T) * components * size.x * size.y);
  }
}

namespace cpuRE {
  Renderer::Renderer() : camera_(std::make_shared<Camera>()),
    manipulator_(std::make_shared<Manipulator>()) {
    manipulator_->camera(camera_);
    geometry_ = createTriangleGeometry();
    // geometry_ = createIcosahedronGeometry();
  }

  void Renderer::render() {
    auto& viewport = camera_->viewport();
    if (viewport.z < 2 || viewport.w < 2) {
      return;
    }

    framebuffer_.resize(viewport.z, viewport.w);
    framebuffer_.clearColor(glm::vec4(0.45f, 0.55f, 0.60f, 1.0f));
    framebuffer_.clearDepth(1.f);

    Context context;
    context.mvp = camera_->mvp();
    context.viewport = viewport;
    context.pixel_scale = computePixelScale(context.viewport);
    context.color_buffer = framebuffer_.colorBuffer().get();
    context.depth_buffer = framebuffer_.depthBuffer().get();

    SimplePipeline::run(context, *geometry_);

    framebuffer_.updateTexture();
  }
}