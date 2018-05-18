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

		static VkInstance GetVulkanInstance();

		static void BeginScene();
	private :
		static std::vector<ZString> GetRequiredExtensions();

		static VkInstance m_instance;
	};
}

#endif //ZVULKAN_HPP