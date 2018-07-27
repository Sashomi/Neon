#ifndef ZSWAPCHAIN_HPP
#define ZSWAPCHAIN_HPP

#include <vector>
#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class SwapChain
	{
		struct SwapChains;

	public:
		static bool CreateSwapChain();

		static const std::shared_ptr<SwapChains>& GetSwapChain();
	private:

		static std::shared_ptr<SwapChains> s_swapChain;

		static bool s_isRenderAvailable;

		struct SwapChains
		{
			inline SwapChains() : swapChain(VK_NULL_HANDLE), extent({ 0, 0}), format(VK_FORMAT_UNDEFINED), image(), imageView()
			{
			}

			VkSwapchainKHR swapChain;
			VkExtent2D extent;
			VkFormat format;
			std::vector<VkImage> image;
			std::vector<VkImageView> imageView;
		};

	private:
		static void CreateSwapChainImageView();

		static uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		static VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
	};
}

#endif //ZSWAPCHAIN_HPP