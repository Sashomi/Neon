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
		RenderPass(Device& device, SwapChain& swapChain);
		RenderPass(const RenderPass& renderPass);

		~RenderPass();

		inline const VkRenderPass& GetRenderPass() const;
	
		bool CreateFramebuffer(VkFramebuffer& framebuffer, VkImageView imageView);

		RenderPass& operator=(RenderPass&&) noexcept;

	private:
		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<Device> m_device;

		VkRenderPass m_renderPass;
	
	private:
		bool CreateRenderPass();
	};
}

#include "RenderPass.inl"

#endif //RENDERPASS_HPP