#include "renderer.h"
#include "pipeline/primitive_types.h"
#include "pipeline/shader.h"
#include "pipeline/viewport.h"
#include "pipeline/pipeline.h"

namespace cpuRE {
  using SimplePipeline = Pipeline<SimpleVertexShader, SimpleFragmentShader, TrianglesPrimitiveType>;

  struct RenderVisitor : public Visitor {
    RenderVisitor(Context& context, Camera& camera) :
      Visitor(), context_(context), camera_(camera) {}

    void traverse(Mesh& mesh) override {
      context_.mvp = camera_.mvp() * m.top();
      SimplePipeline::run({
        mesh.vertices.data(),
        mesh.normals.data(),
        mesh.indices.data(),
        mesh.indices.size()
      }, context_);
    }

    Context& context_;
    Camera& camera_;
  };

  Renderer::Renderer() : camera_(std::make_shared<Camera>()),
    manipulator_(std::make_shared<Manipulator>()) {
    manipulator_->camera(camera_);
    scene_ = createTriangle();
  }

  void Renderer::render() {
    auto& viewport = camera_->viewport();
    if (viewport.z < 2 || viewport.w < 2) {
      return;
    }

    framebuffer_.resize({ viewport.z, viewport.w });
    framebuffer_.apply();

    Context context;
    context.viewport = viewport;
    context.pixel_scale = computePixelScale(context.viewport);
    context.color_buffer = framebuffer_.colorBuffer().get();
    context.depth_buffer = framebuffer_.depthBuffer().get();
    context.options = options_;
    context.status = status_;

    RenderVisitor visitor(context, *camera_);
    scene_->accept(visitor);

    framebuffer_.updateTexture();
  }
}