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

	class CommandBuffers
	{
	public:
		CommandBuffers() = default;
		CommandBuffers(const Device& device, const SwapChain& swapChain, const Pipeline& pipeline, const RenderPass& renderPass);
		CommandBuffers(const CommandBuffers& commandBuffers);

		~CommandBuffers() = default;

		bool CreateCommandBuffers();
		bool RecordCommandBuffers();

		void DestroyRessources();
		void DestroyCommandPool();
		void FreeCommandBuffers();
		
	public:
		inline const VkCommandBuffer& GetCommandBuffers(uint32_t imageIndex) const
		{
			return m_commandBuffers[imageIndex];
		}

		inline const std::vector<VkCommandBuffer>& GetCommandBuffers() const
		{
			return m_commandBuffers;
		}

		inline const VkCommandPool& GetCommandPool() const
		{
			return m_commandPool;
		}

	private:
		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Pipeline> m_pipeline;
		std::shared_ptr<RenderPass> m_renderPass;

	private:
		bool CreateCommandPool(uint32_t indexFamily, VkCommandPool* commandPool);

		bool AllocateCommandBuffers(uint32_t imageCount, VkCommandPool commandPool, VkCommandBuffer* commandBuffer);

		std::vector<VkCommandBuffer> BuildVectorCommandBuffers();
	};
}

#endif //ZCOMMANDBUFFERS_HPP