#pragma once

#ifndef ZDEVICE_HPP
#define ZDEVICE_HPP

#include <vector>

namespace Zx
{
	class ZDevice
	{
	public:
		struct Queue
		{
			int indexFamily = -1;
			int presentFamily = -1;

			inline bool IsValidQueue();
		};

		struct SwapChainDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> format;
			std::vector<VkPresentModeKHR> presentmode;
		};

		//----------------------------------------------

		ZDevice() = default;
		~ZDevice() = delete;

		static void InitializeDevice();
		static void UnInitializeDevice();

		static Queue GetQueueFamiliy(VkPhysicalDevice);

		static VkPhysicalDevice& GetPhysicalDevice();
		static VkDevice& GetLogicalDevice();
		static VkSwapchainKHR& GetSwapChain();
	
		static const std::vector<const char*>& GetDeviceExtension();

	private :
		static VkPhysicalDevice m_physicalDevice;
		static VkDevice m_logicalDevice;
		static VkQueue m_graphicsQueue;
		static VkQueue m_presentQueue;
		static const std::vector<const char*> m_deviceExtensions;
		static VkSwapchainKHR m_swapChain;
		static std::vector<VkImage> m_swapChainImage;
		static VkFormat m_swapChainImageFormat;
		static VkExtent2D m_swapChainExtent;


		static void FoundPhysicalDevice();
		static void CreateLogicalDevice();
		static void CreateSwapChain();

		static int GetGPUScore(VkPhysicalDevice);
		static bool IsDeviceExtensionSupport(VkPhysicalDevice);

		static VkSurfaceFormatKHR GetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
		static VkExtent2D GetSwapExtent(const VkSurfaceCapabilitiesKHR&);
		static VkPresentModeKHR GetSwapPresentMode(const std::vector<VkPresentModeKHR>&);

		static inline SwapChainDetails BuildSwapChainDetails(VkPhysicalDevice);

		static std::vector<VkImage> BuildVectorVkImage();
	};
}

#include "ZDevice.inl"

#endif //ZDEVICE_HPP