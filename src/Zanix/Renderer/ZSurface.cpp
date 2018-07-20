#include <Zanix/Renderer/ZSurface.hpp>

namespace Zx
{
	ZPlatform ZSurface::GetSurfacePlatform()
	{
		return *s_surfacePlatform;
	}
}
