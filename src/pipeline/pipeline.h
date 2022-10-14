#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "primitive_types.h"
#include "geometry_stage.h"
#include "rasterization_stage.h"
#include "shader.h"

namespace cpuRE {
  template <typename VertexShader, typename FragmentShader, typename PrimitiveType>
  struct Pipeline {
    using GeometryStage = GeometryStage<VertexShader, PrimitiveType>;
    using RasterizationStage = RasterizationStage<FragmentShader>;

    static void run(Context& context, Geometry& geometry) {
      TriangleBuffer triangle_buffer;
      GeometryStage::run(context, triangle_buffer, geometry);
      RasterizationStage::run(context, triangle_buffer);
    }
  };

  using SimplePipeline = Pipeline<SimpleVertexShader, SimpleFragmentShader, TrianglesPrimitiveType>;
}

#endif