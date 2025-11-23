# CLAUDE.md - XVRenderer Codebase Guide

## Project Overview

**XVRenderer** is a hobby graphics rendering library built on the Vulkan API. It's part of a larger game engine project and is currently in early development, focusing on wrapping Vulkan with RAII principles and setting up the rendering pipeline.

- **Project Type**: Static Library
- **Language**: C++ (C++latest standard)
- **Build System**: Premake5
- **License**: MIT (Copyright 2022 Xivi)
- **Platform Target**: Windows (currently uses VC2022 libraries)

## Repository Structure

```
XVRenderer/
├── Include/XVRenderer/    # Public header files
│   ├── Renderer.h         # Main renderer interface (currently empty, under development)
│   └── Window.h           # Window management wrapper over GLFW
├── Source/XVRenderer/     # Implementation files
│   └── Window.cpp         # Window implementation
├── Dep/                   # External dependencies
│   ├── glfw-3.3.7/       # GLFW windowing library (precompiled)
│   ├── glm/              # OpenGL Mathematics library
│   └── XVUtilities/      # Git submodule - custom utilities library
├── premake5.lua          # Build configuration
├── .gitignore
├── .gitmodules
├── LICENSE
└── README.md
```

## Technology Stack

### Core Technologies
- **Vulkan SDK**: Primary graphics API (requires VULKAN_SDK environment variable)
- **GLFW 3.3.7**: Cross-platform windowing and input
- **GLM**: Mathematics library for graphics
- **C++latest**: Modern C++ features enabled

### Dependencies
1. **XVUtilities** (Git Submodule)
   - URL: https://github.com/spencertan/XVUtilities.git
   - Provides: Types (u8, u32, i32, Ptr), Helper classes (NonCopyable)

2. **Vulkan SDK** (System Dependency)
   - Must be installed on system
   - Requires VULKAN_SDK environment variable

3. **GLFW** (Vendored)
   - Precompiled libraries included
   - Using VC2022 static multithreaded libraries (glfw3_mt.lib)

4. **GLM** (Vendored)
   - Header-only library included

## Build System

### Premake5 Configuration

The project uses Premake5 as its build system generator. Key configuration details:

- **Project Kind**: StaticLib
- **C++ Dialect**: C++latest
- **Edit and Continue**: Disabled
- **File Pattern**: Recursively includes all `.h`, `.cpp`, `.inl` files from Source/ and Include/

### Include Paths
- **Public Headers**: `Include/XVRenderer`
- **System Headers**:
  - `Dep/XVUtilities/Include`
  - `Dep/glfw-3.3.7/include`
  - `Dep/glm/glm`
  - `$(VULKAN_SDK)/include`

### Library Links
- `$(VULKAN_SDK)/lib/vulkan-1.lib`
- `Dep/glfw-3.3.7/lib-vc2022/glfw3_mt.lib`

### Build Artifacts
The `.gitignore` excludes:
- `compile_commands.json`
- `.ccls-cache/`
- `build/`

## Code Organization

### Namespace Convention
All code is wrapped in the `XV` namespace.

### File Organization
- **Headers**: `Include/XVRenderer/[ClassName].h`
- **Implementation**: `Source/XVRenderer/[ClassName].cpp`
- **Inline Implementation**: `.inl` files supported (if needed)

### Coding Conventions

1. **Type Aliases** (from XVUtilities):
   - `u8`, `u32` - Unsigned integers
   - `i32` - Signed integers
   - `Ptr<T>` - Pointer type wrapper

2. **Class Design**:
   - Use `NonCopyable` base class for non-copyable resources
   - RAII principles for resource management
   - Member variables prefixed with `m_` (e.g., `m_window`, `m_width`)
   - Static variables prefixed with `s_` (e.g., `s_glfw_windows_count`)

3. **Include Order**:
   ```cpp
   // System/External libraries first
   #define GLFW_INCLUDE_VULKAN
   #include <glfw/glfw3.h>
   #include <vulkan/vulkan.hpp>

   // XVUtilities includes
   #include <XVUtilities/Types.h>
   #include <XVUtilities/Helper/NonCopyable.h>

   // Local includes
   #include "LocalHeader.h"
   ```

4. **Configuration Structs**:
   - Use config structs for complex initialization (e.g., `WindowConfig`)
   - Provide default values where sensible

## Current Components

### Window System (Completed)

**Location**: `Include/XVRenderer/Window.h`, `Source/XVRenderer/Window.cpp`

The `Window` class is a RAII wrapper around GLFW windows with the following features:

- **Lifecycle Management**:
  - Automatic GLFW initialization on first window creation
  - Reference counting for GLFW cleanup (when last window is destroyed)
  - Throws `std::runtime_error` on initialization/creation failures

- **Window Modes**:
  - `Fullscreen`: Exclusive fullscreen
  - `Windowed`: Regular windowed mode
  - `WindowedFullscreen`: Borderless window matching monitor resolution

- **Configuration Options** (`WindowConfig`):
  - `title`: Window title
  - `width`, `height`: Window dimensions (overridden in fullscreen modes)
  - `mode`: WindowMode enum
  - `visible`: Initial visibility
  - `resizable`: Whether window can be resized

- **Callback Support**:
  - Static callbacks (shared): Error, Joystick
  - Per-window callbacks: Close, Size, Framebuffer, ContentScale, Position, Iconify, Maximize, Focus, Refresh, Key, MouseButton, Scroll, CursorPos, CursorEnter, Drop

**Important Notes**:
- Window is configured with `GLFW_CLIENT_API = GLFW_NO_API` for Vulkan
- Auto-iconify is disabled (`GLFW_AUTO_ICONIFY = GLFW_FALSE`)
- Reference counting ensures GLFW cleanup only when all windows are destroyed

### Renderer System (Under Development)

**Location**: `Include/XVRenderer/Renderer.h`

Currently empty - this is the primary area of active development. Future implementation will:
- Wrap Vulkan API with RAII principles
- Set up the rendering pipeline
- Manage Vulkan resources (devices, queues, command buffers, etc.)

## Development Workflows

### Initial Setup

1. **Clone Repository**:
   ```bash
   git clone <repository-url>
   cd XVRenderer
   git submodule update --init --recursive
   ```

2. **Install Vulkan SDK**:
   - Download and install from https://vulkan.lunarg.com/
   - Ensure `VULKAN_SDK` environment variable is set

3. **Generate Build Files**:
   ```bash
   premake5 vs2022  # or vs2019, gmake2, etc.
   ```

### Adding New Components

1. **Create Header**:
   - Location: `Include/XVRenderer/[ComponentName].h`
   - Wrap in `XV` namespace
   - Include necessary dependencies from XVUtilities
   - Use forward declarations where possible

2. **Create Implementation**:
   - Location: `Source/XVRenderer/[ComponentName].cpp`
   - Include the header first: `#include "[ComponentName].h"`

3. **Update Premake** (if needed):
   - Usually not required (auto-includes `**/*.h` and `**/*.cpp`)
   - Only modify if adding new dependencies or link requirements

### Working with Submodules

The project depends on `XVUtilities` as a git submodule:

```bash
# Update submodule to latest
cd Dep/XVUtilities
git pull origin main
cd ../..
git add Dep/XVUtilities
git commit -m "Update submodule"

# If XVUtilities is missing
git submodule update --init --recursive
```

### Modifying Build Configuration

When modifying `premake5.lua`:
1. Edit the file
2. Regenerate build files: `premake5 vs2022`
3. Reload solution/project in IDE

## Common Pitfalls and Best Practices

### Pitfalls to Avoid

1. **GLFW Initialization**:
   - Don't call `glfwInit()` directly - the Window class manages this
   - Bug exists in Window.cpp:14 - condition is inverted (should be `!s_glfw_initialised`)

2. **Memory Management**:
   - Use RAII principles consistently
   - Don't manually manage Vulkan resources - wrap them in classes with proper destructors

3. **Include Paths**:
   - Use `<>` for system/external includes
   - Use `""` for local project includes
   - Don't use absolute paths

4. **Platform Assumptions**:
   - Current code assumes Windows (VC2022 libraries)
   - Be mindful when adding cross-platform features

### Best Practices

1. **Error Handling**:
   - Use exceptions for initialization failures (as Window does)
   - Consider error codes for runtime errors in Vulkan wrapper

2. **Configuration**:
   - Use config structs for complex initialization
   - Provide sensible defaults

3. **Resource Management**:
   - Follow RAII principles strictly
   - Use `NonCopyable` for resource-owning classes
   - Implement move semantics where appropriate

4. **Vulkan Integration**:
   - Always check Vulkan result codes
   - Use vulkan.hpp (C++ bindings) for RAII support
   - Consider validation layers during development

## AI Assistant Guidelines

### When Adding Features

1. **Read Existing Code First**: Always read related files before making changes
2. **Follow Existing Patterns**: Match the coding style and patterns in Window.cpp
3. **Use XVUtilities Types**: Prefer `u32`, `i32`, `Ptr<T>` over raw types
4. **Namespace Consistency**: All code in `XV` namespace
5. **RAII Everything**: Resource-owning classes should follow RAII
6. **Config Structs**: Use configuration structs for complex initialization

### When Fixing Bugs

1. **Check Window.cpp:14**: Known bug - GLFW initialization logic is inverted
2. **Verify Error Handling**: Ensure proper exception throwing/handling
3. **Test Resource Cleanup**: Verify destructors are called in proper order

### When Refactoring

1. **Maintain API Compatibility**: Public headers in Include/ define the API
2. **Keep Implementation Private**: Implementation details in Source/
3. **Update Premake if Needed**: Only if adding new external dependencies

### When Implementing Vulkan Features

1. **Use vulkan.hpp**: Prefer C++ bindings over C API
2. **Wrap in Classes**: Each Vulkan resource should have a RAII wrapper
3. **Validation Layers**: Enable for development, make optional for release
4. **Follow Vulkan Best Practices**:
   - Minimize state changes
   - Batch operations
   - Use appropriate queue families

## Project Status and Roadmap

### Completed
- [x] Basic project structure
- [x] Build system setup (Premake5)
- [x] GLFW window wrapper with RAII
- [x] Window callback support
- [x] Basic error handling

### In Progress
- [ ] Vulkan RAII wrappers
- [ ] Rendering pipeline setup
- [ ] Renderer interface design

### Future Considerations
- [ ] Cross-platform support (Linux, macOS)
- [ ] Multiple rendering backends
- [ ] Shader compilation pipeline
- [ ] Resource management system
- [ ] Scene graph integration

## Questions to Ask Before Making Changes

1. Does this change require new external dependencies?
2. Does this affect the public API (headers in Include/)?
3. Is this consistent with RAII principles?
4. Does this work with the existing Window implementation?
5. Will this require Vulkan SDK features not yet initialized?
6. Is this cross-platform or Windows-specific?

## Getting Help

- **Repository**: Part of a larger engine project
- **Dependencies**: Check XVUtilities repository for utility documentation
- **Vulkan**: Refer to Vulkan SDK documentation and vulkan.hpp reference

## Notes for AI Assistants

- This is an early-stage project - many systems are still undefined
- The Renderer.h is intentionally empty - it's the next major development target
- Code quality over speed - this is a learning project
- RAII and modern C++ practices are priorities
- Always verify Vulkan SDK is available before suggesting Vulkan-dependent code
- Be conservative with dependencies - keep the project lightweight
