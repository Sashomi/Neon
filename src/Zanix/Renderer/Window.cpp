#include <Zanix/Core/String.hpp>
#include <Zanix/Renderer/Renderer.hpp>
#include <Zanix/Renderer/Window.hpp>

namespace Zx
{
	/*
	@brief : Basic constructor of Window
	*/
	Window::Window()
	{
		m_surfacePlatform = std::make_shared<Platform>();
	}

	/*
	@brief : Constructor with an VkInstance
	@param : The VkInstance to use for the creation of the Window
	*/
	Window::Window(const VkInstance& instance)
	{
		m_instance = instance;
		m_surfacePlatform = std::make_shared<Platform>();
	}

	/*
	@brief : Copy constructor
	@param : A constant reference of another Window
	*/
	Window::Window(const Window& window)
	{
		m_surfacePlatform = window.m_surfacePlatform;
	}

	/*
	@brief : Creates the presentation surface
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool Window::CreatePresentationSurface()
	{
		return (m_surfacePlatform->CreateSurface());
	}

	/*
	@brief : Creates a Windows
	@param : The width of the window
	@param:  The height of the window
	@param : The title of the window
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool Window::CreateZWindow(int width, int height, const String& title)
	{
		return (m_surfacePlatform->CreateZWindow(width, height, title));
	}
}
