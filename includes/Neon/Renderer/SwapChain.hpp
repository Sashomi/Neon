#ifndef ZSWAPCHAIN_HPP
#define ZSWAPCHAIN_HPP

#include <vector>
#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Window;
	class Device;

	class SwapChain
	{
		struct SwapChains;

	public:
		SwapChain() = default;
		SwapChain(const Device& device, const Window& window);
		SwapChain(const SwapChain& swapChain);
		SwapChain(SwapChain&& swapChain) noexcept;

		~SwapChain();

		bool CreateSwapChain();
		
		//Getters and Setters

		inline bool IsRenderAvailable() const;
		inline const std::shared_ptr<SwapChains>& GetSwapChain() const;
		
		inline void SetDevice(const Device& device);
		inline void SetWindow(const Window& window);

		SwapChain& operator=(SwapChain&& swapChain) noexcept;

	private:
		std::shared_ptr<SwapChains> m_swapChain;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<Window> m_window;

		struct SwapChains
		{
			inline SwapChains() : swapChain(VK_NULL_HANDLE), extent({ 0, 0 }), format(VK_FORMAT_UNDEFINED), image(), imageView()
			{}

			VkSwapchainKHR swapChain;
			VkExtent2D extent;
			VkFormat format;
			std::vector<VkImage> image;
			std::vector<VkImageView> imageView;
		};

		bool m_isRenderAvailable;

	private:
		bool CreateSwapChainImageView();

		uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
	};
}

#include "SwapChain.inl"

#endif //ZSWAPCHAIN_HPP