#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "geometry_stage.h"
#include "rasterization_stage.h"
#include "shader.h"

namespace cpuRE {
  template <typename VertexShader, typename FragmentShader>
  struct Pipeline {
    static void run(Context& context, Geometry& geometry) {
      TriangleBuffer triangle_buffer;
      GeometryStage<VertexShader>::run(context, triangle_buffer, geometry);
      RasterizationStage<FragmentShader>::run(context, triangle_buffer);
    }
  };

  using SimplePipeline = Pipeline<SimpleVertexShader, SimpleFragmentShader>;
}

#endif