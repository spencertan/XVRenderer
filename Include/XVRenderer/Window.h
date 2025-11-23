#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "RenderAPI.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <vector>

namespace XV
{

enum class WindowMode : u8
{
  Fullscreen,
  Windowed,
  WindowedFullscreen
};

struct WindowConfig
{
  std::string title;
  u32 width;
  u32 height;
  WindowMode mode;
  bool visible;
  bool resizable;
  RenderAPI api = RenderAPI::Vulkan;  // Which rendering API to use
};


// Wrapper over GLFW Windows
class Window : NonCopyable
{

Ptr<GLFWwindow> m_window{nullptr};
u32 m_width;
u32 m_height;
WindowMode m_mode;

public:

  Window( const WindowConfig &config = {} );
  ~Window();

  void SetTitle(const std::string& title);
  void SetUserPtr(Ptr<void> user_ptr);
  void ShowWindow();
  void PollEvents();
  bool ShouldClose();

  // Accessors
  Ptr<GLFWwindow> GetGLFWWindow() const { return m_window; }
  u32 GetWidth() const { return m_width; }
  u32 GetHeight() const { return m_height; }
  WindowMode GetMode() const { return m_mode; }

  // Vulkan support
  static std::vector<const char*> GetRequiredInstanceExtensions();
  
  // Callback shared among all windows
  static void SetErrorCallback(GLFWerrorfun callback);
  static void SetJoystickCallback( GLFWjoystickfun callback);

  // Per window callback
  void SetWindowCloseCallback(GLFWwindowclosefun callback);
  void SetWindowSizeCallback(GLFWwindowsizefun callback);
  void SetFramebufferSizeCallback(GLFWframebuffersizefun callback);
  void SetWindowContentScaleCallback(GLFWwindowcontentscalefun callback);
  void SetWindowPosCallback(GLFWwindowposfun callback);
  void SetWindowIconifyCallback(GLFWwindowiconifyfun callback);
  void SetWindowMaximizeCallback(GLFWwindowmaximizefun callback);
  void SetWindowFocusCallback(GLFWwindowfocusfun callback);
  void SetWindowRefreshCallback(GLFWwindowrefreshfun callback);
  void SetKeyCallback(GLFWkeyfun callback);
  void SetMouseButtonCallback(GLFWmousebuttonfun callback);
  void SetScrollCallback(GLFWscrollfun callback);
  void SetCursorPosCallback(GLFWcursorposfun callback);
  void SetCursorEnterCallback(GLFWcursorenterfun callback);
  void SetDropCallback(GLFWdropfun callback);

};


}