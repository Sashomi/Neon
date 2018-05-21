#include <Zanix/Renderer/ZVulkan.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Component/ZDevice.hpp>

#include <set>

namespace Zx
{
	/*
	@brief : Founds an appropriate physical device
	*/
	void ZDevice::FoundPhysicalDevice()
	{
		uint32_t gpuCount = 0;
		vkEnumeratePhysicalDevices(ZVulkan::GetVulkanInstance(), &gpuCount, nullptr);

		if (gpuCount == 0)
		{
			throw ZOperationFailed(__FILE__, "No GPU support vulkan found");
		}

		std::vector<VkPhysicalDevice> devices(gpuCount);
		vkEnumeratePhysicalDevices(ZVulkan::GetVulkanInstance(), &gpuCount, devices.data());
		
		for (const auto& device : devices)
		{
			if (IsPhysicalDeviceAppropriate(device))
			{
				m_physicalDevice = device;
				break;
			}
		}

		//If no device found
		if (m_physicalDevice == VK_NULL_HANDLE)
			throw ZOperationFailed(__FILE__, "Failed to found an appropriate GPU");
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

		#ifndef NDEBUG
				createInfo.enabledLayerCount = static_cast<uint32_t>(ZVulkan::GetValidationsLayers().size());
				createInfo.ppEnabledLayerNames = ZVulkan::GetValidationsLayers().data();
		#else
				createInfo.enabledLayerCount = 0;
		#endif

		if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
		{
			throw ZOperationFailed(__FILE__, "Failed to create logical device");
		}
		
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
			VkBool32 presentSupported = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, ZVulkan::GetWindowSurface(), &presentSupported);

			if (queueFamily.queueCount > 0 && presentSupported)
				queue.indexFamily = i;

			if (queue.IsValidQueue())
				break;

			i++;
		}

		return queue;
	}

	/*
	@brief : Returns true if the device is appropriate, false otherwise
	@param : The device to test
	*/
	bool ZDevice::IsPhysicalDeviceAppropriate(VkPhysicalDevice device)
	{
		Queue queue = GetQueueFamiliy(device);
		
		return queue.IsValidQueue();
	}

	VkPhysicalDevice ZDevice::m_physicalDevice = VK_NULL_HANDLE;
	VkDevice ZDevice::m_logicalDevice = VK_NULL_HANDLE;
	VkQueue ZDevice::m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::m_presentQueue = VK_NULL_HANDLE;
}