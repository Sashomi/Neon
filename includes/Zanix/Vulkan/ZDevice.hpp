#pragma once

#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class ZDevice
	{
	public:
		struct Queue
		{
			int indexFamily = -1;
			int presentFamily = -1;

			inline bool IsValidQueue();
		};

		//----------------------------------------------

		ZDevice() = default;
		~ZDevice() = delete;

		static void InitializeDevice();
		static void UnInitializeDevice();

		static Queue GetQueueFamiliy(VkPhysicalDevice);

		static VkPhysicalDevice& GetPhysicalDevice();
		static VkDevice& GetLogicalDevice();
	
		static const std::vector<const char*>& GetDeviceExtension();

	private :
		static VkPhysicalDevice s_physicalDevice;
		static VkDevice s_logicalDevice;
		static VkQueue s_graphicsQueue;
		static VkQueue s_presentQueue;
		static const std::vector<const char*> s_deviceExtensions;

		static void FoundPhysicalDevice();
		static void CreateLogicalDevice();

		static int GetGPUScore(VkPhysicalDevice);
		static bool IsDeviceExtensionSupport(VkPhysicalDevice);
	};
}

#include "ZDevice.inl"

#endif //ZDEVICE_HPP