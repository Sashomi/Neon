#include <vector>

#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/ZUtils.hpp>
#include <Zanix/Renderer/Window.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Renderer.hpp>


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

	/*
	@brief : Initialize Vulkan API
	*/
	void Renderer::Initialize()
	{
		CreateInstance();
		Window::CreatePresentationSurface();
		Device::CreateDevice();
		SwapChain::CreateSwapChain();
	}

	/*
	@brief : Destroys command buffers ressources, the device, and destroys vulkan instance
	*/
	void Renderer::Destroy()
	{
		CommandBuffers::DestroyRessources();
		Device::DestroyDevice();
		DestroyInstance();
	}

	/*
	@brief : Returns the vulkan instance
	*/
	const VkInstance& Renderer::GetVulkanInstance()
	{
		return s_instance;
	}

	//-------------------------Private method-------------------------

	void Renderer::CreateInstance()
	{
		if (!IsExtensionAvailable())
			throw ZOperationFailed(__FILE__, "Extensions are not available.");

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

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &s_instance) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a Vulkan Instance.");
	}

	//-------------------------------------------------------------------------

	bool Renderer::IsExtensionAvailable()
	{
		uint32_t extensionsCount = 0;
		if ((vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr) != VK_SUCCESS) || (extensionsCount == 0))
			throw ZOperationFailed(__FILE__, "Failed to enumerate availabe extensions.");

		std::vector<VkExtensionProperties> extensionsProperties(extensionsCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensionsProperties.data()) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to enumerate availabe extensions - data.");

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

	//-------------------------------------------------------------------------

	void Renderer::DestroyInstance()
	{
		if (s_instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(s_instance, nullptr);
		}
	}

	//-------------------------------------------------------------------------

	VkInstance Renderer::s_instance = VK_NULL_HANDLE;
}