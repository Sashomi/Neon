#include <Neon/Core/File.hpp>
#include <Neon/Core/SmartDeleter.hpp>
#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/Device.hpp>
#include <Neon/Renderer/RenderPass.hpp>

namespace Zx
{
	/*
	@brief : Creates a render pass
	@param : The device of the application
	@param : The swapChain of the application
	*/
	RenderPass::RenderPass(Device& device, SwapChain& swapChain) : m_renderPass(VK_NULL_HANDLE)
	{
		m_device = std::make_shared<Device>(device);
		m_swapChain = std::make_shared<SwapChain>(swapChain);

		if (!CreateRenderPass())
			std::cout << "Failed to create a render pass" << std::endl;

		device = std::move(*m_device);
		swapChain = std::move(*m_swapChain);
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the RenderPass to copy
	*/
	RenderPass::RenderPass(const RenderPass& rend) : m_swapChain(rend.m_swapChain), m_device(rend.m_device), m_renderPass(rend.m_renderPass)
	{}

	/*
	@brief : Destroys a render pass
	*/
	RenderPass::~RenderPass()
	{
		if (m_renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_device->GetDevice()->logicalDevice, m_renderPass, nullptr);
			m_renderPass = VK_NULL_HANDLE;
		}
	}

	/*
	@brief : Creates a framebuffer
	@param : Returns true if the creation is a success, false otherwise
	*/
	bool RenderPass::CreateFramebuffer(VkFramebuffer& framebuffer, VkImageView imageView)
	{
		if (framebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(m_device->GetDevice()->logicalDevice, framebuffer, nullptr);
			framebuffer = VK_NULL_HANDLE;
		}

		VkFramebufferCreateInfo frameBufferCreateInfo =
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			nullptr,
			0,
			m_renderPass,
			1,
			&imageView,
			m_swapChain->GetSwapChain()->extent.width,
			m_swapChain->GetSwapChain()->extent.height,
			1
		};

		if (vkCreateFramebuffer(m_device->GetDevice()->logicalDevice, &frameBufferCreateInfo, nullptr, &framebuffer) != VK_SUCCESS)
		{
			std::cout << "Failed to create frame buffer" << std::endl;
			return false;
		}

		return true;
	}

	/*
	@brief : Assigns the render pass by move semantic
	@param : The renderPass to move
	@return : A reference to this
	*/
	RenderPass& RenderPass::operator=(RenderPass&& renderPass) noexcept
	{
		std::swap(m_device, renderPass.m_device);
		std::swap(m_renderPass, renderPass.m_renderPass);
		std::swap(m_swapChain, renderPass.m_swapChain);

		return (*this);
	}

	//---------------------------Private method---------------------------
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

		std::vector<VkSubpassDependency> subpassDependency =
		{
			{
				VK_SUBPASS_EXTERNAL,
				0,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VK_DEPENDENCY_BY_REGION_BIT
			},
			{
				0,
				VK_SUBPASS_EXTERNAL,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT
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
			static_cast<uint32_t>(subpassDependency.size()),
			subpassDependency.data()
		};

		if (vkCreateRenderPass(m_device->GetDevice()->logicalDevice, &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
		{
			std::cout << "Failed to create render pass" << std::endl;
			return false;
		}

		return true;
	}
}