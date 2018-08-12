#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

#include <memory>

namespace Zx
{
	class Device;
	class SwapChain;

	class RenderPass
	{
		struct RenderPasss;

	public:
		RenderPass() = default;
		RenderPass(const Device& device, const SwapChain& swapChain);
		RenderPass(const RenderPass& renderPass);

		~RenderPass() = default;

		bool CreateRenderPass();
		bool CreateFramebuffer();

		void DestroyRenderPass();
		void DestroyFrameBuffer();

	public:
		inline const std::shared_ptr<RenderPasss>& GetRenderPass() const
		{
			return m_renderPass;
		}

	private:
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Device> m_device;
		std::shared_ptr<RenderPasss> m_renderPass;

		struct RenderPasss
		{
			inline RenderPasss() : renderPass(VK_NULL_HANDLE), framebuffer()
			{}

			VkRenderPass renderPass;
			std::vector<VkFramebuffer> framebuffer;
		};
	};
}

#endif //RENDERPASS_HPP