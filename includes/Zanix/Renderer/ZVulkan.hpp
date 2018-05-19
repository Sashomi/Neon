#pragma once

#ifndef ZVULKAN_HPP
#define ZVULKAN_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class ZVulkan
	{
	public : 
		ZVulkan() = default;
		~ZVulkan() = delete;

		static void Initialize(const ZString& applicationName);
		static void UnInitialize();

		static bool IsInitialize();

		static bool IsExtensionsSupported();
		static bool IsLayersSupported();

		static VkInstance GetVulkanInstance();

		static void SetupDebugCallback();
		
	private :
		// Method requires for active the debug callback propose by Vulkan
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
		static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

		static std::vector<const char*> GetRequiredExtensions();

		static VkInstance m_instance;
		static VkDebugReportCallbackEXT m_callback;
	};
}

#endif //ZVULKAN_HPP