#include <Zanix/Renderer/ZVulkan.hpp>
#include <Zanix/ZUtils.hpp>
#include <Zanix/Component/ZDevice.hpp>

#include <set>
#include <map>

namespace Zx
{
	const std::vector<const char*> ZDevice::m_deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/*
	@brief : Founds an appropriate physical device
	*/
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

	/*
	@brief : Returns the device found by FoundPhysicalDevice
	*/
	VkPhysicalDevice ZDevice::GetPhysicalDevice()
	{
		return m_physicalDevice;
	}

	/*
	@brief : Returns the logical device
	*/
	VkDevice ZDevice::GetLogicalDevice()
	{
		return m_logicalDevice;
	}

	/*
	@brief : Creates logical device for m_physicalDevice
	*/
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

		createInfo.enabledExtensionCount = 0;

		#ifndef ZDEBUG
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

	int ZDevice::GetGPUScore(VkPhysicalDevice device)
	{
		Queue queue = GetQueueFamiliy(device);

		if (!queue.IsValidQueue() || !(IsDeviceExtensionSupport(device)))
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

	VkPhysicalDevice ZDevice::m_physicalDevice = VK_NULL_HANDLE;
	VkDevice ZDevice::m_logicalDevice = VK_NULL_HANDLE;
	VkQueue ZDevice::m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::m_presentQueue = VK_NULL_HANDLE;
}