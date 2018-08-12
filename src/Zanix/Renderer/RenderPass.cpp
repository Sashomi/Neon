#include <Zanix/Core/File.hpp>
#include <Zanix/Core/SmartDeleter.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/RenderPass.hpp>

namespace Zx
{
	/*
	@brief : Constructor with the needed informations
	@param : The device of the application
	@param : The swapChain of the application
	*/
	RenderPass::RenderPass(const Device& device, const SwapChain& swapChain)
	{
		m_device = std::make_shared<Device>(device);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_renderPass = std::make_shared<RenderPasss>();
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the RenderPass to copy
	*/
	RenderPass::RenderPass(const RenderPass& rend) : m_swapChain(rend.m_swapChain), m_device(rend.m_device), m_renderPass(rend.m_renderPass)
	{}

	/*
	@brief : Creates a render pass
	@return : Returns true if the creation is a success, false otherwise
	*/
	bool RenderPass::CreateRenderPass()
	{
		VkAttachmentDescription attachmentDescription[] =
		{
			{
			0,
			m_swapChain->GetSwapChain()->format,
			VK_SAMPLE_COUNT_1_BIT, // nombre d'échantillon(s) de l'image
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			}
		};
		
		VkAttachmentReference colorAttachmentReference[] =
		{
			{
			0,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			}
		};

		VkSubpassDescription subpassDescription[] =
		{
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
			}
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

		if (vkCreateRenderPass(m_device->GetDevice()->logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass->renderPass) != VK_SUCCESS)
		{
			std::cout << "Failed to create render pass" << std::endl;
			return false;
		}

		return true;
	}

	/*
	@brief : Creates a framebuffer
	@param : Returns true if the creation is a success, false otherwise
	*/
	bool RenderPass::CreateFramebuffer()
	{
		m_renderPass->framebuffer.resize(m_swapChain->GetSwapChain()->imageView.size());

		for (std::size_t i = 0; i < m_swapChain->GetSwapChain()->imageView.size(); i++)
		{
			VkImageView attachments[] =
			{
				m_swapChain->GetSwapChain()->imageView[i]
			};

			VkFramebufferCreateInfo frameBufferCreateInfo =
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				nullptr,
				0,
				m_renderPass->renderPass,
				1,
				attachments,
				300,
				300,
				1
			};

			if (vkCreateFramebuffer(m_device->GetDevice()->logicalDevice, &frameBufferCreateInfo, nullptr, &m_renderPass->framebuffer[i]) != VK_SUCCESS)
			{
				std::cout << "Failed to create frame buffer" << std::endl;
				return false;
			}
		}

		return true;
	}

	/*
	@brief : Destroys a render pass
	*/
	void RenderPass::DestroyRenderPass()
	{
		vkDestroyRenderPass(m_device->GetDevice()->logicalDevice, m_renderPass->renderPass, nullptr);
		m_renderPass->renderPass = VK_NULL_HANDLE;
	}

	/*
	@brief : Destroys framebuffers
	*/
	void RenderPass::DestroyFrameBuffer()
	{
		for (size_t i = 0; i < m_renderPass->framebuffer.size(); ++i) {
			if (m_renderPass->framebuffer[i] != VK_NULL_HANDLE) {
				vkDestroyFramebuffer(m_device->GetDevice()->logicalDevice, m_renderPass->framebuffer[i], nullptr);
				m_renderPass->framebuffer[i] = VK_NULL_HANDLE;
			}
		}
	}
}