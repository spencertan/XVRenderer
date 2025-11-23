#pragma once

#include "Mesh.h"
#include "Material.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <memory>
#include <unordered_map>
#include <string>

namespace XV
{

// Forward declarations
class IRenderer;

// Resource manager for meshes, materials, textures, etc.
class ResourceManager : NonCopyable
{
  IRenderer* m_renderer = nullptr;

  // Resource storage
  std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
  std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;

  // Resource counters for unique naming
  u32 m_mesh_counter = 0;
  u32 m_material_counter = 0;

public:
  ResourceManager() = default;
  explicit ResourceManager(IRenderer* renderer);

  // Set the renderer (for uploading resources)
  void SetRenderer(IRenderer* renderer) { m_renderer = renderer; }

  // === Mesh Management ===

  // Create and register a mesh
  Mesh* CreateMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
  Mesh* CreateMesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices); // Auto-named

  // Create predefined meshes
  Mesh* CreateCubeMesh(const std::string& name = "");
  Mesh* CreatePlaneMesh(float width = 1.0f, float height = 1.0f, const std::string& name = "");

  // Get mesh by name
  Mesh* GetMesh(const std::string& name);
  const Mesh* GetMesh(const std::string& name) const;

  // === Material Management ===

  // Create and register a material
  Material* CreateMaterial(const std::string& name, const MaterialProperties& props = {});
  Material* CreateMaterial(const MaterialProperties& props = {}); // Auto-named

  // Create predefined materials
  Material* CreateDefaultMaterial(const std::string& name = "");
  Material* CreateColoredMaterial(const glm::vec3& color, const std::string& name = "");

  // Get material by name
  Material* GetMaterial(const std::string& name);
  const Material* GetMaterial(const std::string& name) const;

  // === Resource Cleanup ===

  void Clear();
  void ClearMeshes();
  void ClearMaterials();

  // === Statistics ===

  u32 GetMeshCount() const { return static_cast<u32>(m_meshes.size()); }
  u32 GetMaterialCount() const { return static_cast<u32>(m_materials.size()); }

private:
  std::string GenerateMeshName();
  std::string GenerateMaterialName();
};

}
