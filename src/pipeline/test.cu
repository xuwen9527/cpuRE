#include <glm/glm.hpp>
#include <tuple>
#include <stdio.h>
#include <thrust/device_vector.h>

__device__ __host__ void a() {
  thrust::device_vector<int> a;
}

// __global__ void test() {
//   thrust::tuple<glm::vec3, glm::vec4> t;

//   glm::vec3 a = {0.f, 0.f, 0.f};
//   glm::vec3 b = {1.f, 1.f, 1.f};

//   auto c = a + b;
//   printf("\n");
//   auto d = std::get<0>(t);
// }