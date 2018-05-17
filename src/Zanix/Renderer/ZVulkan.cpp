#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZVulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Zx
{
	/*
	@brief : Initialize ZVulkan API
	@brief : The name of the application
	*/
	void ZVulkan::Initialize(const ZString& applicationName)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = applicationName.GetPtr();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Zanix Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		{
			throw ZOperationFailed(__FILE__, "Failed to create Vulkan instance");
		}
	}

	/*
	@brief : Destroy ZVulkan API
	*/
	void ZVulkan::Destroy()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	/*
	@brief : Return true if the vulkan API is initialize, false otherwise
	*/
	bool ZVulkan::IsInitialize()
	{
		return (m_instance);
	}

	/*
	@brief : Return the vulkan instance
	@note : Require the vulkan API is initialize
	*/
	VkInstance ZVulkan::GetVulkanInstance()
	{
		ZAssert(IsInitialize(), "Vulkan api not initiallize");
			
		return (m_instance);
	}

	VkInstance ZVulkan::m_instance = nullptr;
}