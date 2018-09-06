#ifndef TEST1_HPP
#define TEST1_HPP

#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Device;
	class SwapChain;
	class Renderer;
	class Pipeline;
	class RenderPass;
	class Window;
	class VertexBuffer;
	class ShaderModule;
	class Sync;
	class CommandBuffers;

	struct RenderingResourcesData;

	class Test1
	{
	public:
		Test1(const RenderPass&, const SwapChain&, const Pipeline&, const VertexBuffer&, const Device&, const Window&, const CommandBuffers&, const std::vector<RenderingResourcesData>&);

		bool RenderingLoop();

	private:
		bool PrepareFrame(VkCommandBuffer commandBuffer, const VkImageView& view, VkFramebuffer& framebuffer);
		void ChildClear();
		bool ChildOnWindowSizeChanged();
		bool OnWindowSizeChanged();
		bool Draw();

	private:
		std::shared_ptr<RenderPass> m_renderPass;
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Pipeline> m_pipeline;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<CommandBuffers> m_commandBuffers;
		std::shared_ptr<std::vector<RenderingResourcesData>> m_renderingResources;

	};
}

#endif //TEST1_HPP