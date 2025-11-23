#pragma once

#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>
#include <glm/glm.hpp>
#include <string>

namespace XV
{

class Shader : NonCopyable
{
  u32 m_program = 0;

  u32 CompileShader(u32 type, const char* source);
  u32 LinkProgram(u32 vertex_shader, u32 fragment_shader);

public:
  Shader() = default;
  Shader(const char* vertex_source, const char* fragment_source);
  ~Shader();

  // Move semantics
  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  void Bind() const;
  void Unbind() const;

  void SetUniformMat4(const char* name, const glm::mat4& matrix);

  u32 GetProgram() const { return m_program; }

  // Default shaders
  static Shader CreateDefault();
};

}
