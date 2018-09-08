#include <iostream>
#include <map>
#include <cstring>

#include <Neon/Core/Exception.hpp>
#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/Window.hpp>
#include <Neon/Renderer/CommandBuffers.hpp>
#include <Neon/Renderer/Renderer.hpp>
#include <Neon/Renderer/Device.hpp>

namespace Zx
{
	/*
	@brief : Constructor with the needed informations
	@param : The Renderer of the application
	@param : The SwapChain of the application
	@param : The window of the Application
	*/
	Device::Device(Renderer& renderer, SwapChain& swapChain, Window& window, std::vector<RenderingResourcesData>& renderingResources)
	{
		m_renderer = std::make_shared<Renderer>(renderer);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_window = std::make_shared<Window>(window);
		m_device = std::make_shared<Devices>();
		m_renderingResources = std::make_shared<std::vector<RenderingResourcesData>>(renderingResources);
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the Device to copy
	*/
	Device::Device(const Device& device) : m_renderer(device.m_renderer), m_swapChain(device.m_swapChain), m_window(device.m_window), m_device(device.m_device)
	{}
	
	/*
	@brief : Movement constructor
	@param : A constant reference to the Device to move
	*/
	Device::Device(Device&& device) noexcept
	{
		std::swap(m_renderer, device.m_renderer);
		std::swap(m_swapChain, device.m_swapChain);
		std::swap(m_window, device.m_window);
		std::swap(m_device, device.m_device);
	}

	/*
	@brief : Destroys the device
	*/
	Device::~Device()
	{
		vkDestroyDevice(m_device->logicalDevice, nullptr);
		m_device->logicalDevice = VK_NULL_HANDLE;
	}

	/*
	@brief : Creates the logical and physical device
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool Device::CreateDevice()
	{
		if (m_device == nullptr)
			m_device = std::make_shared<Devices>();

		if (!(FoundPhysicalDevice()) || !(CreateLogicalDevice()))
			return false;

		GetDeviceQueue();

		return true;
	}
	

	/*
	@brief : Assign a Device by move semantic
	@param : A reference to the Device to move
	@return : The Device to move
	*/
	Device& Device::operator=(Device&& device) noexcept
	{
		std::swap(m_renderer, device.m_renderer);
		std::swap(m_swapChain, device.m_swapChain);
		std::swap(m_window, device.m_window);
		std::swap(m_device, device.m_device);

		return (*this);
	}

	//-------------------------Private method-------------------------

	void Device::GetDeviceQueue()
	{
		vkGetDeviceQueue(m_device->logicalDevice, m_device->graphicsIndexFamily, 0, &m_device->graphicsQueue);
		vkGetDeviceQueue(m_device->logicalDevice, m_device->presentIndexFamily, 0, &m_device->presentQueue);
	}

	//--------------------------------------------------------------------------

	bool Device::FoundPhysicalDevice()
	{
		uint32_t numDevices = 0;
		if ((vkEnumeratePhysicalDevices(m_renderer->GetVulkanInstance(), &numDevices, nullptr) != VK_SUCCESS) || (numDevices == 0))
			return false;

		std::vector<VkPhysicalDevice> physicalDevice(numDevices);
		if (vkEnumeratePhysicalDevices(m_renderer->GetVulkanInstance(), &numDevices, physicalDevice.data()) != VK_SUCCESS)
			return false;

		std::multimap<int, VkPhysicalDevice> devicesMap;
		int score = 0;

		for (const auto& device : physicalDevice)
		{
			score = GetGPUScore(device);
			devicesMap.insert(std::make_pair(score, device));
		}

		// On récupère le meilleur GPU
		if (devicesMap.rbegin()->first > 0)
		{
			m_device->physicalDevice = devicesMap.rbegin()->second;
			return true;
		}

		return false;
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
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_window->GetSurface(), &queuePresentSupport[i]);
		
			if ((queueFamilyProperties[i].queueCount > 0) && (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				if (m_device->graphicsIndexFamily == UINT32_MAX)
					graphicsIndexFamily = i;

				if (queuePresentSupport[i])
				{
					m_device->graphicsIndexFamily = i;
					m_device->presentIndexFamily = i;
					
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

		m_device->graphicsIndexFamily = graphicsIndexFamily;
		m_device->presentIndexFamily = presentIndexFamily;

		return true;
	}

	//--------------------------------------------------------------------------

	bool Device::CreateLogicalDevice()
	{	
		std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo;
		std::vector<float> queuePriorities = { 1.0f };

		deviceQueueInfo.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			m_device->graphicsIndexFamily,
			static_cast<uint32_t>(queuePriorities.size()),
			queuePriorities.data()
		});

		if (m_device->graphicsIndexFamily != m_device->presentIndexFamily)
		{
			deviceQueueInfo.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				m_device->presentIndexFamily,
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

		if (vkCreateDevice(m_device->physicalDevice, &deviceInfo, nullptr, &m_device->logicalDevice) != VK_SUCCESS)
			return false;

		return true;
	}

	//--------------------------------------------------------------------------

	bool Device::IsExtensionAvailable()
	{
		uint32_t extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(m_device->physicalDevice, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensionsAvailable(extensionsCount);
		vkEnumerateDeviceExtensionProperties(m_device->physicalDevice, nullptr, &extensionsCount, extensionsAvailable.data());

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
}
