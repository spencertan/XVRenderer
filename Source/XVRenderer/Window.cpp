#include "Window.h"

namespace XV
{

static i32 s_glfw_windows_count = 0;
static bool s_glfw_initialised = false;

Window::Window( const WindowConfig &config ) :
  m_width( config.width ),
  m_height( config.height ),
  m_mode( config.mode )
{
  if ( s_glfw_initialised )
  {
    if ( glfwInit() != GLFW_TRUE )
      throw std::runtime_error( "[Window] Failed to initialise GLFW!" );

    s_glfw_initialised = true;
  }

  glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
  glfwWindowHint( GLFW_VISIBLE, config.visible ? GLFW_TRUE : GLFW_FALSE );
  glfwWindowHint( GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE );
  glfwWindowHint( GLFW_AUTO_ICONIFY, GLFW_FALSE );

  Ptr<GLFWmonitor> monitor { nullptr };
  if ( config.mode != WindowMode::Windowed )
  {
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode( monitor );
    m_width = mode->width;
    m_height = mode->height;
  }

  m_window = glfwCreateWindow( static_cast<i32>( m_width ), static_cast<i32>( m_height ), config.title.c_str(), monitor, nullptr );

  if ( !m_window )
    throw std::runtime_error( "[Window] Failed to create window" + config.title + "!" );
  else
    ++s_glfw_windows_count;
}

Window::~Window()
{
  glfwDestroyWindow( m_window );
  if ( --s_glfw_windows_count < 1 )
  {
    s_glfw_initialised = false;
    glfwTerminate();
  }
}

void Window::SetTitle( const std::string &title )
{
  glfwSetWindowTitle( m_window, title.c_str() );
}

void Window::SetUserPtr( Ptr<void> user_ptr )
{
  glfwSetWindowUserPointer( m_window, user_ptr );
}

void Window::ShowWindow()
{
  glfwShowWindow( m_window );
}

void Window::PollEvents()
{
  glfwPollEvents();
}

bool Window::ShouldClose()
{
  return glfwWindowShouldClose( m_window );
}

// Shared callback
void Window::SetErrorCallback( GLFWerrorfun callback )
{
  glfwSetErrorCallback( callback );
}

void Window::SetJoystickCallback( GLFWjoystickfun callback )
{
  glfwSetJoystickCallback( callback );
}

// Per window callback
void Window::SetWindowCloseCallback( GLFWwindowclosefun callback )
{
  glfwSetWindowCloseCallback( m_window, callback );
}

void Window::SetWindowSizeCallback( GLFWwindowsizefun callback )
{
  glfwSetWindowSizeCallback( m_window, callback );
}

void Window::SetFramebufferSizeCallback( GLFWframebuffersizefun callback )
{
  glfwSetFramebufferSizeCallback( m_window, callback );
}

void Window::SetWindowContentScaleCallback( GLFWwindowcontentscalefun callback )
{
  glfwSetWindowContentScaleCallback( m_window, callback );
}

void Window::SetWindowPosCallback( GLFWwindowposfun callback )
{
  glfwSetWindowPosCallback( m_window, callback );
}

void Window::SetWindowIconifyCallback( GLFWwindowiconifyfun callback )
{
  glfwSetWindowIconifyCallback( m_window, callback );
}

void Window::SetWindowMaximizeCallback( GLFWwindowmaximizefun callback )
{
  glfwSetWindowMaximizeCallback( m_window, callback );
}

void Window::SetWindowFocusCallback( GLFWwindowfocusfun callback )
{
  glfwSetWindowFocusCallback( m_window, callback );
}

void Window::SetWindowRefreshCallback( GLFWwindowrefreshfun callback )
{
  glfwSetWindowRefreshCallback( m_window, callback );
}

void Window::SetKeyCallback( GLFWkeyfun callback )
{
  glfwSetKeyCallback( m_window, callback );
}

void Window::SetMouseButtonCallback( GLFWmousebuttonfun callback )
{
  glfwSetMouseButtonCallback( m_window, callback );
}

void Window::SetScrollCallback( GLFWscrollfun callback )
{
  glfwSetScrollCallback( m_window, callback );
}

void Window::SetCursorPosCallback( GLFWcursorposfun callback )
{
  glfwSetCursorPosCallback( m_window, callback );
}

void Window::SetCursorEnterCallback( GLFWcursorenterfun callback )
{
  glfwSetCursorEnterCallback( m_window, callback );
}

void Window::SetDropCallback( GLFWdropfun callback )
{
  glfwSetDropCallback( m_window, callback );
}

}