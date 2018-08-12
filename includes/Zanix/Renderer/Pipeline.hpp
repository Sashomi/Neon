#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <vulkan/vulkan.h>
#include <memory>

namespace Zx
{
	class Device;
	class RenderPass;
	class SwapChain;

	class Pipeline
	{
	public:
		Pipeline() = default;
		Pipeline(const Device& device, const RenderPass& renderPass, const SwapChain& swapChain);
		Pipeline(const Pipeline& pipeline);

		~Pipeline() = default;

		bool CreatePipeline();
		void DestroyPipeline();

	public:
		inline const VkPipeline& GetPipeline() const
		{
			return m_pipeline;
		}

	private:
		VkPipeline m_pipeline;
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<RenderPass> m_renderPass;
	private:
		VkPipelineLayout CreatePipelineLayout();
	};
}

#endif //PIPELINE_HPP