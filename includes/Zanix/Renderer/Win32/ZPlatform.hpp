#ifndef ZPLATFORM_HPP
#define ZPLATFORM_HPP

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <Zanix/Core/ZString.hpp>

namespace Zx
{
	class ZPlatform
	{
	public:
		bool CreateSurface();
		const VkSurfaceKHR& GetSurface() const;

		bool CreateZWindow(int width, int height, const ZString& title);
		bool RenderingLoop() const;

	private:
		HINSTANCE s_windowInstance;
		HWND s_windowHandle;
		VkSurfaceKHR s_windowSurface;
	};
}

#endif //ZPLATFORM_HPP