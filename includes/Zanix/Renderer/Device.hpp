#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class SwapChain;
	class Window;
	class Renderer;

	class Device
	{
		struct Devices;

	public:
		Device() = default;
		Device(const Renderer& renderer, const SwapChain& swapChain, const Window& window);
		Device(const Device& device);
		Device(Device&& device) noexcept;

		~Device() = default;

		bool CreateDevice();
		bool CreateSemaphores();

		void DestroyDevice();

		void GetDeviceQueue();

		Device& operator=(Device&& device) noexcept;

	public:
		inline const std::shared_ptr<Devices>& GetDevice() const
		{
			return m_device;
		}

	private:
		std::shared_ptr<Devices> m_device;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<SwapChain> m_swapChain;

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
		bool FoundPhysicalDevice();
		int GetGPUScore(const VkPhysicalDevice& device);

		bool CreateLogicalDevice();

		bool CheckFamilyQueue(const VkPhysicalDevice& device);

		bool IsExtensionAvailable();
	};
}

#endif //ZDEVICE_HPP