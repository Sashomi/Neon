#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class ZDevice
	{
	public:
		ZDevice() = default;
		~ZDevice() = delete;

		static void InitDevice();
		static void DestroyDevice();

		static VkDevice GetLogicalDevice();
		static VkPhysicalDevice GetPhysicalDevice();

	private:
		static VkDevice s_logicalDevice;
		static VkPhysicalDevice s_physicalDevice;
		static uint32_t s_indexFamily;
		static uint32_t s_presentIndexFamily;
		static VkQueue s_graphicsQueue;
		static VkQueue s_presentQueue;
		static VkSemaphore s_renderingFinishedSemaphore;
		static VkSemaphore s_imageAvailableSemaphore;

	private:
		static void FoundPhysicalDevice();
		static int GetGPUScore(const VkPhysicalDevice& device);

		static void CreateLogicalDevice();
		static void CreateSemaphores();

		static bool CheckFamilyQueue(const VkPhysicalDevice& device);
		static void GetDeviceQueue();

		static bool IsExtensionAvailable();
	};
}

#endif //ZDEVICE_HPP