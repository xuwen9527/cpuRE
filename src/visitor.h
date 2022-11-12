#ifndef __VISITOR_H__
#define __VISITOR_H__

#include <stack>
#include <glm/glm.hpp>
#include "bounding.h"

namespace cpuRE {
  struct Mesh;
  struct Node;
  struct GroupNode;

  struct Visitor {
    Visitor();

    virtual void traverse(Mesh& mesh);
    virtual void traverse(Node& node);
    virtual void traverse(GroupNode& groupNode);

  protected:
    std::stack<glm::mat4> m;
  };

  struct BoundingSphereVisitor : public Visitor {
    BoundingSphereVisitor();
    void traverse(Mesh& mesh) override;

    BoundingSphere sphere;
  };
}

#endif