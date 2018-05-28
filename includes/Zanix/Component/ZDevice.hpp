#pragma once

#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <vector>

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

		ZDevice() = default;
		~ZDevice() = delete;

		static void FoundPhysicalDevice();
		static void CreateLogicalDevice();

		static Queue GetQueueFamiliy(VkPhysicalDevice);

		static VkPhysicalDevice GetPhysicalDevice();
		static VkDevice	GetLogicalDevice();

		static const std::vector<const char*>& GetDeviceExtension();

	private :
		static VkPhysicalDevice m_physicalDevice;
		static VkDevice m_logicalDevice;
		static VkQueue m_graphicsQueue;
		static VkQueue m_presentQueue;
		static const std::vector<const char*> m_deviceExtensions;

		static int GetGPUScore(VkPhysicalDevice);
		static bool IsDeviceExtensionSupport(VkPhysicalDevice);
	};
}

#include "ZDevice.inl"

#endif //ZDEVICE_HPP