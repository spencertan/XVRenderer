#include "ResourceManager.h"
#include "IRenderer.h"

#include <stdexcept>
#include <iostream>

namespace XV
{

ResourceManager::ResourceManager(IRenderer* renderer)
  : m_renderer(renderer)
{
}

// === Mesh Management ===

Mesh* ResourceManager::CreateMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
  if (m_meshes.find(name) != m_meshes.end())
  {
    std::cerr << "[ResourceManager] Mesh '" << name << "' already exists, returning existing mesh\n";
    return m_meshes[name].get();
  }

  auto mesh = std::make_unique<Mesh>(vertices, indices);

  if (m_renderer)
  {
    mesh->UploadToGPU(m_renderer);
  }

  Mesh* mesh_ptr = mesh.get();
  m_meshes[name] = std::move(mesh);

  std::cout << "[ResourceManager] Created mesh '" << name << "' ("
            << vertices.size() << " vertices, " << indices.size() << " indices)\n";

  return mesh_ptr;
}

Mesh* ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
  return CreateMesh(GenerateMeshName(), vertices, indices);
}

Mesh* ResourceManager::CreateCubeMesh(const std::string& name)
{
  std::string mesh_name = name.empty() ? GenerateMeshName() : name;
  return CreateMesh(mesh_name, Vertex::GetCubeVertices(), Vertex::GetCubeIndices());
}

Mesh* ResourceManager::CreatePlaneMesh(float width, float height, const std::string& name)
{
  std::string mesh_name = name.empty() ? GenerateMeshName() : name;
  Mesh temp_mesh = Mesh::CreatePlane(width, height);
  return CreateMesh(mesh_name, temp_mesh.GetVertices(), temp_mesh.GetIndices());
}

Mesh* ResourceManager::GetMesh(const std::string& name)
{
  auto it = m_meshes.find(name);
  if (it != m_meshes.end())
  {
    return it->second.get();
  }
  return nullptr;
}

const Mesh* ResourceManager::GetMesh(const std::string& name) const
{
  auto it = m_meshes.find(name);
  if (it != m_meshes.end())
  {
    return it->second.get();
  }
  return nullptr;
}

// === Material Management ===

Material* ResourceManager::CreateMaterial(const std::string& name, const MaterialProperties& props)
{
  if (m_materials.find(name) != m_materials.end())
  {
    std::cerr << "[ResourceManager] Material '" << name << "' already exists, returning existing material\n";
    return m_materials[name].get();
  }

  auto material = std::make_unique<Material>(name, props);

  if (m_renderer)
  {
    material->Compile(m_renderer);
  }

  Material* material_ptr = material.get();
  m_materials[name] = std::move(material);

  std::cout << "[ResourceManager] Created material '" << name << "'\n";

  return material_ptr;
}

Material* ResourceManager::CreateMaterial(const MaterialProperties& props)
{
  return CreateMaterial(GenerateMaterialName(), props);
}

Material* ResourceManager::CreateDefaultMaterial(const std::string& name)
{
  std::string mat_name = name.empty() ? GenerateMaterialName() : name;
  MaterialProperties props;
  return CreateMaterial(mat_name, props);
}

Material* ResourceManager::CreateColoredMaterial(const glm::vec3& color, const std::string& name)
{
  std::string mat_name = name.empty() ? GenerateMaterialName() : name;
  MaterialProperties props;
  props.albedo = color;
  return CreateMaterial(mat_name, props);
}

Material* ResourceManager::GetMaterial(const std::string& name)
{
  auto it = m_materials.find(name);
  if (it != m_materials.end())
  {
    return it->second.get();
  }
  return nullptr;
}

const Material* ResourceManager::GetMaterial(const std::string& name) const
{
  auto it = m_materials.find(name);
  if (it != m_materials.end())
  {
    return it->second.get();
  }
  return nullptr;
}

// === Resource Cleanup ===

void ResourceManager::Clear()
{
  ClearMeshes();
  ClearMaterials();
}

void ResourceManager::ClearMeshes()
{
  m_meshes.clear();
  m_mesh_counter = 0;
}

void ResourceManager::ClearMaterials()
{
  m_materials.clear();
  m_material_counter = 0;
}

// === Private Helpers ===

std::string ResourceManager::GenerateMeshName()
{
  return "Mesh_" + std::to_string(m_mesh_counter++);
}

std::string ResourceManager::GenerateMaterialName()
{
  return "Material_" + std::to_string(m_material_counter++);
}

}
