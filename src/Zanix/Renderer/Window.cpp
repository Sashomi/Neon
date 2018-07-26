#include <Zanix/Renderer/Window.hpp>

namespace Zx
{
	/*
	@brief : Creates the presentation surface
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool Window::CreatePresentationSurface()
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
	bool Window::CreateZWindow(int width, int height, String title)
	{
		return (s_surfacePlatform->CreateZWindow(width, height, title));
	}

	/*
	@brief : Gets the surface of the window
	*/
	const VkSurfaceKHR& Window::GetSurface()
	{
		return s_surfacePlatform->GetSurface();
	}

	//-------------------------------------------------------------------------

	std::unique_ptr<Platform> Window::s_surfacePlatform = std::make_unique<Platform>();
}
