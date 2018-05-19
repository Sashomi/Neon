#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZVulkan.hpp>

namespace Zx
{
	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	/*
	@brief : Initialize ZVulkan API
	@brief : The name of the application
	*/
	void ZVulkan::Initialize(const ZString& applicationName)
	{
		#ifndef NDEBUG
		if (!IsLayersSupported())
			throw ZOperationFailed(__FILE__, "Validations layers require");
		#endif

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

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		#ifndef NDEBUG
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		#else
			createInfo.enabledLayerCount = 0;
		#endif

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		{
			throw ZOperationFailed(__FILE__, "Failed to create Vulkan instance");
		}
	}

	/*
	@brief : Destroy ZVulkan API
	*/
	void ZVulkan::UnInitialize()
	{
	#ifndef NDEBUG
		DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
	#endif

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

	/*
	@brief : Returns true if all the extensions return by "glfwGetRequiredInstanceExtensions" are supported, false otherwise
	*/
	bool ZVulkan::IsExtensionsSupported()
	{
		uint32_t extensionCount = 0;

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::vector<const char*> requireExtensions = GetRequiredExtensions();

		for (const char* require : requireExtensions)
		{
			for (const auto& extension : extensions) 
			{
				if (std::strcmp(require, extension.extensionName) == 0)
					return true;
			}
		}

		return false;
	}

	/*
	@brief : Returns true if all the layers are supported, false otherwise
	*/
	bool ZVulkan::IsLayersSupported()
	{
		uint32_t layerCount = 0;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const char* layerName : validationLayers)
		{
			for (const auto& layer : layers)
			{
				if (std::strcmp(layerName, layer.layerName) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}

	/*
	@brief : Active the debug callback propose by Vulkan
	@Note : Require the debug mode
	*/
	void ZVulkan::SetupDebugCallback()
	{
		#ifdef NDEBUG
			return;
		#endif

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = DebugCallback;

		if (CreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS) 
		{
			throw ZOperationFailed(__FILE__, "Failed to set up debug callback");
		}
	}

	//-------------------------Private method-------------------------
	std::vector<const char*> ZVulkan::GetRequiredExtensions()
	{
		uint32_t extensionsCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

		std::vector<const char*> requireExtensions(glfwExtensions, glfwExtensions + extensionsCount);

		#ifndef NDEBUG
			requireExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		#endif

		return requireExtensions;
	}

	VkResult ZVulkan::CreateDebugReportCallbackEXT(VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		if (func != nullptr) 
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else 
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void ZVulkan::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL ZVulkan::DebugCallback(VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData)
	{
		std::cerr << "Validation layer: " << msg << std::endl;

		return VK_FALSE;
	}

	VkDebugReportCallbackEXT ZVulkan::m_callback = nullptr;
	VkInstance ZVulkan::m_instance = nullptr;
}