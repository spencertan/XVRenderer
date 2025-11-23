/*
 * Colored Cube Example
 *
 * This example demonstrates:
 * - Using the OpenGL renderer backend
 * - Creating and rendering a 3D cube with interpolated vertex colors
 * - Basic camera movement and rotation
 * - Using the buffer and shader abstractions
 */

#include <XVRenderer/Window.h>
#include <XVRenderer/RenderAPI.h>
#include <XVRenderer/OpenGL/GLRenderer.h>
#include <XVRenderer/OpenGL/GLBuffer.h>
#include <XVRenderer/OpenGL/GLShader.h>
#include <XVRenderer/Vertex.h>
#include <XVRenderer/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

int main()
{
  try
  {
    // Create window with OpenGL context
    XV::WindowConfig window_config{
      "XVRenderer - Colored Cube Example",  // title
      1280,                                   // width
      720,                                    // height
      XV::WindowMode::Windowed,               // mode
      true,                                   // visible
      true,                                   // resizable
      XV::RenderAPI::OpenGL                   // Use OpenGL backend
    };

    XV::Window window(window_config);
    std::cout << "[Example] Window created\n";

    // Create OpenGL renderer
    XV::GLRenderer renderer(window);
    renderer.SetViewport(0, 0, window.GetWidth(), window.GetHeight());
    renderer.EnableDepthTest(true);
    std::cout << "[Example] Renderer initialized\n";

    // Create camera
    XV::Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.SetAspectRatio(static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()));
    std::cout << "[Example] Camera created\n";

    // Create cube geometry
    auto vertices = XV::Vertex::GetCubeVertices();
    auto indices = XV::Vertex::GetCubeIndices();

    XV::VertexBuffer vbo(vertices);
    XV::IndexBuffer ebo(indices);
    std::cout << "[Example] Cube mesh created (" << vertices.size() << " vertices, "
              << indices.size() << " indices)\n";

    // Create shader
    XV::Shader shader = XV::Shader::CreateDefault();
    std::cout << "[Example] Shader created\n";

    // Main loop
    float rotation = 0.0f;
    std::cout << "[Example] Entering main loop...\n\n";
    std::cout << "Controls:\n";
    std::cout << "  ESC - Close window\n";
    std::cout << "  The cube will automatically rotate\n\n";

    while (!window.ShouldClose())
    {
      // Clear screen
      renderer.Clear(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f));

      // Update rotation
      rotation += 0.01f;

      // Create model matrix (rotating cube)
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model, rotation, glm::vec3(0.5f, 1.0f, 0.0f));

      // Bind shader and set uniforms
      renderer.BindShader(shader);
      renderer.SetUniformMat4("uModel", model);
      renderer.SetUniformMat4("uView", camera.GetViewMatrix());
      renderer.SetUniformMat4("uProjection", camera.GetProjectionMatrix());

      // Bind buffers and draw
      renderer.BindVertexBuffer(vbo);
      renderer.BindIndexBuffer(ebo);
      renderer.DrawIndexed(static_cast<XV::u32>(indices.size()));

      // Swap buffers and poll events
      renderer.EndFrame();
    }

    std::cout << "\n[Example] Shutting down...\n";
    renderer.WaitIdle();
  }
  catch (const std::exception& e)
  {
    std::cerr << "[Example] Error: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "[Example] Clean exit\n";
  return 0;
}
