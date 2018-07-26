#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class Device
	{
	public:
		Device() = default;
		~Device() = delete;

		static void CreateDevice();
		static void DestroyDevice();

		static void GetDeviceQueue();

		static const VkDevice& GetLogicalDevice();
		static const VkPhysicalDevice& GetPhysicalDevice();
		static const VkSemaphore& GetImageAvailableSemaphore();
		static const VkSemaphore& GetRenderingFinishedSemaphore();
		static const VkQueue& GetPresentQueue();
		static const VkQueue& GetGraphicsQueue();

		static uint32_t GetPresentIndexFamilyQueue();
		static uint32_t GetGraphicsIndexFamilyQueue();

	private:
		static VkDevice s_logicalDevice;
		static VkPhysicalDevice s_physicalDevice;
		static uint32_t s_graphicsIndexFamily;
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

		static bool IsExtensionAvailable();
	};
}

#endif //ZDEVICE_HPP