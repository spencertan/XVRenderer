#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace XV
{

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;

  static std::vector<Vertex> GetCubeVertices();
  static std::vector<u32> GetCubeIndices();
};

}
