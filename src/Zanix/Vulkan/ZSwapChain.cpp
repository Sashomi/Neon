#include <Zanix/Vulkan/ZDevice.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Vulkan/ZSwapChain.hpp>

#include <algorithm>

namespace Zx
{
	/*
	@brief : Initialize the swap chain
	*/
	void ZSwapChain::InitializeSwapChain()
	{
		CreateSwapChain();
		CreateImageView();
	}

	/*
	@brief : unitilialize the swap chain
	*/
	void ZSwapChain::UnInitializeSwapChain()
	{
		for (auto imageView : s_swapChainImageView)
			vkDestroyImageView(ZDevice::GetLogicalDevice(), imageView, nullptr);

		vkDestroySwapchainKHR(ZDevice::GetLogicalDevice(), s_swapChain, nullptr);
	}

	/*
	@brief : Returns the swap chain
	*/
	VkSwapchainKHR& ZSwapChain::GetSwapChain()
	{
		return s_swapChain;
	}

	//-------------------------Private methods-------------------------

	void ZSwapChain::CreateSwapChain()
	{
		SwapChainDetails swapChain = BuildSwapChainDetails(ZDevice::GetPhysicalDevice());

		VkSurfaceFormatKHR surfaceFormat = GetSwapSurfaceFormat(swapChain.format);
		VkPresentModeKHR presentMode = GetSwapPresentMode(swapChain.presentmode);
		VkExtent2D extent = GetSwapExtent(swapChain.capabilities);

		uint32_t imageCount = swapChain.capabilities.minImageCount + 1;

		if (swapChain.capabilities.maxImageCount > 0 && imageCount > swapChain.capabilities.maxImageCount)
			imageCount = swapChain.capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = ZVulkan::GetWindowSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		ZDevice::Queue index = ZDevice::GetQueueFamiliy(ZDevice::GetPhysicalDevice());
		uint32_t familyIndex[] = { (uint32_t)index.indexFamily, (uint32_t)index.presentFamily };

		if (index.indexFamily != index.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = familyIndex;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //Meilleur performance
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChain.capabilities.currentTransform; //Pas de transformation
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE; // On ignore les pixels cachés
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(ZDevice::GetLogicalDevice(), &createInfo, nullptr, &s_swapChain) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create swap chain");

		vkGetSwapchainImagesKHR(ZDevice::GetLogicalDevice(), s_swapChain, &imageCount, nullptr);
		s_swapChainImage.resize(imageCount);
		vkGetSwapchainImagesKHR(ZDevice::GetLogicalDevice(), s_swapChain, &imageCount, s_swapChainImage.data());

		s_swapChainExtent = extent;
		s_swapChainImageFormat = surfaceFormat.format;
	}

	//----------------------------------------------------------------

	void ZSwapChain::CreateImageView()
	{
		s_swapChainImageView.resize(s_swapChainImage.size());

		for (int i = 0; i < s_swapChainImageView.size(); i++)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = s_swapChainImage[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = s_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(ZDevice::GetLogicalDevice(), &createInfo, nullptr, &s_swapChainImageView[i]) != VK_SUCCESS)
				throw ZOperationFailed(__FILE__, "Failed to create image view");
		}
	}


	//----------------------------------------------------------------

	VkExtent2D ZSwapChain::GetSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilitities)
	{
		if (surfaceCapabilitities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return surfaceCapabilitities.currentExtent;

		uint32_t width = 0, height = 0;
		ZWindow window = ZVulkan::GetZWindow();
		window.GetWindowSize(&width, &height);

		VkExtent2D actualExtent = { width, height };

		actualExtent.width = std::max(surfaceCapabilitities.minImageExtent.width, std::min(surfaceCapabilitities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(surfaceCapabilitities.minImageExtent.height, std::min(surfaceCapabilitities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}

	//----------------------------------------------------------------

	VkSurfaceFormatKHR ZSwapChain::GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormat)
	{
		// Si la surface n'a aucun format favoris, dans ce cas la taille de "surfaceFormat" vaut 1
		if (surfaceFormat.size() == 1 && surfaceFormat[0].format == VK_FORMAT_UNDEFINED)
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		// On cherche la meilleure possibilitée
		for (const auto& format : surfaceFormat)
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;

		return surfaceFormat[0];
	}

	//----------------------------------------------------------------


	VkPresentModeKHR ZSwapChain::GetSwapPresentMode(const std::vector<VkPresentModeKHR>& presentMode)
	{
		VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& presentMde : presentMode)
		{
			//Triple mise en mémoire
			if (presentMde == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMde;
			else if (presentMde == VK_PRESENT_MODE_IMMEDIATE_KHR)
				mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}

		return mode;
	}

	//----------------------------------------------------------------

	std::vector<VkImage> ZSwapChain::BuildVectorVkImage()
	{
		std::vector<VkImage> vec;

		return vec;
	}

	//----------------------------------------------------------------

	std::vector<VkImageView> ZSwapChain::BuildVectorVkImageView()
	{
		std::vector<VkImageView> vec;

		return vec;
	}


	VkSwapchainKHR ZSwapChain::s_swapChain = VK_NULL_HANDLE;
	VkFormat ZSwapChain::s_swapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D ZSwapChain::s_swapChainExtent = { 0, 0 };

	std::vector<VkImage> ZSwapChain::s_swapChainImage = BuildVectorVkImage();
	std::vector<VkImageView> ZSwapChain::s_swapChainImageView = BuildVectorVkImageView();
}