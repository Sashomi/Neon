#ifndef ZPLATFORM_HPP
#define ZPLATFORM_HPP

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <Zanix/Core/String.hpp>

namespace Zx
{
	class Platform
	{
	public:
		bool CreateSurface();
		const VkSurfaceKHR& GetSurface() const;

		bool CreateZWindow(int width, int height, const String& title);

	private:
		HINSTANCE s_windowInstance;
		HWND s_windowHandle;
		VkSurfaceKHR s_windowSurface;
	};
}

#endif //ZPLATFORM_HPP