#include <Neon/Core/String.hpp>
#include <Neon/Core/Exception.hpp>
#include <Neon/Renderer/Renderer.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/CommandBuffers.hpp>
#include <Neon/Renderer/Win32/Platform.hpp>

#include <iostream>
#include <vulkan/vulkan_win32.h>

namespace Zx
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_SIZE:
		case WM_EXITSIZEMOVE:
			PostMessage(hWnd, WM_USER + 1, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_CLOSE:
			PostMessage(hWnd, WM_USER + 2, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	//---------------------------------------------------------------------

	/*
	@brief : Constructors of the Window OS
	@param : The VkInstance of the application
	*/
	Platform::Platform(const VkInstance& instance) : m_windowHandle(nullptr), m_windowInstance(nullptr), m_windowSurface(VK_NULL_HANDLE)
	{
		m_instance = instance;
	}

	/*
	@brief : Creates a window for Window OS
	@return : Returns true if the creation of the window is a success, false otherwise
	*/
	bool Platform::CreateZWindow(int width, int height, const String& title)
	{
		m_windowInstance = GetModuleHandle(nullptr);

		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_windowInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = "Neon Engine";
		wcex.hIconSm = nullptr;

		if (!RegisterClassEx(&wcex)) {
			return false;
		}

		m_windowHandle = CreateWindow("Neon Engine", title.GetPtr(), WS_OVERLAPPEDWINDOW, 20, 20, width, height, nullptr, nullptr, m_windowInstance, nullptr);

		if (!m_windowHandle)
			return false;

		return true;
	}
	
	/*
	@brief : Create a window's surface
	@return : Returns true if the creation of the window surface is a success, false otherwise
	*/
	bool Platform::CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR surfaceInfo =
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_windowInstance,
			m_windowHandle
		};

		if (vkCreateWin32SurfaceKHR(m_instance, &surfaceInfo, nullptr, &m_windowSurface) == VK_SUCCESS)
			return true;

		std::cout << "Could not create presentation surface." << std::endl;

		return false;
	}
}