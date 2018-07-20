#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>
#include <Zanix/Renderer/Win32/ZPlatform.hpp>

#include <vulkan/vulkan_win32.h>

namespace Zx
{
	void ZPlatform::CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR surfaceInfo =
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			s_windowInstance,
			s_windowHandle
		};

		if (vkCreateWin32SurfaceKHR(ZRenderer::GetVulkanInstance(), &surfaceInfo, nullptr, &s_windowSurface) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a win32 surface.");
	}

	VkSurfaceKHR ZPlatform::GetSurface()
	{
		return s_windowSurface;
	}

	HINSTANCE ZPlatform::s_windowInstance = VK_NULL_HANDLE;
	HWND ZPlatform::s_windowHandle = VK_NULL_HANDLE;
	VkSurfaceKHR ZPlatform::s_windowSurface = VK_NULL_HANDLE;
}