#include <vector>
#include <map>

#include <Zanix/Renderer/ZSurface.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>
#include <Zanix/Renderer/ZDevice.hpp>

namespace Zx
{
	/*
	@brief : Founds a physical device and gets a graphics queue
	*/
	void ZDevice::InitDevice()
	{
		FoundPhysicalDevice();
		CreateLogicalDevice();
		GetDeviceQueue();
	}

	/*
	@brief : Destroys the logical device
	*/
	void ZDevice::DestroyDevice()
	{
		if (s_logicalDevice != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(s_logicalDevice);
			vkDestroyDevice(s_logicalDevice, nullptr);
		}
	}

	VkDevice ZDevice::GetLogicalDevice()
	{
		return s_logicalDevice;
	}

	VkPhysicalDevice ZDevice::GetPhysicalDevice()
	{
		return s_physicalDevice;
	}

	//-------------------------Private method-------------------------

	void ZDevice::FoundPhysicalDevice()
	{
		uint32_t numDevices = 0;
		vkEnumeratePhysicalDevices(ZRenderer::GetVulkanInstance(), &numDevices, nullptr);

		std::vector<VkPhysicalDevice> physicalDevice(numDevices);
		vkEnumeratePhysicalDevices(ZRenderer::GetVulkanInstance(), &numDevices, physicalDevice.data());

		std::multimap<int, VkPhysicalDevice> devicesMap;
		int score = 0;

		for (const auto& device : physicalDevice)
		{
			score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		// On récupère le meilleur GPU
		if (devicesMap.rbegin()->first > 0)
			s_physicalDevice = devicesMap.rbegin()->second;
		else
			throw ZOperationFailed(__FILE__, "Failed to find a suitable GPU");
	}

	int ZDevice::GetGPUScore(const VkPhysicalDevice& device)
	{
		if (!CheckFamilyQueue(device))
			return 0;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (!(deviceFeatures.geometryShader))
			return 0;

		int score = 0;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += deviceProperties.limits.maxImageDimension3D;

		return score;
	}

	bool ZDevice::CheckFamilyQueue(const VkPhysicalDevice& device)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0)
			throw ZOperationFailed(__FILE__, "This physical device doesn't have any queue.");

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

		std::vector<VkBool32> queuePresentSupport(queueFamilyCount);

		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(s_physicalDevice, i, ZSurface::GetSurfacePlatform().GetSurface(), &queuePresentSupport[i]);

			if ((queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				if (s_indexFamily == UINT32_MAX)
					s_indexFamily = i;

				if (queuePresentSupport[i])
				{
					s_indexFamily = i;
					s_presentIndexFamily = i;
					return true;
				}
			}
		}

		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			if (queuePresentSupport[i])
			{
				s_presentIndexFamily = i;
				break;
			}
		}

		if (s_indexFamily == UINT32_MAX || s_presentIndexFamily == UINT32_MAX)
			return false;

		return true;
	}

	void ZDevice::CreateLogicalDevice()
	{	
		std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo;
		std::vector<float> queuePriorities = { 1.0f };

		deviceQueueInfo.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			s_indexFamily,
			static_cast<uint32_t>(queuePriorities.size()),
			queuePriorities.data()
		});

		if (s_indexFamily != s_presentIndexFamily)
		{
			deviceQueueInfo.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				s_presentIndexFamily,
				static_cast<uint32_t>(queuePriorities.size()),
				queuePriorities.data()
			});
		}

		std::vector<const char*> extensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDeviceCreateInfo deviceInfo =
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(deviceQueueInfo.size()),
			&deviceQueueInfo[0],
			0,
			nullptr,
			static_cast<uint32_t>(extensions.size()),
			&extensions[0],
			nullptr
		};

		if (vkCreateDevice(s_physicalDevice, &deviceInfo, nullptr, &s_logicalDevice) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a logical device.");
	}

	void ZDevice::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo =
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0
		};

		if ((vkCreateSemaphore(s_logicalDevice, &semaphoreInfo, nullptr, &s_imageAvailableSemaphore) != VK_SUCCESS) 
			|| (vkCreateSemaphore(s_logicalDevice, &semaphoreInfo, nullptr, &s_renderingFinishedSemaphore) != VK_SUCCESS))
			throw ZOperationFailed(__FILE__, "Failed to create semaphore");
	}

	void ZDevice::GetDeviceQueue()
	{
		vkGetDeviceQueue(s_logicalDevice, s_indexFamily, 0, &s_graphicsQueue);
		vkGetDeviceQueue(s_logicalDevice, s_presentIndexFamily, 0, &s_presentQueue);
	}

	bool ZDevice::IsExtensionAvailable()
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(s_physicalDevice, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionsAvailable(extensionsCount);
		vkEnumerateDeviceExtensionProperties(s_physicalDevice, nullptr, &extensionsCount, extensionsAvailable.data());

		std::vector<const char*> deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		for (std::size_t i = 0; i < deviceExtensions.size(); i++)
		{
			for (std::size_t j = 0; j < extensionsAvailable.size(); j++)
			{
				if (std::strcmp(extensionsAvailable[j].extensionName, deviceExtensions[i]) == 0)
					return true;
			}
		}

		return false;
	}

	VkDevice ZDevice::s_logicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice ZDevice::s_physicalDevice = VK_NULL_HANDLE;
	uint32_t ZDevice::s_indexFamily = UINT32_MAX;
	uint32_t ZDevice::s_presentIndexFamily = UINT32_MAX;
	VkQueue ZDevice::s_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::s_presentQueue = VK_NULL_HANDLE;
	VkSemaphore ZDevice::s_imageAvailableSemaphore = VK_NULL_HANDLE;
	VkSemaphore ZDevice::s_renderingFinishedSemaphore = VK_NULL_HANDLE;
}