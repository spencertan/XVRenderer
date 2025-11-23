#pragma once

#include <XVUtilities/Types.h>

namespace XV
{

enum class RenderAPI : u8
{
  None = 0,
  Vulkan,
  OpenGL
};

// Runtime detection and selection
class RenderAPIInfo
{
public:
  static RenderAPI DetectBestAPI();
  static bool IsAPISupported(RenderAPI api);
  static const char* GetAPIName(RenderAPI api);
};

}
