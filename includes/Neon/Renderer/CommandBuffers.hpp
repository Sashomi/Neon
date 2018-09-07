#ifndef ZCOMMANDBUFFERS_HPP
#define ZCOMMANDBUFFERS_HPP

#include <vector>
#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Device;
	class SwapChain;
	class RenderPass;
	class Pipeline;

	struct RenderingResourcesData
	{
		VkCommandBuffer commandBuffer;
		VkFence fence;
		VkFramebuffer framebuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore finishedRenderingSemaphore;

		inline RenderingResourcesData() : commandBuffer(VK_NULL_HANDLE), fence(VK_NULL_HANDLE), framebuffer(VK_NULL_HANDLE)
			, imageAvailableSemaphore(VK_NULL_HANDLE), finishedRenderingSemaphore(VK_NULL_HANDLE)
		{}
	};

	class CommandBuffers
	{
	public:
		CommandBuffers() = default;
		CommandBuffers(Device& device, SwapChain& swapChain, Pipeline& pipeline, RenderPass& renderPass);
		CommandBuffers(const CommandBuffers& commandBuffers);

		~CommandBuffers();
		
		inline const VkCommandPool& GetCommandPool() const;
		inline std::vector<RenderingResourcesData>& GetRenderingResources();

	private:
		std::vector<RenderingResourcesData> m_renderingResources;

		VkCommandPool m_commandPool;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Pipeline> m_pipeline;
		std::shared_ptr<RenderPass> m_renderPass;

	private:
		bool CreateCommandBuffers();
		bool CreateCommandPool(uint32_t indexFamily, VkCommandPool* commandPool);

		bool AllocateCommandBuffers(uint32_t imageCount, VkCommandPool commandPool, VkCommandBuffer* commandBuffer);
	};
}

#include "CommandBuffers.inl"

#endif //ZCOMMANDBUFFERS_HPP