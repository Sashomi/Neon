#include <Neon/Core/Exception.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/Window.hpp>
#include <Neon/Renderer/SwapChain.hpp>

namespace Zx
{
	/*
	@brief : Constructor with the needed informations
	@param : The device of the application
	@param : The window of the application
	*/
	SwapChain::SwapChain(const Device& device, const Window& window) : m_isRenderAvailable(false)
	{
		m_device = std::make_shared<Device>(device);
		m_window = std::make_shared<Window>(window);
		m_swapChain = std::make_shared<SwapChains>();
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the swapChain to copy
	*/
	SwapChain::SwapChain(const SwapChain& swap) : m_device(swap.m_device), m_swapChain(swap.m_swapChain), m_window(swap.m_window)
	{}

	/*
	@brief : Movement constructor
	@param : A constant reference to the SwapChain to move
	*/
	SwapChain::SwapChain(SwapChain&& swapChain) noexcept
	{
		std::swap(m_device, swapChain.m_device);
		std::swap(m_swapChain, swapChain.m_swapChain);
		std::swap(m_window, swapChain.m_window);
	}

	/*
	@brief : Destroys SwapChain
	*/
	SwapChain::~SwapChain()
	{
		if (m_swapChain->swapChain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(m_device->GetDevice()->logicalDevice, m_swapChain->swapChain, nullptr);
			m_swapChain->swapChain = VK_NULL_HANDLE;
		}
	}

	/*
	@brief : Creates a swap chain
	@returns : Returns true if the creation is a success, false otherwise
	*/
	bool SwapChain::CreateSwapChain()
	{
		if (m_swapChain == nullptr)
			m_swapChain = std::make_shared<SwapChains>();

		m_isRenderAvailable = false;

		if (m_device->GetDevice()->logicalDevice != VK_NULL_HANDLE)
			vkDeviceWaitIdle(m_device->GetDevice()->logicalDevice);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;	
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->GetDevice()->physicalDevice, m_window->GetSurface(), &surfaceCapabilities) != VK_SUCCESS)
		{
			std::cout << "Failed to get a physical device surface capabilities" << std::endl;
			return false;
		}

		//Formats

		uint32_t formatCount = 0;
		if ((vkGetPhysicalDeviceSurfaceFormatsKHR(m_device->GetDevice()->physicalDevice, m_window->GetSurface(), &formatCount, nullptr)
			!= VK_SUCCESS) || (formatCount == 0))
		{
			std::cout << "Failed to get the format count available" << std::endl;
			return false;
		}

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_device->GetDevice()->physicalDevice, m_window->GetSurface(), &formatCount, surfaceFormats.data())
			!= VK_SUCCESS)
		{
			std::cout << "Failed to get the format count available - data" << std::endl;
			return false;
		}

		//Modes

		uint32_t modesCount = 0;
		if ((vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->GetDevice()->physicalDevice, m_window->GetSurface(), &modesCount, nullptr)
			!= VK_SUCCESS) || (modesCount == 0))
		{
			std::cout << "Failed to get the present modes count available" << std::endl;
			return false;
		}

		std::vector<VkPresentModeKHR> presentModes(modesCount);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->GetDevice()->physicalDevice, m_window->GetSurface(), &modesCount, presentModes.data())
			!= VK_SUCCESS)
		{
			std::cout << "Failed to get the present modes count available - data" << std::endl;
			return false;
		}

		//Création de la swap chain

		uint32_t desiredNumImages = GetSwapChainNumImages(surfaceCapabilities);
		VkSurfaceFormatKHR desiredSurfaceFormat = GetSwapChainFormat(surfaceFormats);
		VkExtent2D desiredExtent = GetSwapChainExtent(surfaceCapabilities);
		VkImageUsageFlags desiredUsageFlags = GetSwapChainUsageFlags(surfaceCapabilities);
		VkSurfaceTransformFlagBitsKHR desiredSurfaceTransform = GetSwapChainTransform(surfaceCapabilities);
		VkPresentModeKHR desiredPresentMode = GetSwapChainPresentMode(presentModes);
		VkSwapchainKHR oldSwapChain = m_swapChain->swapChain;

		if ((static_cast<int>(desiredUsageFlags) == -1) || (static_cast<int>(desiredPresentMode) == -1))
			return false;

		if ((desiredExtent.width == 0) || (desiredExtent.height == 0))
			return true;

		VkSwapchainCreateInfoKHR swapChainInfo =
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,
			m_window->GetSurface(),
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

		if (vkCreateSwapchainKHR(m_device->GetDevice()->logicalDevice, &swapChainInfo, nullptr, &m_swapChain->swapChain) != VK_SUCCESS)
		{
			std::cout << "Failed to create swap chain" << std::endl;
			return false;
		}

		if (oldSwapChain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(m_device->GetDevice()->logicalDevice, oldSwapChain, nullptr);

		m_isRenderAvailable = true;

		m_swapChain->extent = desiredExtent;
		m_swapChain->format = desiredSurfaceFormat.format;
		
		uint32_t imageCount = 0;
		if ((vkGetSwapchainImagesKHR(m_device->GetDevice()->logicalDevice, m_swapChain->swapChain, &imageCount, nullptr)) != VK_SUCCESS || (imageCount == 0))
		{
			std::cout << "Could not get the number of swap chain images" << std::endl;
			return false;
		}

		m_swapChain->image.resize(imageCount);
			
		if(vkGetSwapchainImagesKHR(m_device->GetDevice()->logicalDevice, m_swapChain->swapChain, &imageCount, m_swapChain->image.data()) != VK_SUCCESS)
		{
			std::cout << "Could not get the number of swap chain images - data" << std::endl;
			return false;
		}

		return CreateSwapChainImageView();
	}

	/*
	@brief : Assign a SwapChain by move semantic
	@param : A reference to the SwapChain to move
	@return : The SwapChain to move
	*/
	SwapChain& SwapChain::operator=(SwapChain&& swapChain) noexcept
	{
		std::swap(m_device, swapChain.m_device);
		std::swap(m_swapChain, swapChain.m_swapChain);
		std::swap(m_window, swapChain.m_window);

		return (*this);
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

	bool SwapChain::CreateSwapChainImageView()
	{
		m_swapChain->imageView.resize(m_swapChain->image.size());

		for (size_t i = 0; i < m_swapChain->image.size(); ++i)
		{
			VkImageViewCreateInfo imageViewInfo =
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				0,
				m_swapChain->image[i],
				VK_IMAGE_VIEW_TYPE_2D,
				m_swapChain->format,
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
			
			if (vkCreateImageView(m_device->GetDevice()->logicalDevice, &imageViewInfo, nullptr, &m_swapChain->imageView[i]) != VK_SUCCESS)
			{
				std::cout << "Failed to create a swap chain imave view" << std::endl;
				return false;
			}
		}

		m_isRenderAvailable = true;
		
		return true;
	}
}