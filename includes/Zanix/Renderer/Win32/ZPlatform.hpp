#ifndef ZPLATFORM_HPP
#define ZPLATFORM_HPP

#include <Windows.h>
#include <vulkan/vulkan.h>

namespace Zx
{
	class ZPlatform
	{
	public:
		static void CreateSurface();
		static VkSurfaceKHR GetSurface();

	private:
		static HINSTANCE s_windowInstance;
		static HWND s_windowHandle;
		static VkSurfaceKHR s_windowSurface;
	};
}

#endif //ZPLATFORM_HPP