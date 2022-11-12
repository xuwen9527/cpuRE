#ifndef __SCENE_H__
#define __SCENE_H__

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "visitor.h"
#include "bounding.h"

namespace cpuRE {
  struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<std::uint32_t> indices;
    BoundingBox bounds;
	};

  struct Node {
    std::vector<std::shared_ptr<Mesh>> meshs;
    glm::mat4 m = glm::identity<glm::mat4>();

    void accept(Visitor& visitor) {
      visitor.traverse(*this);
    }
  };

  struct GroupNode : public Node {
    std::vector<std::shared_ptr<Node>> nodes;

    void accept(Visitor& visitor) {
      visitor.traverse(*this);
    }
  };

  using Scene = GroupNode;

  std::shared_ptr<Scene> createTriangle();

  std::shared_ptr<Scene> createIcosahedron();
}

#endif