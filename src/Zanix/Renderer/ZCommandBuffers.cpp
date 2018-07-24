#include <vulkan/vulkan.h>

#include <Zanix/Core/ZException.hpp>
#include <Zanix/Renderer/ZDevice.hpp>
#include <Zanix/Renderer/ZSwapChain.hpp>
#include <Zanix/Renderer/ZCommandBuffers.hpp>

namespace Zx
{
	/*
	@brief : Creates command buffers
	*/
	void ZCommandBuffers::CreateCommandBuffers()
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			0,
			ZDevice::GetPresentIndexFamilyQueue()
		};

		if (vkCreateCommandPool(ZDevice::GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &s_commandPool) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create command pool");

		uint32_t imageCount = 0;
		if (vkGetSwapchainImagesKHR(ZDevice::GetLogicalDevice(), ZSwapChain::GetSwapChain(), &imageCount, nullptr) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Could not get the number of swap chain images");

		s_presentQueueCommandBuffers.resize(imageCount);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			s_commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			imageCount
		};

		if (vkAllocateCommandBuffers(ZDevice::GetLogicalDevice(), &commandBufferAllocateInfo, &s_presentQueueCommandBuffers[0]) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to allocate command buffers");

		if (!RecordCommandsBuffers())
			throw ZOperationFailed(__FILE__, "Failed to record command buffers");
	}

	/*
	@brief : Destroys ressources about the command buffers and the command pool
	*/
	void ZCommandBuffers::DestroyRessources()
	{
		if (ZDevice::GetLogicalDevice() != VK_NULL_HANDLE) {
			vkDeviceWaitIdle(ZDevice::GetLogicalDevice());

			if ((ZCommandBuffers::GetPresentQueueCommandBuffers().size() > 0) && (ZCommandBuffers::GetPresentQueueCommandBuffers().data() != VK_NULL_HANDLE)) {
				vkFreeCommandBuffers(ZDevice::GetLogicalDevice(), ZCommandBuffers::GetCommandPool(), static_cast<uint32_t>(ZCommandBuffers::GetPresentQueueCommandBuffers().size()), ZCommandBuffers::GetPresentQueueCommandBuffers().data());
				std::vector<VkCommandBuffer> cmdBuf = ZCommandBuffers::GetPresentQueueCommandBuffers();
				cmdBuf.clear();
			}

			if (ZCommandBuffers::GetCommandPool() != VK_NULL_HANDLE) {
				vkDestroyCommandPool(ZDevice::GetLogicalDevice(), ZCommandBuffers::GetCommandPool(), nullptr);
				VkCommandPool cmd = ZCommandBuffers::GetCommandPool();
				cmd = VK_NULL_HANDLE;
			}
		}
	}

	/*
	@brief : Gets the presentation queue of command buffers
	@param : The index of the image in the queue
	*/
	const VkCommandBuffer& ZCommandBuffers::GetPresentQueueCommandBuffers(uint32_t imageIndex)
	{
		return s_presentQueueCommandBuffers[imageIndex];
	}

	/*
	@brief : Gets the presentation queue of command buffers
	*/
	const std::vector<VkCommandBuffer>& ZCommandBuffers::GetPresentQueueCommandBuffers()
	{
		return s_presentQueueCommandBuffers;
	}

	/*
	@brief : Gets the command pool
	*/
	const VkCommandPool& ZCommandBuffers::GetCommandPool()
	{
		return s_commandPool;
	}

	//-------------------------Private method-------------------------

	bool ZCommandBuffers::RecordCommandsBuffers()
	{
		uint32_t image_count = static_cast<uint32_t>(s_presentQueueCommandBuffers.size());

		std::vector<VkImage> swap_chain_images(image_count);
		if (vkGetSwapchainImagesKHR(ZDevice::GetLogicalDevice(), ZSwapChain::GetSwapChain(), &image_count, &swap_chain_images[0]) != VK_SUCCESS) {
			throw ZOperationFailed(__FILE__, "Could not get swap chain images");	
		}

		VkCommandBufferBeginInfo cmd_buffer_begin_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,                                     
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, 
			nullptr                                       
		};

		//La couleur orange clair
		VkClearColorValue clear_color = {
			{ 1.0f, 0.8f, 0.4f, 0.0f }
		};

		VkImageSubresourceRange image_subresource_range = {
			VK_IMAGE_ASPECT_COLOR_BIT,                   
			0,                                         
			1,                                          
			0,                                         
			1                                            
		};

		for (uint32_t i = 0; i < image_count; ++i) {
			VkImageMemoryBarrier barrierPresentToClear = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, 
				nullptr,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				ZDevice::GetPresentIndexFamilyQueue() ,
				ZDevice::GetPresentIndexFamilyQueue(),
				swap_chain_images[i],
				image_subresource_range
			};

			VkImageMemoryBarrier barrierClearToPresent = {
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				ZDevice::GetPresentIndexFamilyQueue(),
				ZDevice::GetPresentIndexFamilyQueue(),
				swap_chain_images[i],
				image_subresource_range
			};

			vkBeginCommandBuffer(s_presentQueueCommandBuffers[i], &cmd_buffer_begin_info);
			vkCmdPipelineBarrier(s_presentQueueCommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrierPresentToClear);

			vkCmdClearColorImage(s_presentQueueCommandBuffers[i], swap_chain_images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range);

			vkCmdPipelineBarrier(s_presentQueueCommandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrierClearToPresent);
			
			if (vkEndCommandBuffer(s_presentQueueCommandBuffers[i]) != VK_SUCCESS)
				throw ZOperationFailed(__FILE__, "Could not record command buffers");
		}

		return true;
	}

	//------------------------------------------------------------------------

	std::vector<VkCommandBuffer> ZCommandBuffers::BuildVectorCommandBuffers()
	{
		std::vector<VkCommandBuffer> commandBuffer;

		return commandBuffer;
	}

	VkCommandPool ZCommandBuffers::s_commandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> ZCommandBuffers::s_presentQueueCommandBuffers = ZCommandBuffers::BuildVectorCommandBuffers();
}