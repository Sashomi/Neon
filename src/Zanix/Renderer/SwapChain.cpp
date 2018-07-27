#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/SwapChain.hpp>

namespace Zx
{
	/*
	@brief : Creates a swap chain
	@returns : Returns true if the creation is a success, false otherwise
	*/
	bool SwapChain::CreateSwapChain()
	{
		s_isRenderAvailable = false;

		if (Device::GetDevices()->logicalDevice != VK_NULL_HANDLE)
			vkDeviceWaitIdle(Device::GetDevices()->logicalDevice);

		for (size_t i = 0; i < s_swapChain->image.size(); i++)
		{
			if (s_swapChain->imageView[i] != VK_NULL_HANDLE)
			{
				vkDestroyImageView(Device::GetDevices()->logicalDevice, s_swapChain->imageView[i], nullptr);
				s_swapChain->imageView[i] = VK_NULL_HANDLE;
			}
		}

		s_swapChain->image.clear();

		VkSurfaceCapabilitiesKHR surfaceCapabilities;

		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device::GetDevices()->physicalDevice, Window::GetSurface(), &surfaceCapabilities) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get a physical device surface capabilities");

		//Formats

		uint32_t formatCount = 0;
		if((vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetDevices()->physicalDevice, Window::GetSurface(), &formatCount, nullptr)
			!= VK_SUCCESS) ||(formatCount == 0))
			throw ZOperationFailed(__FILE__, "Failed to get the format count available");

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		if(vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetDevices()->physicalDevice, Window::GetSurface(), &formatCount, surfaceFormats.data())
			!= VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get the format count available - data");

		//Modes

		uint32_t modesCount = 0;
		if ((vkGetPhysicalDeviceSurfacePresentModesKHR(Device::GetDevices()->physicalDevice, Window::GetSurface(), &modesCount, nullptr)
			!= VK_SUCCESS) || (modesCount == 0))
			throw ZOperationFailed(__FILE__, "Failed to get the present modes count available");

		std::vector<VkPresentModeKHR> presentModes(modesCount);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(Device::GetDevices()->physicalDevice, Window::GetSurface(), &modesCount, presentModes.data())
			!= VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to get the present modes count available - data");

		//Création de la swap chain

		uint32_t desiredNumImages = GetSwapChainNumImages(surfaceCapabilities);
		VkSurfaceFormatKHR desiredSurfaceFormat = GetSwapChainFormat(surfaceFormats);
		VkExtent2D desiredExtent = GetSwapChainExtent(surfaceCapabilities);
		VkImageUsageFlags desiredUsageFlags = GetSwapChainUsageFlags(surfaceCapabilities);
		VkSurfaceTransformFlagBitsKHR desiredSurfaceTransform = GetSwapChainTransform(surfaceCapabilities);
		VkPresentModeKHR desiredPresentMode = GetSwapChainPresentMode(presentModes);
		VkSwapchainKHR oldSwapChain = s_swapChain->swapChain;

		if ((static_cast<int>(desiredUsageFlags) == -1) || (static_cast<int>(desiredPresentMode) == -1))
			return false;

		if ((desiredExtent.width == 0) || (desiredExtent.height == 0))
			return true;

		VkSwapchainCreateInfoKHR swapChainInfo =
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,
			Window::GetSurface(),
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

		if (vkCreateSwapchainKHR(Device::GetDevices()->logicalDevice, &swapChainInfo, nullptr, &s_swapChain->swapChain) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create swap chain");

		if (oldSwapChain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(Device::GetDevices()->logicalDevice, oldSwapChain, nullptr);

		s_isRenderAvailable = true;

		s_swapChain->format = desiredSurfaceFormat.format;
		
		uint32_t imageCount = 0;
		if ((vkGetSwapchainImagesKHR(Device::GetDevices()->logicalDevice, s_swapChain->swapChain, &imageCount, nullptr)) != VK_SUCCESS || (imageCount == 0))
			throw ZOperationFailed(__FILE__, "Could not get the number of swap chain images");

		s_swapChain->image.resize(imageCount);
			
		if(vkGetSwapchainImagesKHR(Device::GetDevices()->logicalDevice, s_swapChain->swapChain, &imageCount, s_swapChain->image.data()) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Could not get swap chain images");

		CreateSwapChainImageView();

		return true;
	}

	/*
	@brief : Returns the swap chain
	*/
	const std::shared_ptr<SwapChain::SwapChains>&  SwapChain::GetSwapChain()
	{
		return s_swapChain;
	}


	//-------------------------Private method-------------------------

	uint32_t SwapChain::GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
		if ((surfaceCapabilities.maxImageCount > 0) && (imageCount > surfaceCapabilities.maxImageCount))
			imageCount = surfaceCapabilities.maxImageCount;

		return imageCount;
	}

	//-------------------------------------------------------------------------

	VkSurfaceFormatKHR SwapChain::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats)
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

	//-------------------------------------------------------------------------

	VkExtent2D SwapChain::GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.currentExtent.width == -1)
		{
			VkExtent2D swapChainExtent = { 640, 480 };
			
			if (swapChainExtent.width < surfaceCapabilities.minImageExtent.width)
				swapChainExtent.width = surfaceCapabilities.minImageExtent.width;
			if (swapChainExtent.height < surfaceCapabilities.minImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.minImageExtent.height;

			if (swapChainExtent.width > surfaceCapabilities.maxImageExtent.width)
				swapChainExtent.width = surfaceCapabilities.maxImageExtent.width;
			if (swapChainExtent.height > surfaceCapabilities.maxImageExtent.height)
				swapChainExtent.height = surfaceCapabilities.maxImageExtent.height;

			return swapChainExtent;
		}

		return surfaceCapabilities.currentExtent;
	}

	//-------------------------------------------------------------------------

	VkImageUsageFlags SwapChain::GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		return static_cast<VkImageUsageFlags>(-1);
	}

	//-------------------------------------------------------------------------

	VkSurfaceTransformFlagBitsKHR SwapChain::GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
	{
		if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		
		return surfaceCapabilities.currentTransform;
	}

	//-------------------------------------------------------------------------

	VkPresentModeKHR SwapChain::GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
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

	//-------------------------------------------------------------------------

	void SwapChain::CreateSwapChainImageView()
	{
		s_swapChain->imageView.resize(s_swapChain->image.size());

		for (size_t i = 0; i < s_swapChain->image.size(); ++i)
		{
			VkImageViewCreateInfo imageViewInfo =
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				0,
				s_swapChain->image[0],
				VK_IMAGE_VIEW_TYPE_2D,
				s_swapChain->format,
				{
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY
				},
				{
					VK_IMAGE_ASPECT_COLOR_BIT,
					0,
					1,
					0,
					1
				}
			};
			
			if (vkCreateImageView(Device::GetDevices()->logicalDevice, &imageViewInfo, nullptr, &s_swapChain->imageView[i]) != VK_SUCCESS)
				throw ZOperationFailed(__FILE__, "Failed to create a swap chain imave view");
		}

		s_isRenderAvailable = true;
	}

	//-------------------------------------------------------------------------
	
	std::shared_ptr<SwapChain::SwapChains> SwapChain::s_swapChain = std::make_shared<SwapChain::SwapChains>();

	bool SwapChain::s_isRenderAvailable = false;
}