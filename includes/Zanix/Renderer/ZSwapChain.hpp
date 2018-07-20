#ifndef ZSWAPCHAIN_HPP
#define ZSWAPCHAIN_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class ZSwapChain
	{
	public:
		static bool CreateSwapChain();
	private:
		static VkSwapchainKHR s_swapChain;

	private:
		static uint32_t GetSwapChainNumImages(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceFormatKHR GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats);
		static VkExtent2D GetSwapChainExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkImageUsageFlags GetSwapChainUsageFlags(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		static VkPresentModeKHR GetSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
	};
}

#endif //ZSWAPCHAIN_HPP