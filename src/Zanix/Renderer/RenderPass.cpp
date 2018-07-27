#include <Zanix/Core/Exception.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/RenderPass.hpp>

namespace Zx
{
	void RenderPass::CreateRenderPass()
	{

		VkAttachmentDescription attachmentDescription[] =
		{
			0,
			SwapChain::GetSwapChain()->format,
			VK_SAMPLE_COUNT_1_BIT, // nombre d'échantillon(s) de l'image
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};

		VkAttachmentReference colorAttachmentReference[] =
		{
			0,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		};

		VkSubpassDescription subpassDescription[] =
		{
			0,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			0,
			nullptr,
			1,
			colorAttachmentReference,
			nullptr,
			nullptr,
			0,
			nullptr
		};

		VkRenderPassCreateInfo renderPassCreateInfo =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			nullptr,
			0,
			1,
			attachmentDescription,
			1,
			subpassDescription,
			0,
			nullptr
		};

		if (vkCreateRenderPass(Device::GetDevices()->logicalDevice, &renderPassCreateInfo, nullptr, &s_renderPass->renderPass) != VK_SUCCESS)
			throw ZOperationFailed(__FILE__, "Failed to create a render pass");
	}

	void RenderPass::CreateFramebuffer()
	{
		const std::vector<VkImageView> &swapChainImage = SwapChain::GetSwapChain()->imageView;
		s_renderPass->framebuffer.resize(swapChainImage.size());

		for (std::size_t i = 0; i < s_renderPass->framebuffer.size(); i++)
		{
			VkFramebufferCreateInfo frameBufferCreateInfo =
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				nullptr,
				0,
				s_renderPass->renderPass,
				1,
				&swapChainImage[i],
				300,
				300,
				1
			};

			if (vkCreateFramebuffer(Device::GetDevices()->logicalDevice, &frameBufferCreateInfo, nullptr, &s_renderPass->framebuffer[i]) != VK_SUCCESS)
				throw ZOperationFailed(__FILE__, "Failed to create framebuffer");
		}
	}

	std::shared_ptr<RenderPass::RenderPasss> RenderPass::s_renderPass = std::make_shared<RenderPass::RenderPasss>();
}