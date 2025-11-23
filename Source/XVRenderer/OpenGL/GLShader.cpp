#include "OpenGL/GLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

namespace XV
{

Shader::Shader(const char* vertex_source, const char* fragment_source)
{
  u32 vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_source);
  u32 fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_source);
  m_program = LinkProgram(vertex_shader, fragment_shader);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
  if (m_program)
  {
    glDeleteProgram(m_program);
  }
}

Shader::Shader(Shader&& other) noexcept
  : m_program(other.m_program)
{
  other.m_program = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
  if (this != &other)
  {
    if (m_program) glDeleteProgram(m_program);

    m_program = other.m_program;
    other.m_program = 0;
  }
  return *this;
}

void Shader::Bind() const
{
  glUseProgram(m_program);
}

void Shader::Unbind() const
{
  glUseProgram(0);
}

void Shader::SetUniformMat4(const char* name, const glm::mat4& matrix)
{
  i32 location = glGetUniformLocation(m_program, name);
  if (location != -1)
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

u32 Shader::CompileShader(u32 type, const char* source)
{
  u32 shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  i32 success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    std::string shader_type = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    throw std::runtime_error("[Shader] " + shader_type + " compilation failed: " + std::string(info_log));
  }

  return shader;
}

u32 Shader::LinkProgram(u32 vertex_shader, u32 fragment_shader)
{
  u32 program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  i32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    char info_log[512];
    glGetProgramInfoLog(program, 512, nullptr, info_log);
    throw std::runtime_error("[Shader] Program linking failed: " + std::string(info_log));
  }

  return program;
}

Shader Shader::CreateDefault()
{
  const char* vertex_source = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main()
    {
      gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
      vertexColor = aColor;
    }
  )";

  const char* fragment_source = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;

    void main()
    {
      FragColor = vec4(vertexColor, 1.0);
    }
  )";

  return Shader(vertex_source, fragment_source);
}

}
