#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Renderer/ZVulkan.hpp>
#include <Zanix/ZUtils.hpp>
#include <Zanix/Component/ZDevice.hpp>

#include <set>
#include <map>
#include <algorithm>

namespace Zx
{
	const std::vector<const char*> ZDevice::m_deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/*
	@brief : Initialize ZDevice
	*/
	void ZDevice::InitializeDevice()
	{
		FoundPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain();
	}

	/*
	@brief : Uninitialize ZDevice
	*/
	void ZDevice::UnInitializeDevice()
	{
		vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	/*
	@brief : Returns the device found by FoundPhysicalDevice
	*/
	VkPhysicalDevice& ZDevice::GetPhysicalDevice()
	{
		return m_physicalDevice;
	}

	/*
	@brief : Returns the logical device
	*/
	VkDevice& ZDevice::GetLogicalDevice()
	{
		return m_logicalDevice;
	}

	/*
	@brief : Returns the swap chain
	*/
	VkSwapchainKHR& ZDevice::GetSwapChain()
	{
		return m_swapChain;
	}

	/*
	@brief : Returns the queue family for graphics
	@param : The device use
	*/
	ZDevice::Queue ZDevice::GetQueueFamiliy(VkPhysicalDevice device)
	{
		Queue queue;

		uint32_t queueFamilyCount = 0;
		
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				queue.indexFamily = i;
			
			VkBool32 presentSupported = false;

			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ZVulkan::GetWindowSurface(), &presentSupported);

			if (queueFamily.queueCount > 0 && presentSupported)
				queue.presentFamily = i;

			if (queue.IsValidQueue())
				break;

			i++;
		}

		return queue;
	}
	
	/*
	@brief : Returns the require physical device extensions
	*/
	const std::vector<const char*>& ZDevice::GetDeviceExtension()
	{
		return m_deviceExtensions;
	}	

	//-------------------------Private method-------------------------

	void ZDevice::CreateLogicalDevice()
	{
		Queue queue = GetQueueFamiliy(m_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::set<int> uniqueQueueFamilies =
		{
			queue.indexFamily, queue.presentFamily
		};

		float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		//On le laisse sans rien pour l'instant on précisera des options potentielles plus tard
		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &physicalDeviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

		#ifdef ZDEBUG
				createInfo.enabledLayerCount = static_cast<uint32_t>(ZVulkan::GetValidationsLayers().size());
				createInfo.ppEnabledLayerNames = ZVulkan::GetValidationsLayers().data();
		#else
				createInfo.enabledLayerCount = 0;
		#endif

		if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create logical device");

		vkGetDeviceQueue(m_logicalDevice, queue.indexFamily, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, queue.presentFamily, 0, &m_presentQueue);
	}

	//----------------------------------------------------------------

	void ZDevice::FoundPhysicalDevice()
	{
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(ZVulkan::GetVulkanInstance(), &gpuCount, nullptr);

		if (gpuCount == 0)
			throw ZOperationFailed(__FILE__, "No GPU support vulkan found");

		std::vector<VkPhysicalDevice> devices(gpuCount);
		vkEnumeratePhysicalDevices(ZVulkan::GetVulkanInstance(), &gpuCount, devices.data());

		std::multimap<int, VkPhysicalDevice> devicesMap;

		for (const auto& device : devices)
		{
			int score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		//We get the best GPU
		if (devicesMap.rbegin()->first > 0)
			m_physicalDevice = devicesMap.rbegin()->second;
		else
			throw ZOperationFailed(__FILE__, "Failed to find a suitable GPU");
	}

	//----------------------------------------------------------------

	void ZDevice::CreateSwapChain()
	{
		SwapChainDetails swapChain = BuildSwapChainDetails(m_physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = GetSwapSurfaceFormat(swapChain.format);
		VkPresentModeKHR presentMode = GetSwapPresentMode(swapChain.presentmode);
		VkExtent2D extent = GetSwapExtent(swapChain.capabilities);

		uint32_t imageCount = swapChain.capabilities.minImageCount + 1;

		if (swapChain.capabilities.maxImageCount > 0 && imageCount > swapChain.capabilities.maxImageCount) {
			imageCount = swapChain.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = ZVulkan::GetWindowSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		Queue index = GetQueueFamiliy(m_physicalDevice);
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

		if (vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
			throw ZOperationFailed(__FILE__, "Failed to create swap chain");
		}

		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
		m_swapChainImage.resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImage.data());

		m_swapChainExtent = extent;
		m_swapChainImageFormat = surfaceFormat.format;
	}

	//----------------------------------------------------------------

	int ZDevice::GetGPUScore(VkPhysicalDevice device)
	{
		Queue queue = GetQueueFamiliy(device);

		if (!queue.IsValidQueue() || !(IsDeviceExtensionSupport(device)))
			return 0;

		//Nécessite la vérification des extensions IsDeviceExtensionSupport
		bool swapChainAdequate = false;
		SwapChainDetails details = BuildSwapChainDetails(device);
		swapChainAdequate = !details.format.empty() && !details.presentmode.empty();

		if (!swapChainAdequate)
			return 0;

		int score = 0;
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += deviceProperties.limits.maxImageDimension2D;
		score += deviceProperties.limits.maxViewports;

		// Si le GPU ne supporte pas les shaders
		if (!(deviceFeatures.geometryShader))
			return 0;

		return score;
	}

	//----------------------------------------------------------------
	
	VkExtent2D ZDevice::GetSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilitities)
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

	VkSurfaceFormatKHR ZDevice::GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormat)
	{
		// Si la surface n'a aucun format favoris, dans ce cas la taille de "surfaceFormat" vaut 1
		if (surfaceFormat.size() == 1 && surfaceFormat[0].format == VK_FORMAT_UNDEFINED)
			return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

		// On cherche la meilleure possibilitée
		for (const auto& format : surfaceFormat)
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;

		return surfaceFormat[0];
	}
	//----------------------------------------------------------------


	VkPresentModeKHR ZDevice::GetSwapPresentMode(const std::vector<VkPresentModeKHR>& presentMode)
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

	bool ZDevice::IsDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionProperties(extensionsCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, extensionProperties.data());

		for (const char* extensionName : m_deviceExtensions)
		{
			bool found = false;
			for (const auto& extension : extensionProperties)
			{
				if (std::strcmp(extension.extensionName, extensionName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
				return false;
		}

		return true;
	}

	//----------------------------------------------------------------

	std::vector<VkImage> ZDevice::BuildVectorVkImage()
	{
		std::vector<VkImage> vec;

		return vec;
	}

	VkPhysicalDevice ZDevice::m_physicalDevice = VK_NULL_HANDLE;
	VkDevice ZDevice::m_logicalDevice = VK_NULL_HANDLE;
	VkQueue ZDevice::m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::m_presentQueue = VK_NULL_HANDLE;
	VkSwapchainKHR ZDevice::m_swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> ZDevice::m_swapChainImage = BuildVectorVkImage();
	VkFormat ZDevice::m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D ZDevice::m_swapChainExtent = { 0, 0 };
}