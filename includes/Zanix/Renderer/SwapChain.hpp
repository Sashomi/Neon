#ifndef ZSWAPCHAIN_HPP
#define ZSWAPCHAIN_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class SwapChain
	{
	public:
		static bool CreateSwapChain();

		static const VkSwapchainKHR& GetSwapChain();
	private:
		static VkSwapchainKHR s_swapChain;
		static VkExtent2D s_swapChainExtent;
		static VkFormat s_swapChainImageFormat;
		static std::vector<VkImage> s_swapChainImages;
		static std::vector<VkImageView> s_swapChainImagesView;

		static bool s_isRenderAvailable;

	private:
		static void CreateSwapChainImageView();

		static uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		static VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);

		static std::vector<VkImage> BuildVectorVkImage();
		static std::vector<VkImageView> BuildVectorVkImageView();
	};
}

#endif //ZSWAPCHAIN_HPP