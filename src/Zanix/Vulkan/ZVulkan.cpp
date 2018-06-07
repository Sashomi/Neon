#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Zanix/Vulkan/ZDevice.hpp>
#include <Zanix/Vulkan/ZSwapChain.hpp>
#include <Zanix/Graphics/ZWindow.hpp>
#include <Zanix/Core/ZException.hpp>
#include <Zanix/Vulkan/ZVulkan.hpp>

namespace Zx
{
	const std::vector<const char*> ZVulkan::s_validationLayers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	/*
	@brief : Initialize ZVulkan API
	@param : The name of the application
	@note : Requires to create a ZWindow before because this is influence the device choice
	*/
	void ZVulkan::Initialize(const ZWindow& window)
	{
		if (IsInitialize())
			ZOperationFailed(__FILE__, "ZVulkan is already initialize");
		
		if (!window.GetWindow())
			ZOperationFailed(__FILE__, "The window do not be a nullptr");

		s_window = window;

		CreateInstance(window.GetWindowTitle());
		SetupDebugCallback();
		CreateWindowSurface();
		ZDevice::InitializeDevice();
		ZSwapChain::InitializeSwapChain();
	}

	/*
	@brief : Destroy ZVulkan API
	*/
	void ZVulkan::UnInitialize()
	{
		ZSwapChain::UnInitializeSwapChain();
		ZDevice::UnInitializeDevice();

		DestroyDebugReportCallbackEXT(s_instance, s_callback, nullptr);
		vkDestroySurfaceKHR(s_instance, s_surface, nullptr);
		vkDestroyInstance(s_instance, nullptr);
	}

	/*
	@brief : Return true if the vulkan API is initialize, false otherwise
	*/
	bool ZVulkan::IsInitialize()
	{
		return (s_instance);
	}

	/*
	@brief : Return the vulkan instance
	@note : Require the vulkan API is initialize
	*/
	VkInstance& ZVulkan::GetVulkanInstance()
	{
		ZAssert(IsInitialize(), "Vulkan api not initiallize");
		
		return (s_instance);
	}

	/*
	@brief : Returns the window surface
	*/
	VkSurfaceKHR& ZVulkan::GetWindowSurface()
	{
		return (s_surface);
	}

	/*
	@brief : Returns the ZWindow
	*/
	ZWindow& ZVulkan::GetZWindow()
	{
		return (s_window);
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
			bool found = false;
			for (const auto& extension : extensions)
				if (std::strcmp(require, extension.extensionName) == 0)
				{
					found = true;
					break;
				}

			if (!found)
				return false;
		}

		return true;
	}
	

	/*
	@brief : Returns the requires validation layers
	*/
	const std::vector<const char*>& ZVulkan::GetValidationsLayers()
	{
		return s_validationLayers;
	}


	//-------------------------Private method-------------------------

	void ZVulkan::CreateInstance(const ZString& applicationName)
	{
		#ifdef ZDEBUG
			if (!IsLayersSupported())
				throw ZOperationFailed(__FILE__, "Validations layers require");
		#endif

		//Application information
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = applicationName.GetPtr();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Zanix Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		//Instance information
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		#ifdef ZDEBUG
			createInfo.enabledLayerCount = static_cast<uint32_t>(s_validationLayers.size());
			createInfo.ppEnabledLayerNames = s_validationLayers.data();
		#else
			createInfo.enabledLayerCount = 0;
		#endif

		if (vkCreateInstance(&createInfo, nullptr, &s_instance) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create Vulkan instance");
	}

	//----------------------------------------------------------------

	void ZVulkan::CreateWindowSurface()
	{
		if (glfwCreateWindowSurface(s_instance, s_window.GetWindow(), nullptr, &s_surface) != VK_SUCCESS)
			ZOperationFailed(__FILE__, "Failed to create window surface");
	}

	//----------------------------------------------------------------

	void ZVulkan::SetupDebugCallback()
	{
		#ifndef ZDEBUG
				return;
		#endif
		
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = DebugCallback;
	
		if (CreateDebugReportCallbackEXT(s_instance, &createInfo, nullptr, &s_callback) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to set up debug callback");
	}

	//----------------------------------------------------------------

	std::vector<const char*> ZVulkan::GetRequiredExtensions()
	{
		uint32_t extensionsCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

		std::vector<const char*> requireExtensions(glfwExtensions, glfwExtensions + extensionsCount);

		#ifdef ZDEBUG
			requireExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		#endif

		return requireExtensions;
	}

	//----------------------------------------------------------------

	bool ZVulkan::IsLayersSupported()
	{
		uint32_t layerCount = 0;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const char* layerName : s_validationLayers)
		{
			bool found = false;
			for (const auto& layer : layers)
				if (strcmp(layerName, layer.layerName) == 0)
				{
					found = true;
					break;
				}

			if (!found)
				return false;
		}

		return true;
	}

	//----------------------------------------------------------------

	VkResult ZVulkan::CreateDebugReportCallbackEXT(VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		if (func)
			return func(instance, pCreateInfo, pAllocator, pCallback);
		else 
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	//----------------------------------------------------------------

	void ZVulkan::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) 
	{
		#ifndef ZDEBUG
			return;
		#endif

		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

		if (!func)
			func(instance, callback, pAllocator);
	}

	//----------------------------------------------------------------

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

	VkDebugReportCallbackEXT ZVulkan::s_callback = VK_NULL_HANDLE;
	VkInstance ZVulkan::s_instance = VK_NULL_HANDLE;
	ZWindow ZVulkan::s_window;
	VkSurfaceKHR ZVulkan::s_surface = VK_NULL_HANDLE;
}