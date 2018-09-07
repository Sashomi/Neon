#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class SwapChain;
	class Window;
	class Renderer;
	
	struct RenderingResourcesData;

	class Device
	{
		struct Devices;

	public:
		Device() = default;
		Device(Renderer& renderer, SwapChain& swapChain, Window& window, std::vector<RenderingResourcesData>& renderingResources);
		Device(const Device& device);
		Device(Device&& device) noexcept;

		~Device();

		bool CreateDevice();

		//Getters and Setters

		inline const std::shared_ptr<Devices>& GetDevice() const;

		inline void SetSwapChain(const SwapChain&);
		inline void SetWindow(const Window&);
		inline void SetRenderer(const Renderer&);

		Device& operator=(Device&& device) noexcept;

	private:
		std::shared_ptr<Devices> m_device;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<std::vector<RenderingResourcesData>> m_renderingResources;

		struct Devices
		{
			inline Devices() : logicalDevice(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), graphicsIndexFamily(UINT32_MAX),
				presentIndexFamily(UINT32_MAX), graphicsQueue(VK_NULL_HANDLE), presentQueue(VK_NULL_HANDLE)
			{}

			VkDevice logicalDevice;
			VkPhysicalDevice physicalDevice;
			uint32_t graphicsIndexFamily;
			uint32_t presentIndexFamily;
			VkQueue graphicsQueue;
			VkQueue presentQueue;
		};

	private:
		bool CreateLogicalDevice();
		bool FoundPhysicalDevice();
		bool CheckFamilyQueue(const VkPhysicalDevice& device);
		bool IsExtensionAvailable();

		void GetDeviceQueue();

		int GetGPUScore(const VkPhysicalDevice& device);
	};
}

#include "Device.inl"

#endif //DEVICE_HPP