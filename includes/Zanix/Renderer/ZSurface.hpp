#ifndef ZSURFACE_HPP
#define ZSURFACE_HPP

#include <memory>

#include <Zanix/ZUtils.hpp>

#ifdef ZANIX_WINDOWS
	#include <Zanix/Renderer/Win32/ZPlatform.hpp>
#endif

namespace Zx
{
	class ZSurface
	{
	public:
		static ZPlatform GetSurfacePlatform();
	private:
		static std::unique_ptr<ZPlatform> s_surfacePlatform;
	};
}

#endif //ZSURFACE_HPP