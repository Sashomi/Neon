#include <iostream>

#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/RenderPass.hpp>
#include <Zanix/Renderer/Pipeline.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>

namespace Zx
{
	/*
	@brief : Constructor with the needed informations
	@param : The device of the application
	@param  The swapChain of the application
	@param : The renderPass of the application
	*/
	CommandBuffers::CommandBuffers(const Device& device, const SwapChain& swapChain, const Pipeline& pipeline, const RenderPass& renderPass)
	{
		m_commandBuffers = BuildVectorCommandBuffers();
		m_device = std::make_shared<Device>(device);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_pipeline = std::make_shared<Pipeline>(pipeline);
		m_renderPass = std::make_shared<RenderPass>(renderPass);
	}

	/*
	@brief : Copy constructor
	@param : A constant reference to the Command Buffers to copy
	*/
	CommandBuffers::CommandBuffers(const CommandBuffers& cmd) : m_device(cmd.m_device), m_swapChain(cmd.m_swapChain), m_pipeline(cmd.m_pipeline)
		, m_renderPass(cmd.m_renderPass), m_commandPool(cmd.m_commandPool), m_commandBuffers(cmd.m_commandBuffers)
	{}

	/*
	@brief : Creates command buffers
	@return : Returns true if the creation of the command buffers is a success, false otherwise
	*/
	bool CommandBuffers::CreateCommandBuffers()
	{
		if (!CreateCommandPool(m_device->GetDevice()->presentIndexFamily, &m_commandPool))
		{
			std::cout << "Failed to create command pool" << std::endl;
			return false;
		}

		uint32_t imageCount = static_cast<uint32_t>(m_swapChain->GetSwapChain()->image.size());
		m_commandBuffers.resize(imageCount, VK_NULL_HANDLE);

		if (!AllocateCommandBuffers(imageCount, m_commandPool, &m_commandBuffers[0]))
		{
			std::cout << "Failed to allocate command buffers" << std::endl;
			return false;
		}

		return true;
	}

	/*
	@brief : Records the command buffers
	*/
	bool CommandBuffers::RecordCommandBuffers()
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			nullptr
		};

		VkImageSubresourceRange imageSubresourceRange =
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			1,
			0,
			1
		};

		//La couleur orange clair
		VkClearValue clearValue = 
		{
			{ 1.0f, 0.8f, 0.4f, 0.0f },
		};

		const std::vector<VkImage>& swapChainImage = m_swapChain->GetSwapChain()->image;

		for (std::size_t i = 0; i < m_commandBuffers.size(); ++i) {
			vkBeginCommandBuffer(m_commandBuffers[i], &commandBufferBeginInfo);

			VkRenderPassBeginInfo renderPassBeginInfo =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				nullptr,
				m_renderPass->GetRenderPass()->renderPass,
				m_renderPass->GetRenderPass()->framebuffer[i],
				{
					{
						0,
						0
					},
					{
						300,
						300
					}
				},
				1,
				&clearValue
			};

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());
			vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(m_commandBuffers[i]);

			if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
			{
				std::cout << "Failed to end command buffer" << std::endl;
				return false;
			}
		}

		return true;
	}

	/*
	@brief : Destroys ressources about the command buffers and the command pool
	*/
	void CommandBuffers::DestroyRessources()
	{
		if (m_device->GetDevice()->logicalDevice != VK_NULL_HANDLE) {
			vkDeviceWaitIdle(m_device->GetDevice()->logicalDevice);

			if ((GetCommandBuffers().size() > 0) && (GetCommandBuffers().data() != VK_NULL_HANDLE)) {
				vkFreeCommandBuffers(m_device->GetDevice()->logicalDevice, GetCommandPool(), static_cast<uint32_t>(GetCommandBuffers().size()), GetCommandBuffers().data());

				m_commandBuffers.clear();
			}

			if (GetCommandPool() != VK_NULL_HANDLE) {
				vkDestroyCommandPool(m_device->GetDevice()->logicalDevice, GetCommandPool(), nullptr);
				m_commandPool = VK_NULL_HANDLE;
			}
		}
	}

	/*
	@brief : Destroys a command pool
	*/
	void CommandBuffers::DestroyCommandPool()
	{
		vkDestroyCommandPool(m_device->GetDevice()->logicalDevice, m_commandPool, nullptr);
		m_commandPool = VK_NULL_HANDLE;
	}

	/*
	@brief : Frees a command buffers
	*/
	void CommandBuffers::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(m_device->GetDevice()->logicalDevice, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
		m_commandBuffers.clear();
	}

	//-------------------------Private method-------------------------

	bool CommandBuffers::CreateCommandPool(uint32_t indexFamily, VkCommandPool* commandPool)
	{
		VkCommandPoolCreateInfo commandPoolInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			indexFamily
		};

		if (vkCreateCommandPool(m_device->GetDevice()->logicalDevice, &commandPoolInfo, nullptr, commandPool) != VK_SUCCESS)
		{
			std::cout << "Failed to create command pool" << std::endl;
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------

	bool CommandBuffers::AllocateCommandBuffers(uint32_t imageCount, VkCommandPool commandPool, VkCommandBuffer* commandBuffer)
	{
		VkCommandBufferAllocateInfo commandBufferAllocate =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			imageCount
		};

		if (vkAllocateCommandBuffers(m_device->GetDevice()->logicalDevice, &commandBufferAllocate, commandBuffer) != VK_SUCCESS)
		{
			std::cout << "Failed to allocate command buffers" << std::endl;
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------

	std::vector<VkCommandBuffer> CommandBuffers::BuildVectorCommandBuffers()
	{
		std::vector<VkCommandBuffer> commandBuffer;

		return commandBuffer;
	}
}