#pragma once

#ifndef ZVULKAN_HPP
#define ZVULKAN_HPP

#include <vector>
#include <vulkan/vulkan.h>

// @Alexander Overvoorde

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
		

		static VkInstance& GetVulkanInstance();
		static VkSurfaceKHR& GetWindowSurface();
		static ZWindow& GetZWindow();

		static const std::vector<const char*>& GetValidationsLayers();
		
	private :
		//Attributes
		static VkInstance s_instance;
		static VkDebugReportCallbackEXT s_callback;
		static ZWindow s_window;
		static VkSurfaceKHR s_surface;
		static const std::vector<const char*> s_validationLayers;

		//Private methods
		static void CreateInstance(const ZString&);
		static void CreateWindowSurface();
		static void SetupDebugCallback();
		static std::vector<const char*> GetRequiredExtensions();
		static bool IsLayersSupported();

		//Private methods require for active the Vulkan's debug callback
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	};
}

#endif //ZVULKAN_HPP