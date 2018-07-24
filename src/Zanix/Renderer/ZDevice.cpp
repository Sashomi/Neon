#include <vector>
#include <map>

#include <Zanix/Renderer/ZWindow.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>
#include <Zanix/Renderer/ZSwapChain.hpp>
#include <Zanix/Renderer/ZDevice.hpp>

namespace Zx
{
	/*
	@brief : Founds a physical device and gets a graphics queue
	*/
	void ZDevice::CreateDevice()
	{
		FoundPhysicalDevice();
		CreateLogicalDevice();
		GetDeviceQueue();
		CreateSemaphores();
	}

	/*
	@brief : Destroys device, semaphores, swap chain, and the presentation surface
	*/
	void ZDevice::DestroyDevice()
	{
		if (s_logicalDevice != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(s_logicalDevice);
			
			if (s_imageAvailableSemaphore != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(s_logicalDevice, s_imageAvailableSemaphore, nullptr);
			}

			if (s_renderingFinishedSemaphore != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(s_logicalDevice, s_renderingFinishedSemaphore, nullptr);
			}

			if (ZSwapChain::GetSwapChain() != VK_NULL_HANDLE)
			{
				vkDestroySwapchainKHR(s_logicalDevice, ZSwapChain::GetSwapChain(), nullptr);
			}

			vkDestroyDevice(s_logicalDevice, nullptr);

			if (ZWindow::GetSurface() != VK_NULL_HANDLE)
			{
				vkDestroySurfaceKHR(ZRenderer::GetVulkanInstance(), ZWindow::GetSurface(), nullptr);
			}
		}
	}

	/*
	@brief : Gets the graphics index family and the present index family
	*/
	void ZDevice::GetDeviceQueue()
	{
		vkGetDeviceQueue(s_logicalDevice, s_graphicsIndexFamily, 0, &s_graphicsQueue);
		vkGetDeviceQueue(s_logicalDevice, s_presentIndexFamily, 0, &s_presentQueue);
	}

	/*
	@brief : Gets the logical device
	*/
	const VkDevice& ZDevice::GetLogicalDevice()
	{
		return s_logicalDevice;
	}
	
	/*
	@brief : Gets the physical device
	*/
	const VkPhysicalDevice& ZDevice::GetPhysicalDevice()
	{
		return s_physicalDevice;
	}

	/*
	@brief : Gets the image available semaphore
	*/
	const VkSemaphore& ZDevice::GetImageAvailableSemaphore()
	{
		return s_imageAvailableSemaphore;
	}

	/*
	@brief : Gets the rendering (finished) semaphore
	*/
	const VkSemaphore& ZDevice::GetRenderingFinishedSemaphore()
	{
		return s_renderingFinishedSemaphore;
	}

	/*
	@brief : Gets the presentation queue
	*/
	const VkQueue& ZDevice::GetPresentQueue()
	{
		return s_presentQueue;
	}

	/*
	@brief : Gets the graphics queue
	*/
	const VkQueue& ZDevice::GetGraphicsQueue()
	{
		return s_graphicsQueue;
	}

	/*
	@brief : Gets the index of the presentation family queue
	*/
	uint32_t ZDevice::GetPresentIndexFamilyQueue()
	{
		return s_presentIndexFamily;
	}

	/*
	@brief : Gets the index of the graphics family queue
	*/
	uint32_t ZDevice::GetGraphicsIndexFamilyQueue()
	{
		return s_graphicsIndexFamily;
	}

	//-------------------------Private method-------------------------

	void ZDevice::FoundPhysicalDevice()
	{
		uint32_t numDevices = 0;
		if ((vkEnumeratePhysicalDevices(ZRenderer::GetVulkanInstance(), &numDevices, nullptr) != VK_SUCCESS) || (numDevices == 0))
			throw ZOperationFailed(__FILE__, "Failed to enumerate physical device");

		std::vector<VkPhysicalDevice> physicalDevice(numDevices);
		if(vkEnumeratePhysicalDevices(ZRenderer::GetVulkanInstance(), &numDevices, physicalDevice.data()) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to enumerate physical device - data");

		std::multimap<int, VkPhysicalDevice> devicesMap;
		int score = 0;

		for (const auto& device : physicalDevice)
		{
			score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		// On r�cup�re le meilleur GPU
		if (devicesMap.rbegin()->first > 0)
			s_physicalDevice = devicesMap.rbegin()->second;
		else
			throw ZOperationFailed(__FILE__, "Failed to find a suitable GPU");
	}

	//--------------------------------------------------------------------------

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

	//--------------------------------------------------------------------------

	bool ZDevice::CheckFamilyQueue(const VkPhysicalDevice& physicalDevice)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0)
			throw ZOperationFailed(__FILE__, "This physical device doesn't have any queue families");

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		std::vector<VkBool32> queuePresentSupport(queueFamilyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

		uint32_t graphicsIndexFamily = UINT32_MAX;
		uint32_t presentIndexFamily = UINT32_MAX;

		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, ZWindow::GetSurface(), &queuePresentSupport[i]);
		
			if ((queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				if (s_graphicsIndexFamily == UINT32_MAX)
					graphicsIndexFamily = i;

				if (queuePresentSupport[i])
				{
					s_graphicsIndexFamily = i;
					s_presentIndexFamily = i;
					return true;
				}
			}
		}

		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			if (queuePresentSupport[i])
			{
				presentIndexFamily = i;
				break;
			}
		}

		if (graphicsIndexFamily == UINT32_MAX || presentIndexFamily == UINT32_MAX)
			return false;

		s_graphicsIndexFamily = graphicsIndexFamily;
		s_presentIndexFamily = presentIndexFamily;

		return true;
	}

	//--------------------------------------------------------------------------

	void ZDevice::CreateLogicalDevice()
	{	
		std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo;
		std::vector<float> queuePriorities = { 1.0f };

		deviceQueueInfo.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			s_graphicsIndexFamily,
			static_cast<uint32_t>(queuePriorities.size()),
			queuePriorities.data()
		});

		if (s_graphicsIndexFamily != s_presentIndexFamily)
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
			deviceQueueInfo.data(),
			0,
			nullptr,
			static_cast<uint32_t>(extensions.size()),
			extensions.data(),
			nullptr
		};

		if (vkCreateDevice(s_physicalDevice, &deviceInfo, nullptr, &s_logicalDevice) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a logical device.");
	}

	//--------------------------------------------------------------------------

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

	//--------------------------------------------------------------------------

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

	//-------------------------------------------------------------------------

	VkDevice ZDevice::s_logicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice ZDevice::s_physicalDevice = VK_NULL_HANDLE;
	uint32_t ZDevice::s_graphicsIndexFamily = UINT32_MAX;
	uint32_t ZDevice::s_presentIndexFamily = UINT32_MAX;
	VkQueue ZDevice::s_graphicsQueue = VK_NULL_HANDLE;
	VkQueue ZDevice::s_presentQueue = VK_NULL_HANDLE;
	VkSemaphore ZDevice::s_imageAvailableSemaphore = VK_NULL_HANDLE;
	VkSemaphore ZDevice::s_renderingFinishedSemaphore = VK_NULL_HANDLE;
}