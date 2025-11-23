/*
 * Scene Graph Example
 *
 * This example demonstrates:
 * - Scene graph hierarchy with parent-child relationships
 * - Resource management system
 * - Multiple objects with different materials
 * - Transform hierarchies and world space transformations
 * - Rendering with the scene renderer
 */

#include <XVRenderer/Window.h>
#include <XVRenderer/RenderAPI.h>
#include <XVRenderer/OpenGL/GLRenderer.h>
#include <XVRenderer/Scene.h>
#include <XVRenderer/SceneRenderer.h>

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
      "XVRenderer - Scene Graph Example",  // title
      1280,                                  // width
      720,                                   // height
      XV::WindowMode::Windowed,              // mode
      true,                                  // visible
      true,                                  // resizable
      XV::RenderAPI::OpenGL                  // Use OpenGL backend
    };

    XV::Window window(window_config);
    std::cout << "[Example] Window created\n";

    // Create OpenGL renderer
    XV::GLRenderer renderer(window);
    renderer.SetViewport(0, 0, window.GetWidth(), window.GetHeight());
    renderer.EnableDepthTest(true);
    std::cout << "[Example] Renderer initialized\n";

    // Create scene
    XV::Scene scene("MainScene", &renderer);
    std::cout << "[Example] Scene created\n";

    // Setup camera
    auto& camera = scene.GetMainCamera();
    camera.SetPosition(glm::vec3(0.0f, 3.0f, 10.0f));
    camera.SetAspectRatio(static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()));
    camera.Rotate(0.0f, -15.0f); // Look down slightly

    // Create resources via resource manager
    auto& resources = scene.GetResourceManager();

    // Create meshes
    auto* cube_mesh = resources.CreateCubeMesh("Cube");
    auto* plane_mesh = resources.CreatePlaneMesh(10.0f, 10.0f, "Ground");
    std::cout << "[Example] Meshes created\n";

    // Create materials with different colors
    auto* red_material = resources.CreateColoredMaterial(glm::vec3(1.0f, 0.2f, 0.2f), "RedMaterial");
    auto* green_material = resources.CreateColoredMaterial(glm::vec3(0.2f, 1.0f, 0.2f), "GreenMaterial");
    auto* blue_material = resources.CreateColoredMaterial(glm::vec3(0.2f, 0.2f, 1.0f), "BlueMaterial");
    auto* ground_material = resources.CreateColoredMaterial(glm::vec3(0.5f, 0.5f, 0.5f), "GroundMaterial");
    std::cout << "[Example] Materials created\n";

    // Create scene hierarchy
    auto* root = scene.GetRoot();

    // Ground plane
    auto* ground = root->AddChild("Ground");
    ground->SetMesh(plane_mesh);
    ground->SetMaterial(ground_material);
    ground->GetTransform().SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

    // Parent cube (rotates)
    auto* parent_cube = root->AddChild("ParentCube");
    parent_cube->SetMesh(cube_mesh);
    parent_cube->SetMaterial(red_material);
    parent_cube->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Child cube 1 (orbits around parent)
    auto* child1 = parent_cube->AddChild("Child1");
    child1->SetMesh(cube_mesh);
    child1->SetMaterial(green_material);
    child1->GetTransform().SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    child1->GetTransform().SetScale(0.5f);

    // Child cube 2 (orbits around parent)
    auto* child2 = parent_cube->AddChild("Child2");
    child2->SetMesh(cube_mesh);
    child2->SetMaterial(blue_material);
    child2->GetTransform().SetPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    child2->GetTransform().SetScale(0.5f);

    // Grandchild (orbits around child1)
    auto* grandchild = child1->AddChild("Grandchild");
    grandchild->SetMesh(cube_mesh);
    grandchild->SetMaterial(green_material);
    grandchild->GetTransform().SetPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    grandchild->GetTransform().SetScale(0.5f);

    std::cout << "[Example] Scene hierarchy created:\n";
    std::cout << "  - Ground (plane)\n";
    std::cout << "  - ParentCube (red, rotates)\n";
    std::cout << "    - Child1 (green, smaller, orbits parent)\n";
    std::cout << "      - Grandchild (green, tiny, orbits child1)\n";
    std::cout << "    - Child2 (blue, smaller, orbits parent)\n";

    // Create scene renderer
    XV::SceneRenderer scene_renderer(&renderer);
    std::cout << "[Example] Scene renderer created\n";

    // Main loop
    std::cout << "\n[Example] Entering main loop...\n";
    std::cout << "Controls:\n";
    std::cout << "  ESC - Close window\n";
    std::cout << "  Observe the hierarchical transformations!\n\n";

    float time = 0.0f;

    while (!window.ShouldClose())
    {
      time += 0.016f; // ~60 FPS

      // Clear screen
      renderer.Clear(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f));

      // Animate parent cube rotation
      parent_cube->GetTransform().SetRotation(
        glm::vec3(time * 0.5f, time, time * 0.3f)
      );

      // Animate child1 local rotation
      child1->GetTransform().SetRotation(
        glm::vec3(0.0f, time * 2.0f, 0.0f)
      );

      // Animate grandchild local rotation
      grandchild->GetTransform().SetRotation(
        glm::vec3(time * 3.0f, 0.0f, 0.0f)
      );

      // Render the scene
      scene_renderer.Render(scene);

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
