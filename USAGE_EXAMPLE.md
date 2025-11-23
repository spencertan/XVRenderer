# XVRenderer Usage Example

This document provides a basic example of how to use the XVRenderer library to create a Vulkan-based rendering application.

## Basic Setup

```cpp
#include <XVRenderer/Window.h>
#include <XVRenderer/Renderer.h>

int main()
{
    try
    {
        // Create a window
        XV::WindowConfig window_config{
            "My Vulkan Application",  // title
            1280,                      // width
            720,                       // height
            XV::WindowMode::Windowed,  // mode
            true,                      // visible
            true                       // resizable
        };

        XV::Window window(window_config);

        // Create the renderer
        XV::RendererConfig renderer_config{
            "My Vulkan Application",  // application_name
            true                      // enable_validation (set to false for release builds)
        };

        XV::Renderer renderer(window, renderer_config);

        // Main loop
        while (!window.ShouldClose())
        {
            window.PollEvents();

            // TODO: Rendering code will go here
            //       - Acquire next swapchain image
            //       - Record command buffers
            //       - Submit to queue
            //       - Present
        }

        // Wait for all GPU operations to complete before cleanup
        renderer.WaitIdle();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## Component Overview

### Window
- Manages GLFW window creation and lifecycle
- Provides callbacks for input events
- Handles different window modes (Windowed, Fullscreen, WindowedFullscreen)

### Renderer
- High-level interface that manages all Vulkan components
- Automatically creates and configures:
  - Vulkan instance (with validation layers in debug mode)
  - Window surface
  - Physical device selection
  - Logical device
  - Swapchain

### Cross-Platform Support

The renderer is designed to work across Windows, macOS, and Linux:

- **Platform.h** provides platform detection macros
- GLFW handles platform-specific window creation
- Vulkan instance extensions are automatically detected for the current platform
- Surface creation works seamlessly across all platforms

### Debug vs Release

In debug builds (when `_DEBUG`, `DEBUG`, or `!NDEBUG` is defined):
- Validation layers are enabled by default
- Debug messenger provides detailed Vulkan debugging information

In release builds:
- Validation layers are disabled for better performance
- Debug messenger is not created

## Advanced Configuration

### Custom Physical Device Requirements

```cpp
// If you need specific GPU features
XV::PhysicalDeviceRequirements requirements;
requirements.require_discrete_gpu = true;  // Only select dedicated GPUs
requirements.require_geometry_shader = true;
requirements.required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
```

### Custom Device Features

```cpp
XV::DeviceConfig device_config;
device_config.enabled_features.geometryShader = VK_TRUE;
device_config.enabled_features.tessellationShader = VK_TRUE;
device_config.enabled_features.samplerAnisotropy = VK_TRUE;
```

### Custom Swapchain Configuration

```cpp
XV::SwapchainConfig swapchain_config;
swapchain_config.width = 1920;
swapchain_config.height = 1080;
swapchain_config.preferred_present_mode = vk::PresentModeKHR::eMailbox;  // For low latency
swapchain_config.image_count = 3;  // Triple buffering
```

## Handling Window Resize

```cpp
// Set up a framebuffer resize callback
window.SetFramebufferSizeCallback([](GLFWwindow* window, int width, int height) {
    // Signal that swapchain needs to be recreated
    // (implementation depends on your application structure)
});

// When you detect resize, recreate the swapchain
renderer.RecreateSwapchain();
```

## Next Steps

The current implementation provides the foundation for a Vulkan renderer. To extend it further, you'll need to add:

1. **Command Buffers**: For recording rendering commands
2. **Render Passes**: Define rendering operations
3. **Graphics Pipeline**: Shaders, vertex input, rasterization state
4. **Synchronization**: Semaphores and fences for frame pacing
5. **Vertex/Index Buffers**: For geometry data
6. **Descriptor Sets**: For shader resources (uniforms, textures)
7. **Depth Buffering**: For 3D rendering

All of these should follow the same RAII pattern used in the existing components.

## Error Handling

All components throw `std::runtime_error` with descriptive messages when initialization fails. Wrap initialization code in try-catch blocks to handle errors gracefully.

## Cleanup

All resources are automatically cleaned up in the correct order when objects go out of scope, thanks to RAII design:
1. Swapchain (image views, then swapchain)
2. Device (logical device)
3. Surface
4. Instance (debug messenger, then instance)
5. Window (GLFW cleanup when last window is destroyed)

Always call `renderer.WaitIdle()` before allowing the renderer to be destroyed to ensure all GPU operations have completed.
