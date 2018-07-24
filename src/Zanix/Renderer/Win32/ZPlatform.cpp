#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>
#include <Zanix/Renderer/ZDevice.hpp>
#include <Zanix/Renderer/ZSwapChain.hpp>
#include <Zanix/Renderer/ZCommandBuffers.hpp>
#include <Zanix/Renderer/Win32/ZPlatform.hpp>

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

	bool ZPlatform::CreateZWindow(int width, int height, const ZString& title)
	{
		s_windowInstance = GetModuleHandle(nullptr);

		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = s_windowInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = "Zanix Engine";
		wcex.hIconSm = nullptr;

		if (!RegisterClassEx(&wcex)) {
			return false;
		}

		s_windowHandle = CreateWindow("Zanix Engine", title.GetPtr(), WS_OVERLAPPEDWINDOW, 20, 20, width, height, nullptr, nullptr, s_windowInstance, nullptr);

		if (!s_windowHandle)
			return false;

		return true;
	}
	
	bool ZPlatform::CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR surfaceInfo =
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			s_windowInstance,
			s_windowHandle
		};

		if (vkCreateWin32SurfaceKHR(ZRenderer::GetVulkanInstance(), &surfaceInfo, nullptr, &s_windowSurface) == VK_SUCCESS)
			return true;

		std::cout << "Could not create presentation surface." << std::endl;

		return false;
	}
	
	const VkSurfaceKHR& ZPlatform::GetSurface() const
	{
		return s_windowSurface;
	}
}