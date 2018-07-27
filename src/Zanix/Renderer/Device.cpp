#include <vector>
#include <map>

#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/Renderer.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Device.hpp>

namespace Zx
{
	/*
	@brief : Founds a physical device and gets a graphics queue
	*/
	void Device::CreateDevice()
	{
		FoundPhysicalDevice();
		CreateLogicalDevice();
		GetDeviceQueue();
		CreateSemaphores();
	}

	/*
	@brief : Destroys device, semaphores, swap chain, and the presentation surface
	*/
	void Device::DestroyDevice()
	{
		if (s_devices->logicalDevice != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(s_devices->logicalDevice);
			
			if (s_devices->imageAvailableSemaphore != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(s_devices->logicalDevice, s_devices->imageAvailableSemaphore, nullptr);
			}

			if (s_devices->renderingFinishedSemaphore != VK_NULL_HANDLE)
			{
				vkDestroySemaphore(s_devices->logicalDevice, s_devices->renderingFinishedSemaphore, nullptr);
			}

			if (SwapChain::GetSwapChain() != VK_NULL_HANDLE)
			{
				vkDestroySwapchainKHR(s_devices->logicalDevice, SwapChain::GetSwapChain()->swapChain, nullptr);
			}

			vkDestroyDevice(s_devices->logicalDevice, nullptr);

			if (Window::GetSurface() != VK_NULL_HANDLE)
			{
				vkDestroySurfaceKHR(Renderer::GetVulkanInstance(), Window::GetSurface(), nullptr);
			}
		}
	}

	/*
	@brief : Gets the graphics index family and the present index family
	*/
	void Device::GetDeviceQueue()
	{
		vkGetDeviceQueue(s_devices->logicalDevice, s_devices->graphicsIndexFamily, 0, &s_devices->graphicsQueue);
		vkGetDeviceQueue(s_devices->logicalDevice, s_devices->presentIndexFamily, 0, &s_devices->presentQueue);
	}

	/*
	@brief : Returns the structure who countains the device's variables
	*/
	const std::shared_ptr <Device::Devices>& Device::GetDevices()
	{
		return s_devices;
	}

	//-------------------------Private method-------------------------

	void Device::FoundPhysicalDevice()
	{
		uint32_t numDevices = 0;
		if ((vkEnumeratePhysicalDevices(Renderer::GetVulkanInstance(), &numDevices, nullptr) != VK_SUCCESS) || (numDevices == 0))
			throw ZOperationFailed(__FILE__, "Failed to enumerate physical device");

		std::vector<VkPhysicalDevice> physicalDevice(numDevices);
		if(vkEnumeratePhysicalDevices(Renderer::GetVulkanInstance(), &numDevices, physicalDevice.data()) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to enumerate physical device - data");

		std::multimap<int, VkPhysicalDevice> devicesMap;
		int score = 0;

		for (const auto& device : physicalDevice)
		{
			score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		// On récupère le meilleur GPU
		if (devicesMap.rbegin()->first > 0)
			s_devices->physicalDevice = devicesMap.rbegin()->second;
		else
			throw ZOperationFailed(__FILE__, "Failed to find a suitable GPU");
	}

	//--------------------------------------------------------------------------

	int Device::GetGPUScore(const VkPhysicalDevice& device)
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

	bool Device::CheckFamilyQueue(const VkPhysicalDevice& physicalDevice)
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
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, Window::GetSurface(), &queuePresentSupport[i]);
		
			if ((queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				if (s_devices->graphicsIndexFamily == UINT32_MAX)
					graphicsIndexFamily = i;

				if (queuePresentSupport[i])
				{
					s_devices->graphicsIndexFamily = i;
					s_devices->presentIndexFamily = i;
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

		s_devices->graphicsIndexFamily = graphicsIndexFamily;
		s_devices->presentIndexFamily = presentIndexFamily;

		return true;
	}

	//--------------------------------------------------------------------------

	void Device::CreateLogicalDevice()
	{	
		std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo;
		std::vector<float> queuePriorities = { 1.0f };

		deviceQueueInfo.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			s_devices->graphicsIndexFamily,
			static_cast<uint32_t>(queuePriorities.size()),
			queuePriorities.data()
		});

		if (s_devices->graphicsIndexFamily != s_devices->presentIndexFamily)
		{
			deviceQueueInfo.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				s_devices->presentIndexFamily,
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

		if (vkCreateDevice(s_devices->physicalDevice, &deviceInfo, nullptr, &s_devices->logicalDevice) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a logical device.");
	}

	//--------------------------------------------------------------------------

	void Device::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo =
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0
		};

		if ((vkCreateSemaphore(s_devices->logicalDevice, &semaphoreInfo, nullptr, &s_devices->imageAvailableSemaphore) != VK_SUCCESS)
			|| (vkCreateSemaphore(s_devices->logicalDevice, &semaphoreInfo, nullptr, &s_devices->renderingFinishedSemaphore) != VK_SUCCESS))
			throw ZOperationFailed(__FILE__, "Failed to create semaphore");
	}

	//--------------------------------------------------------------------------

	bool Device::IsExtensionAvailable()
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(s_devices->physicalDevice, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionsAvailable(extensionsCount);
		vkEnumerateDeviceExtensionProperties(s_devices->physicalDevice, nullptr, &extensionsCount, extensionsAvailable.data());

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

	std::shared_ptr<Device::Devices> Device::s_devices = std::make_shared<Device::Devices>();
}