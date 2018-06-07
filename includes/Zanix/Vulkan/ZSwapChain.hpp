#pragma once

#ifndef ZSWAPCHAIN_HPP
#define ZSWAPCHAIN_HPP

#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class ZSwapChain
	{
	public:
		struct SwapChainDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> format;
			std::vector<VkPresentModeKHR> presentmode;
		};

		static void InitializeSwapChain();
		static void UnInitializeSwapChain();

		static VkSwapchainKHR& GetSwapChain();

		static inline SwapChainDetails BuildSwapChainDetails(VkPhysicalDevice);

	private:
		static VkSwapchainKHR s_swapChain;

		static VkExtent2D s_swapChainExtent;
		static VkFormat s_swapChainImageFormat;
		static std::vector<VkImage> s_swapChainImage;
		static std::vector<VkImageView> s_swapChainImageView;
		
		static void CreateSwapChain();
		static void CreateImageView();

		static VkSurfaceFormatKHR GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		static VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR&);
		static VkPresentModeKHR GetSwapPresentMode(const std::vector<VkPresentModeKHR>&);

		static std::vector<VkImage> BuildVectorVkImage();
		static std::vector<VkImageView> BuildVectorVkImageView();
	};
}

#include "ZSwapChain.inl"

#endif //ZSWAPCHAIN_HPP