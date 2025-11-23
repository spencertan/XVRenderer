#include "Vertex.h"
#include <XVUtilities/Types.h>

namespace XV
{

std::vector<Vertex> Vertex::GetCubeVertices()
{
  return {
    // Front face (red variations)
    {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}}, // 0: bottom-left  (red)
    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.5f, 0.0f}}, // 1: bottom-right (red-orange)
    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}}, // 2: top-right    (yellow)
    {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.5f}}, // 3: top-left     (red-magenta)

    // Back face (blue variations)
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 4: bottom-left  (blue)
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 1.0f}}, // 5: bottom-right (blue-cyan)
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}}, // 6: top-right    (cyan)
    {{-0.5f,  0.5f, -0.5f}, {0.5f, 0.0f, 1.0f}}, // 7: top-left     (blue-magenta)
  };
}

std::vector<u32> Vertex::GetCubeIndices()
{
  return {
    // Front face
    0, 1, 2,
    2, 3, 0,

    // Back face
    5, 4, 7,
    7, 6, 5,

    // Left face
    4, 0, 3,
    3, 7, 4,

    // Right face
    1, 5, 6,
    6, 2, 1,

    // Top face
    3, 2, 6,
    6, 7, 3,

    // Bottom face
    4, 5, 1,
    1, 0, 4
  };
}

}
