#include "scene.h"

namespace cpuRE {
  std::shared_ptr<Scene> createTriangle() {
    auto mesh = std::make_shared<Mesh>();
    mesh->vertices = {
      { -1.0f, -1.0f, 0.0f },
      {  1.0f, -0.4f, 0.0f },
      { -0.2f,  1.0f, 0.0f }
    };

    mesh->normals = {
      { 0.0f,  0.0f, -1.0f },
      { 0.0f,  0.0f, -1.0f },
      { 0.0f,  0.0f, -1.0f }
    };

    mesh->indices = {
      0, 1, 2
    };

    auto node = std::make_shared<Node>();
    node->meshs = { mesh };
  
    auto scene = std::make_shared<Scene>();
    scene->nodes = { node };
    return scene;
  }

  std::shared_ptr<Scene> createIcosahedron() {
  	const float phi = 1.6180339887498948482045868343656f;
  	const float iphi = 1.0f / phi;
    const auto n = glm::normalize(glm::vec2(1.0f, phi - 1.0f));

    auto mesh = std::make_shared<Mesh>();
    mesh->vertices = {
      { -1.0f,  1.0f, -1.0f },
      { -phi,   0.0f, -iphi },
      { -1.0f, -1.0f, -1.0f },
      {  0.0f, -iphi, -phi  },
      {  0.0f,  iphi, -phi  },

      {  1.0f,  1.0f, -1.0f },
      {  0.0f,  iphi, -phi  },
      {  0.0f, -iphi, -phi  },
      {  1.0f, -1.0f, -1.0f },
      {  phi,   0.0f, -iphi },

      {  1.0f,  1.0f, -1.0f },
      {  phi,   0.0f, -iphi },
      {  phi,   0.0f,  iphi },
      {  1.0f,  1.0f,  1.0f },
      {  iphi,  phi,   0.0f },

      {  1.0f,  1.0f,  1.0f },
      {  0.0f,  iphi,  phi  },
      { -1.0f,  1.0f,  1.0f },
      { -iphi,  phi,   0.0f },
      {  iphi,  phi,   0.0f },

      { -1.0f,  1.0f,  1.0f },
      { -phi,   0.0f,  iphi },
      { -phi,   0.0f, -iphi },
      { -1.0f,  1.0f, -1.0f },
      { -iphi,  phi,   0.0f },

      { -1.0f,  1.0f, -1.0f },
      {  0.0f,  iphi, -phi  },
      {  1.0f,  1.0f, -1.0f },
      {  iphi,  phi,   0.0f },
      { -iphi,  phi,   0.0f },

      { -1.0f, -1.0f, -1.0f },
      { -iphi, -phi,   0.0f },
      {  iphi, -phi,   0.0f },
      {  1.0f, -1.0f, -1.0f },
      {  0.0f, -iphi, -phi  },

      {  1.0f, -1.0f, -1.0f },
      {  iphi, -phi,   0.0f },
      {  1.0f, -1.0f,  1.0f },
      {  phi,   0.0f,  iphi },
      {  phi,   0.0f, -iphi },

      {  1.0f, -1.0f,  1.0f },
      {  0.0f, -iphi,  phi  },
      {  0.0f,  iphi,  phi  },
      {  1.0f,  1.0f,  1.0f },
      {  phi,   0.0f,  iphi },

      { -1.0f, -1.0f,  1.0f },
      { -phi,   0.0f,  iphi },
      { -1.0f,  1.0f,  1.0f },
      {  0.0f,  iphi,  phi  },
      {  0.0f, -iphi,  phi  },

      { -1.0f, -1.0f,  1.0f },
      { -iphi, -phi,   0.0f },
      { -1.0f, -1.0f, -1.0f },
      { -phi,   0.0f, -iphi },
      { -phi,   0.0f,  iphi },

      {  1.0f, -1.0f,  1.0f },
      {  iphi, -phi,   0.0f },
      { -iphi, -phi,   0.0f },
      { -1.0f, -1.0f,  1.0f },
      {  0.0f, -iphi,  phi  }
    };

    mesh->normals = {
      { -n.y, 0.0f, -n.x },
      { -n.y, 0.0f, -n.x },
      { -n.y, 0.0f, -n.x },
      { -n.y, 0.0f, -n.x },
      { -n.y, 0.0f, -n.x },

      {  n.y, 0.0f, -n.x },
      {  n.y, 0.0f, -n.x },
      {  n.y, 0.0f, -n.x },
      {  n.y, 0.0f, -n.x },
      {  n.y, 0.0f, -n.x },

      {  n.x,  n.y, 0.0f },
      {  n.x,  n.y, 0.0f },
      {  n.x,  n.y, 0.0f },
      {  n.x,  n.y, 0.0f },
      {  n.x,  n.y, 0.0f },

      { 0.0f,  n.x,  n.y },
      { 0.0f,  n.x,  n.y },
      { 0.0f,  n.x,  n.y },
      { 0.0f,  n.x,  n.y },
      { 0.0f,  n.x,  n.y },

      { -n.x,  n.y, 0.0f },
      { -n.x,  n.y, 0.0f },
      { -n.x,  n.y, 0.0f },
      { -n.x,  n.y, 0.0f },
      { -n.x,  n.y, 0.0f },

      { 0.0f,  n.x, -n.y },
      { 0.0f,  n.x, -n.y },
      { 0.0f,  n.x, -n.y },
      { 0.0f,  n.x, -n.y },
      { 0.0f,  n.x, -n.y },

      { 0.0f, -n.x, -n.y },
      { 0.0f, -n.x, -n.y },
      { 0.0f, -n.x, -n.y },
      { 0.0f, -n.x, -n.y },
      { 0.0f, -n.x, -n.y },

      {  n.x, -n.y, 0.0f },
      {  n.x, -n.y, 0.0f },
      {  n.x, -n.y, 0.0f },
      {  n.x, -n.y, 0.0f },
      {  n.x, -n.y, 0.0f },

      {  n.y, 0.0f,  n.x },
      {  n.y, 0.0f,  n.x },
      {  n.y, 0.0f,  n.x },
      {  n.y, 0.0f,  n.x },
      {  n.y, 0.0f,  n.x },

      { -n.y, 0.0f,  n.x },
      { -n.y, 0.0f,  n.x },
      { -n.y, 0.0f,  n.x },
      { -n.y, 0.0f,  n.x },
      { -n.y, 0.0f,  n.x },

      { -n.x, -n.y, 0.0f },
      { -n.x, -n.y, 0.0f },
      { -n.x, -n.y, 0.0f },
      { -n.x, -n.y, 0.0f },
      { -n.x, -n.y, 0.0f },

      { 0.0f, -n.x,  n.y },
      { 0.0f, -n.x,  n.y },
      { 0.0f, -n.x,  n.y },
      { 0.0f, -n.x,  n.y },
      { 0.0f, -n.x,  n.y }
    };

    mesh->indices = {
       1,  0,  2,  2,  0,  3,  3,  0,  4,
       6,  5,  7,  7,  5,  8,  8,  5,  9,
      11, 10, 12, 12, 10, 13, 13, 10, 14,
      16, 15, 17, 17, 15, 18, 18, 15, 19,
      21, 20, 22, 22, 20, 23, 23, 20, 24,
      26, 25, 27, 27, 25, 28, 28, 25, 29,
      31, 30, 32, 32, 30, 33, 33, 30, 34,
      36, 35, 37, 37, 35, 38, 38, 35, 39,
      41, 40, 42, 42, 40, 43, 43, 40, 44,
      46, 45, 47, 47, 45, 48, 48, 45, 49,
      51, 50, 52, 52, 50, 53, 53, 50, 54,
      56, 55, 57, 57, 55, 58, 58, 55, 59
    };

    auto node = std::make_shared<Node>();
    node->meshs = { mesh };
  
    auto scene = std::make_shared<Scene>();
    scene->nodes = { node };
    return scene;
  }
}