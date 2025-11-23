#pragma once

// Platform detection macros for cross-platform Vulkan support

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
  #define XV_PLATFORM_WINDOWS
  #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__APPLE__)
  #define XV_PLATFORM_MACOS
  #include <TargetConditionals.h>
  #if TARGET_OS_MAC
    #define VK_USE_PLATFORM_METAL_EXT
  #endif
#elif defined(__linux__)
  #define XV_PLATFORM_LINUX
  // Most Linux systems use X11 or Wayland
  // GLFW will handle this, but we need to enable both
  #define VK_USE_PLATFORM_XLIB_KHR
  #define VK_USE_PLATFORM_WAYLAND_KHR
#else
  #error "Unsupported platform!"
#endif

// Debug/Release detection
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
  #define XV_DEBUG
#else
  #define XV_RELEASE
#endif
