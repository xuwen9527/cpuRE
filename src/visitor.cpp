#include "visitor.h"
#include "scene.h"
#include "glm/ext.hpp"

namespace cpuRE {
  Visitor::Visitor() {
    m.emplace(glm::identity<glm::mat4>());
  }

  void Visitor::traverse(Mesh& mesh) {}

  void Visitor::traverse(Node& node) {
    m.emplace(m.top() * node.m);
    for (auto& mesh : node.meshs) {
      traverse(*mesh);
    }
    m.pop();
  }

  void Visitor::traverse(GroupNode& groupNode) {
    m.emplace(m.top() * groupNode.m);
    for (auto& node : groupNode.nodes) {
      traverse(*node);
    }
    m.pop();
  }

  BoundingSphereVisitor::BoundingSphereVisitor() : Visitor() {

  }

  void BoundingSphereVisitor::traverse(Mesh& mesh) {
    BoundingBox box;
    box.expandBy(mesh.vertices);

    auto transform = m.top();
    glm::vec4 min = transform * glm::vec4(box.min(), 1.f);
    glm::vec4 max = transform * glm::vec4(box.max(), 1.f);

    sphere.expandBy(glm::vec3(min / min.w));
    sphere.expandBy(glm::vec3(max / max.w));
  }
}