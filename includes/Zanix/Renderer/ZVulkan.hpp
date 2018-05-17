#pragma once

#ifndef ZVULKAN_HPP
#define ZVULKAN_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class ZVulkan
	{
	public : 
		ZVulkan() = default;
		~ZVulkan() = delete;

		static void Initialize(const ZString& applicationName);
		static void Destroy();

		static bool IsInitialize();
		static VkInstance GetVulkanInstance();

		static void BeginScene();
	private :
		static VkInstance m_instance;
	};
}

#endif //ZVULKAN_HPP