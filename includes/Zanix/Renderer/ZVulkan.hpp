#pragma once

#ifndef ZVULKAN_HPP
#define ZVULKAN_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class ZString;
	class ZWindow;

	class ZVulkan
	{
	public : 
		ZVulkan() = default;
		~ZVulkan() = delete;

		static void Initialize(const ZWindow&);
		static void UnInitialize();

		static bool IsInitialize();
		static bool IsExtensionsSupported();
		static bool IsLayersSupported();

		static VkInstance GetVulkanInstance();
		static VkSurfaceKHR GetWindowSurface();

		static const std::vector<const char*>& GetValidationsLayers();
		
	private :
		//Attributes
		static VkInstance m_instance;
		static VkDebugReportCallbackEXT m_callback;
		static ZWindow m_window;
		static VkSurfaceKHR m_surface;
		static const std::vector<const char*> m_validationLayers;


		//Private methods
		static void CreateInstance(const ZString&);
		static void CreateWindowSurface();
		static void SetupDebugCallback();
		static std::vector<const char*> GetRequiredExtensions();


		//Private methods require for active the Vulkan's debug callback
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	};
}

#endif //ZVULKAN_HPP