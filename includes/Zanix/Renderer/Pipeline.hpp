#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <vulkan/vulkan.h>
#include <memory>

namespace Zx
{
	class Device;
	class RenderPass;
	class SwapChain;

	template <class O, class F>
	class SmartDeleter;

	class Pipeline
	{
	public:
		Pipeline() = default;
		Pipeline(Device& device, RenderPass& renderPass, SwapChain& swapChain);
		Pipeline(const Pipeline& pipeline);

		~Pipeline();

		//Getter

		inline const VkPipeline& GetPipeline() const;

		Pipeline& operator=(Pipeline&&) noexcept;

	private:
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<RenderPass> m_renderPass;

		VkPipeline m_pipeline;
	private:
		bool CreatePipeline();

		SmartDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> CreatePipelineLayout();
	};
}

#include "Pipeline.inl"

#endif //PIPELINE_HPP