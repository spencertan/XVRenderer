# XVRenderer

A cross-platform graphics rendering library with support for both Vulkan and OpenGL.

This is a hobby project for learning modern graphics API design and implementation. The library is part of a larger game engine project.

## Features

✅ **Dual Rendering Backends**
- Vulkan (primary, high-performance)
- OpenGL 3.3+ (fallback, maximum compatibility)
- Automatic API detection and selection

✅ **Cross-Platform Support**
- Windows
- macOS
- Linux

✅ **Modern C++ Design**
- RAII resource management
- Move semantics
- Type-safe abstractions
- Clean, minimal API

✅ **Foundation Complete**
- Window management (GLFW)
- Vulkan instance, device, and swapchain
- OpenGL context and rendering
- Vertex/Index buffers
- Shader system
- Camera system
- Example applications

## Current Status

### Completed
- ✅ Vulkan foundation (instance, device, physical device selection, surface, swapchain)
- ✅ OpenGL renderer backend
- ✅ Buffer abstractions (vertex, index)
- ✅ Shader system
- ✅ Basic 3D camera
- ✅ Colored cube example with interpolated vertex colors

### In Progress
- 🔨 Vulkan rendering pipeline
- 🔨 Command buffers
- 🔨 Render passes

### Planned
- 📋 Texture system
- 📋 Advanced lighting
- 📋 Material system
- 📋 Scene graph

## Dependencies

### Required
- **C++17 or later** compiler
- **CMake** or **Premake5** (build system)
- **Vulkan SDK** (for Vulkan support) - https://vulkan.lunarg.com/
- **GLFW 3.3.7** (included in `Dep/`)
- **GLM** (included in `Dep/`)
- **GLAD** (for OpenGL) - See `Dep/glad/README.md` for setup

### Git Submodules
- **XVUtilities** - Custom utility library

## Building

### 1. Clone Repository

```bash
git clone https://github.com/spencertan/XVRenderer.git
cd XVRenderer
git submodule update --init --recursive
```

### 2. Setup GLAD (for OpenGL support)

Follow instructions in `Dep/glad/README.md` to generate and install GLAD files for OpenGL 3.3 Core.

Alternatively, you can skip this step and use Vulkan-only mode by ensuring Vulkan SDK is installed.

### 3. Generate Project Files

Using Premake5:
```bash
premake5 vs2022    # For Visual Studio 2022
premake5 gmake2    # For GNU Make
premake5 xcode4    # For Xcode
```

### 4. Build

#### Visual Studio
Open the generated `.sln` file and build.

#### Make
```bash
cd build
make config=debug    # or config=release
```

### 5. Run Example

The colored cube example will be in `build/Debug/bin/` or `build/Release/bin/`:

```bash
./build/Debug/bin/ColoredCubeExample      # Linux/macOS
build\Debug\bin\ColoredCubeExample.exe    # Windows
```

## Project Structure

```
XVRenderer/
├── Include/XVRenderer/     # Public API headers
│   ├── Window.h            # Window management
│   ├── RenderAPI.h         # API selection
│   ├── IRenderer.h         # Renderer interface
│   ├── Vertex.h            # Vertex structures
│   ├── Camera.h            # 3D camera
│   ├── OpenGL/             # OpenGL backend
│   │   ├── GLRenderer.h
│   │   ├── GLBuffer.h
│   │   └── GLShader.h
│   └── [Vulkan headers]    # Vulkan backend
├── Source/XVRenderer/      # Implementation
├── Examples/               # Example applications
│   └── ColoredCube.cpp     # Rotating colored cube
├── Dep/                    # Dependencies
│   ├── glfw-3.3.7/        # Windowing library
│   ├── glm/               # Math library
│   ├── glad/              # OpenGL loader (setup required)
│   └── XVUtilities/       # Utility library (submodule)
├── CLAUDE.md              # AI assistant guide
├── USAGE_EXAMPLE.md       # Usage documentation
└── premake5.lua           # Build configuration
```

## Quick Start

### OpenGL Renderer Example

```cpp
#include <XVRenderer/Window.h>
#include <XVRenderer/OpenGL/GLRenderer.h>
#include <XVRenderer/OpenGL/GLBuffer.h>
#include <XVRenderer/OpenGL/GLShader.h>
#include <XVRenderer/Vertex.h>
#include <XVRenderer/Camera.h>

int main()
{
  // Create window with OpenGL
  XV::WindowConfig config{
    "My App", 1280, 720,
    XV::WindowMode::Windowed,
    true, true,
    XV::RenderAPI::OpenGL
  };
  XV::Window window(config);

  // Create renderer
  XV::GLRenderer renderer(window);

  // Create camera
  XV::Camera camera(glm::vec3(0, 0, 5));
  camera.SetAspectRatio(1280.0f / 720.0f);

  // Create cube
  XV::VertexBuffer vbo(XV::Vertex::GetCubeVertices());
  XV::IndexBuffer ebo(XV::Vertex::GetCubeIndices());
  XV::Shader shader = XV::Shader::CreateDefault();

  // Render loop
  while (!window.ShouldClose()) {
    renderer.Clear(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f));

    glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                   glm::radians(45.0f),
                                   glm::vec3(1, 1, 0));

    renderer.BindShader(shader);
    renderer.SetUniformMat4("uModel", model);
    renderer.SetUniformMat4("uView", camera.GetViewMatrix());
    renderer.SetUniformMat4("uProjection", camera.GetProjectionMatrix());

    renderer.BindVertexBuffer(vbo);
    renderer.BindIndexBuffer(ebo);
    renderer.DrawIndexed(36);

    renderer.EndFrame();
  }

  return 0;
}
```

See `Examples/ColoredCube.cpp` for a complete working example.

## Documentation

- **CLAUDE.md** - Comprehensive guide for AI assistants and developers
- **USAGE_EXAMPLE.md** - Detailed usage examples
- **Dep/glad/README.md** - GLAD setup instructions

## License

MIT License - Copyright (c) 2022 Xivi

See [LICENSE](LICENSE) for details.

## Contributing

This is a personal learning project, but feedback and suggestions are welcome!

## External Libraries

- **Vulkan** - https://vulkan.lunarg.com/
- **GLFW** - https://www.glfw.org/
- **GLM** - https://github.com/g-truc/glm
- **GLAD** - https://glad.dav1d.de/
