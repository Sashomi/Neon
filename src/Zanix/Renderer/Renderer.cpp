#include <vector>

#include <Zanix/Core/Exception.hpp>
#include <Zanix/ZUtils.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>
#include <Zanix/Renderer/Renderer.hpp>

#ifdef ZDEBUG
	#define VALIDATIONS_LAYERS
#else
	#undef VALIDATIONS_LAYERS
#endif

namespace Zx
{
	std::vector<const char*> extensions =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
		#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VK_KHR_XCB_SURFACE_EXTENSION_NAME
		#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			VK_KHR_XLIB_SURFACE_EXTENSION_NAME
		#endif
	};

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	//------------------------------------------------

	/*
	@brief : Constructor with the needed informations
	@param : The device of the application
	*/
	Renderer::Renderer(const Device& device) : m_instance(VK_NULL_HANDLE), m_callback(VK_NULL_HANDLE)
	{
		m_device = std::make_shared<Device>(device);
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the Renderer to copy
	*/
	Renderer::Renderer(const Renderer& renderer)
	{
		m_instance = renderer.m_instance;
		m_device = renderer.m_device;
		m_callback = renderer.m_callback;
	}
	
	/*
	@brief : Initialize Vulkan API
	@return : Returns true if the initialization of the vulkan API is a success, false otherwise
	*/
	bool Renderer::Initialize(Window& window, Device& device, SwapChain& swapChain)
	{
		if (!(CreateInstance()))
			return false;

		if (!SetupDebugCallback())
			return false;

		window.SetVkInstance(m_instance);
			
		if (!(window.CreatePresentationSurface()))
			return false;

		Device d(*this, swapChain, window);
		device = std::move(d);
			
		if (!(device.CreateDevice()))
			return false;

		SwapChain swap(device, window);
		swapChain = std::move(swap);

		if (!(swapChain.CreateSwapChain()))
			return false;

		return true;
	}

	/*
	@brief : Destroys the vulkan instance
	*/
	void Renderer::DestroyRenderer()
	{
		DestroyInstance();
	}

	//-------------------------Private method-------------------------

	bool Renderer::CreateInstance()
	{
		#ifdef VALIDATIONS_LAYERS
			if (!CheckValidationLayerSupport())
			{
				std::cout << "Validation layers are not support" << std::endl;
				return false;
			}

			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		#endif

		if (!IsExtensionAvailable())
			return false;

		VkApplicationInfo applicationInfo =
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			nullptr,
			"Zanix Engine",
			VK_MAKE_VERSION(1, 0, 0),
			"Zanix Engine",
			VK_MAKE_VERSION(1, 0, 0),
			VK_API_VERSION_1_1
		};

		VkInstanceCreateInfo instanceCreateInfo =
		{
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			nullptr,
			0,
			&applicationInfo,
			0,
			nullptr,
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		};

		#ifdef VALIDATIONS_LAYERS
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		#endif

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
			return false;

		return true;
	}

	//-------------------------------------------------------------------------

	bool Renderer::IsExtensionAvailable()
	{
		uint32_t extensionsCount = 0;
		if ((vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr) != VK_SUCCESS) || (extensionsCount == 0))
			return false;

		std::vector<VkExtensionProperties> extensionsProperties(extensionsCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionsProperties.data()) != VK_SUCCESS)
			return false;

		for (const char* extensionName : extensions)
		{
			for (const auto& extensionProperties : extensionsProperties)
			{
				if (std::strcmp(extensionProperties.extensionName, extensionName) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}

	//-----------------------------------------------------------------------

	bool Renderer::CheckValidationLayerSupport()
	{
		uint32_t validationLayerCount = 0;
		vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

		std::vector<VkLayerProperties> valLayers(validationLayerCount);
		vkEnumerateInstanceLayerProperties(&validationLayerCount, valLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;
			
			for (const auto& layers : valLayers)
			{
				if (std::strcmp(layerName, layers.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------

	void Renderer::DestroyInstance()
	{
		#ifdef VALIDATIONS_LAYERS
				DestroyDebugCallback(m_instance, m_callback, nullptr);
		#endif

		if (m_instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(m_instance, nullptr);
		}
	}

	//-------------------------------------------------------------------------

	void Renderer::Destroy()
	{
		DestroyInstance();
	}

	//-------------------------------------------------------------------------

	bool Renderer::SetupDebugCallback()
	{
		#ifndef VALIDATIONS_LAYERS 
				return;
		#endif

		VkDebugReportCallbackCreateInfoEXT debugReportCallbackInfo =
		{
			VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
			nullptr,
			VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
			DebugCallback,
			nullptr
		};

		if (CreateDebugReportCallbackEXT(m_instance, &debugReportCallbackInfo, nullptr, &m_callback) != VK_SUCCESS)
		{
			std::cout << "Failed to create debug report callback" << std::endl;
			return false;
		}
		
		return true;
	}

	VkResult Renderer::CreateDebugReportCallbackEXT(const VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
	{
		auto function = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		if (function != nullptr)
			return function(instance, pCreateInfo, pAllocator, pCallback);

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void Renderer::DestroyDebugCallback(const VkInstance& instance, const VkDebugReportCallbackEXT& callback, const VkAllocationCallbacks* pAllocator)
	{
		auto function = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		if (function != nullptr)
			function(instance, callback, pAllocator);
	}

	//-------------------------------------------------------------------------

	VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData) {

		std::cerr << "validation layer: " << msg << std::endl;

		return VK_FALSE;
	}
}