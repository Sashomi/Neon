#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Device
	{
		struct Devices;

	public:
		Device() = default;
		~Device() = delete;

		static void CreateDevice();
		static void DestroyDevice();

		static void GetDeviceQueue();

		static const std::shared_ptr<Devices>& GetDevices();

	private:
		
		static std::shared_ptr<Devices> s_devices;

		struct Devices
		{
			inline Devices() : logicalDevice(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), graphicsIndexFamily(UINT32_MAX),
				presentIndexFamily(UINT32_MAX), graphicsQueue(VK_NULL_HANDLE), presentQueue(VK_NULL_HANDLE),
				renderingFinishedSemaphore(VK_NULL_HANDLE), imageAvailableSemaphore(VK_NULL_HANDLE)
			{}

			VkDevice logicalDevice;
			VkPhysicalDevice physicalDevice;
			uint32_t graphicsIndexFamily;
			uint32_t presentIndexFamily;
			VkQueue graphicsQueue;
			VkQueue presentQueue;
			VkSemaphore renderingFinishedSemaphore;
			VkSemaphore imageAvailableSemaphore;
		};

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