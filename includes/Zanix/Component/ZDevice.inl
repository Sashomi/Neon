#include <Zanix/Renderer/ZVulkan.hpp>

namespace Zx
{
	bool ZDevice::Queue::IsValidQueue()
	{
		//TODO : Opti
		return (indexFamily >= 0);
	}

	ZDevice::SwapChainDetails ZDevice::BuildSwapChainDetails(VkPhysicalDevice device)
	{
		SwapChainDetails details;
		
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ZVulkan::GetWindowSurface(), &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, ZVulkan::GetWindowSurface(), &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.format.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, ZVulkan::GetWindowSurface(), &formatCount, details.format.data());
		}

		uint32_t presentCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, ZVulkan::GetWindowSurface(), &presentCount, nullptr);

		if (presentCount != 0)
		{
			details.presentmode.resize(presentCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, ZVulkan::GetWindowSurface(), &presentCount, details.presentmode.data());
		}

		return details;
	}
}