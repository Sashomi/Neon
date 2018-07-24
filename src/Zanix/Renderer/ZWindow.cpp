#include <Zanix/Renderer/ZWindow.hpp>

namespace Zx
{
	/*
	@brief : Creates the presentation surface
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool ZWindow::CreatePresentationSurface()
	{
		return (s_surfacePlatform->CreateSurface());
	}

	/*
	@brief : Creates a Windows
	@param : The width of the window
	@param:  The height of the window
	@param : The title of the window
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool ZWindow::CreateZWindow(int width, int height, ZString title)
	{
		return (s_surfacePlatform->CreateZWindow(width, height, title));
	}

	/*
	@brief : Gets the surface of the window
	*/
	const VkSurfaceKHR& ZWindow::GetSurface()
	{
		return s_surfacePlatform->GetSurface();
	}

	//-------------------------------------------------------------------------

	std::unique_ptr<ZPlatform> ZWindow::s_surfacePlatform = std::make_unique<ZPlatform>();
}
