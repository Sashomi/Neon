#include <vector>

#include <Zanix/Renderer/ZDevice.hpp>
#include <Zanix/Renderer/ZSurface.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZSwapChain.hpp>

namespace Zx
{
	//-------------------------Private method-------------------------

	bool ZSwapChain::CreateSwapChain()
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;

		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ZDevice::GetPhysicalDevice(), ZSurface::GetSurfacePlatform().GetSurface(), &surfaceCapabilities) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get a physical device surface capabilities");

		//Formats

		uint32_t formatCount = 0;
		if((vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::GetPhysicalDevice(), ZSurface::GetSurfacePlatform().GetSurface(), &formatCount, nullptr)
			!= VK_SUCCESS) ||(formatCount == 0))
			throw ZOperationFailed(__FILE__, "Failed to get the format count available");

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		if(vkGetPhysicalDeviceSurfaceFormatsKHR(ZDevice::GetPhysicalDevice(), ZSurface::GetSurfacePlatform().GetSurface(), &formatCount, surfaceFormats.data())
			!= VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get the format count available - data");

		//Modes

		uint32_t modesCount = 0;
		if ((vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::GetPhysicalDevice(), ZSurface::GetSurfacePlatform().GetSurface(), &modesCount, nullptr)
			!= VK_SUCCESS) || (modesCount == 0))
			throw ZOperationFailed(__FILE__, "Failed to get the present modes count available");

		std::vector<VkPresentModeKHR> presentModes(modesCount);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(ZDevice::GetPhysicalDevice(), ZSurface::GetSurfacePlatform().GetSurface(), &modesCount, presentModes.data())
			!= VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get the present modes count available - data");

		//Création de la swap chain

		uint32_t desiredNumImages = GetSwapChainNumImages(surfaceCapabilities);
		VkSurfaceFormatKHR desiredSurfaceFormat = GetSwapChainFormat(surfaceFormats);
		VkExtent2D desiredExtent = GetSwapChainExtent(surfaceCapabilities);
		VkImageUsageFlags desiredUsageFlags = GetSwapChainUsageFlags(surfaceCapabilities);
		VkSurfaceTransformFlagBitsKHR desiredSurfaceTransform = GetSwapChainTransform(surfaceCapabilities);
		VkPresentModeKHR desiredPresentMode = GetSwapChainPresentMode(presentModes);
		VkSwapchainKHR oldSwapChain = s_swapChain;

		if ((static_cast<int>(desiredUsageFlags) == -1) || (static_cast<int>(desiredPresentMode) == -1))
			return false;

		VkSwapchainCreateInfoKHR swapChainInfo =
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,
			ZSurface::GetSurfacePlatform().GetSurface(),
			desiredNumImages,
			desiredSurfaceFormat.format,
			desiredSurfaceFormat.colorSpace,
			desiredExtent,
			1,
			desiredUsageFlags,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr,
			desiredSurfaceTransform,
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			desiredPresentMode,
			VK_TRUE,
			oldSwapChain
		};

		if (vkCreateSwapchainKHR(ZDevice::GetLogicalDevice(), &swapChainInfo, nullptr, &s_swapChain) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create swap chain");

		if (oldSwapChain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(ZDevice::GetLogicalDevice(), oldSwapChain, nullptr);

		return true;
	}

	uint32_t ZSwapChain::GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
		if ((surfaceCapabilities.maxImageCount > 0) && (imageCount > surfaceCapabilities.maxImageCount))
			imageCount = surfaceCapabilities.maxImageCount;

		return imageCount;
	}

	VkSurfaceFormatKHR ZSwapChain::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats)
	{
		if ((surfaceFormats.size() == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
			return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };

		for (const auto& surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM)
				return surfaceFormat;
		}

		return surfaceFormats[0];
	}

	VkExtent2D ZSwapChain::GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.currentExtent.width == -1)
		{
			VkExtent2D swapChainExtent = { 640, 480 };
			
			if (swapChainExtent.width < surfaceCapabilities.minImageExtent.width)
				swapChainExtent.width = surfaceCapabilities.minImageExtent.width;
			if (swapChainExtent.height < surfaceCapabilities.minImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.minImageExtent.height;

			if (swapChainExtent.height > surfaceCapabilities.maxImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.maxImageExtent.height;
			if (swapChainExtent.height > surfaceCapabilities.maxImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.maxImageExtent.height;

			return swapChainExtent;
		}

		return surfaceCapabilities.currentExtent;
	}

	VkImageUsageFlags ZSwapChain::GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		return static_cast<VkImageUsageFlags>(-1);
	}

	VkSurfaceTransformFlagBitsKHR ZSwapChain::GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		
		return surfaceCapabilities.currentTransform;
	}

	VkPresentModeKHR ZSwapChain::GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
	{
		for (const auto& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}

		for (const auto& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_FIFO_KHR)
				return presentMode;
		}

		return static_cast<VkPresentModeKHR>(-1);
	}

	VkSwapchainKHR ZSwapChain::s_swapChain = VK_NULL_HANDLE;
}