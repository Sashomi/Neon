#ifndef ZWINDOW_HPP
#define ZWINDOW_HPP

#include <memory>

#include <Zanix/ZUtils.hpp>

#ifdef ZANIX_WINDOWS
	#include <Zanix/Renderer/Win32/Platform.hpp>
#endif

namespace Zx
{
	class Window
	{
	public:
		static bool CreatePresentationSurface();
		
		static bool CreateZWindow(int width, int height, String title);

		static const VkSurfaceKHR& GetSurface();

	private:
		static std::unique_ptr<Platform> s_surfacePlatform;
	};
}

#endif //ZWINDOW_HPP