#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Vulkan/ZVulkan.hpp>
#include <Zanix/Vulkan/ZSwapChain.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Vulkan/ZDevice.hpp>

#include <set>
#include <map>
#include <algorithm>

namespace Zx
{
	const std::vector<const char*> ZDevice::s_deviceExtensions =
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
	}

	/*
	@brief : Uninitialize ZDevice
	*/
	void ZDevice::UnInitializeDevice()
	{
		

		vkDestroyDevice(s_logicalDevice, nullptr);
	}

	/*
	@brief : Returns the device found by FoundPhysicalDevice
	*/
	VkPhysicalDevice& ZDevice::GetPhysicalDevice()
	{
		return s_physicalDevice;
	}

	/*
	@brief : Returns the logical device
	*/
	VkDevice& ZDevice::GetLogicalDevice()
	{
		return s_logicalDevice;
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
		return s_deviceExtensions;
	}	

	//-------------------------Private methods-------------------------

	void ZDevice::CreateLogicalDevice()
	{
		Queue queue = GetQueueFamiliy(s_physicalDevice);

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

		createInfo.enabledExtensionCount = static_cast<uint32_t>(s_deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = s_deviceExtensions.data();

		#ifdef ZDEBUG
				createInfo.enabledLayerCount = static_cast<uint32_t>(ZVulkan::GetValidationsLayers().size());
				createInfo.ppEnabledLayerNames = ZVulkan::GetValidationsLayers().data();
		#else
				createInfo.enabledLayerCount = 0;
		#endif

		if (vkCreateDevice(s_physicalDevice, &createInfo, nullptr, &s_logicalDevice) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create logical device");

		vkGetDeviceQueue(s_logicalDevice, queue.indexFamily, 0, &s_graphicsQueue);
		vkGetDeviceQueue(s_logicalDevice, queue.presentFamily, 0, &s_presentQueue);
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

		int score = 0;

		for (const auto& device : devices)
		{
			score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		//We get the best GPU
		if (devicesMap.rbegin()->first > 0)
			s_physicalDevice = devicesMap.rbegin()->second;
		else
			throw ZOperationFailed(__FILE__, "Failed to find a suitable GPU");
	}

	//----------------------------------------------------------------

	int ZDevice::GetGPUScore(VkPhysicalDevice device)
	{
		Queue queue = GetQueueFamiliy(device);
		
		if (!queue.IsValidQueue() || !(IsDeviceExtensionSupport(device)))
			return 0;

		//Nécessite la vérification des extensions IsDeviceExtensionSupport
		bool swapChainAdequate = false;
		ZSwapChain::SwapChainDetails details = ZSwapChain::BuildSwapChainDetails(device);
		swapChainAdequate = !details.format.empty() && !details.presentmode.empty();

		if (!swapChainAdequate)
			return 0;

		int score = 0;
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// Si le GPU ne supporte pas les shaders
		if (!(deviceFeatures.geometryShader))
			return 0;
		
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += deviceProperties.limits.maxImageDimension2D;
		score += deviceProperties.limits.maxViewports;

		return score;
	}
	
	//----------------------------------------------------------------

	bool ZDevice::IsDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionProperties(extensionsCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, extensionProperties.data());

		for (const char* extensionName : s_deviceExtensions)
		{
			bool found = false;
			for (const auto& extension : extensionProperties)
				if (std::strcmp(extension.extensionName, extensionName) == 0)
				{
					found = true;
					break;
				}

			if (!found)
				return false;
		}

		return true;
	}

	VkPhysicalDevice ZDevice::s_physicalDevice = VK_NULL_HANDLE;
	VkDevice ZDevice::s_logicalDevice = VK_NULL_HANDLE;
	VkQueue ZDevice::s_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::s_presentQueue = VK_NULL_HANDLE;
}