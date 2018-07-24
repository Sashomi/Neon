#ifndef ZWINDOW_HPP
#define ZWINDOW_HPP

#include <memory>

#include <Zanix/ZUtils.hpp>

#ifdef ZANIX_WINDOWS
	#include <Zanix/Renderer/Win32/ZPlatform.hpp>
#endif

namespace Zx
{
	class ZWindow
	{
	public:
		static bool CreatePresentationSurface();
		
		static bool CreateZWindow(int width, int height, ZString title);

		static const VkSurfaceKHR& GetSurface();

	private:
		static std::unique_ptr<ZPlatform> s_surfacePlatform;
	};
}

#endif //ZWINDOW_HPP