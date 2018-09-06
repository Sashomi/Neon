#include <iostream>

#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/SwapChain.hpp>
#include <Zanix/Renderer/RenderPass.hpp>
#include <Zanix/Renderer/Pipeline.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>

namespace Zx
{
	/*
	@brief : Creates a command buffers
	@param : The device of the application
	@param  The swapChain of the application
	@param : The renderPass of the application
	*/
	CommandBuffers::CommandBuffers(Device& device, SwapChain& swapChain, Pipeline& pipeline, RenderPass& renderPass) :
		m_renderingResources(3)
	{
		m_device = std::make_shared<Device>(device);
		m_swapChain = std::make_shared<SwapChain>(swapChain);
		m_pipeline = std::make_shared<Pipeline>(pipeline);
		m_renderPass = std::make_shared<RenderPass>(renderPass);

		if (!CreateCommandBuffers())
			std::cout << "Failed to create command buffers" << std::endl;

		device = std::move(*m_device);
		swapChain = std::move(*m_swapChain);
		pipeline = std::move(*m_pipeline);
		renderPass = std::move(*m_renderPass);
	}

	/*
	@brief : Copies constructor
	@param : A constant reference to the Command Buffers to copy
	*/
	CommandBuffers::CommandBuffers(const CommandBuffers& cmd) : m_device(cmd.m_device), m_swapChain(cmd.m_swapChain), m_pipeline(cmd.m_pipeline)
		, m_renderPass(cmd.m_renderPass), m_commandPool(cmd.m_commandPool), m_renderingResources(cmd.m_renderingResources)
	{}

	/*
	@brief : Destroys the rendering resources and the command pool
	*/
	CommandBuffers::~CommandBuffers()
	{
		if (m_device->GetDevice() != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(m_device->GetDevice()->logicalDevice);

			for (std::size_t i = 0; i < m_renderingResources.size(); i++)
			{
				if (m_renderingResources[i].framebuffer != VK_NULL_HANDLE)
				{
					vkDestroyFramebuffer(m_device->GetDevice()->logicalDevice, m_renderingResources[i].framebuffer, nullptr);
					m_renderingResources[i].framebuffer = VK_NULL_HANDLE;
				}
					
				if (m_renderingResources[i].commandBuffer != VK_NULL_HANDLE)
				{
					vkFreeCommandBuffers(m_device->GetDevice()->logicalDevice, m_commandPool, 1, &m_renderingResources[i].commandBuffer);
					m_renderingResources[i].commandBuffer = VK_NULL_HANDLE;
				}
				
				if (m_renderingResources[i].imageAvailableSemaphore != VK_NULL_HANDLE)
				{
					vkDestroySemaphore(m_device->GetDevice()->logicalDevice, m_renderingResources[i].imageAvailableSemaphore, nullptr);
					m_renderingResources[i].imageAvailableSemaphore = VK_NULL_HANDLE;
				}

				if (m_renderingResources[i].finishedRenderingSemaphore != VK_NULL_HANDLE)
				{
					vkDestroySemaphore(m_device->GetDevice()->logicalDevice, m_renderingResources[i].finishedRenderingSemaphore, nullptr);
					m_renderingResources[i].finishedRenderingSemaphore = VK_NULL_HANDLE;
				}

				if (m_renderingResources[i].fence != VK_NULL_HANDLE)
				{
					vkDestroyFence(m_device->GetDevice()->logicalDevice, m_renderingResources[i].fence, nullptr);
					m_renderingResources[i].fence = VK_NULL_HANDLE;
				}
			}

			if (m_commandPool != VK_NULL_HANDLE)
			{
				vkDestroyCommandPool(m_device->GetDevice()->logicalDevice, m_commandPool, nullptr);
				m_commandPool = VK_NULL_HANDLE;
			}
		}
	}

	//-------------------------Private method-------------------------
	bool CommandBuffers::CreateCommandBuffers()
	{
		if (!CreateCommandPool(m_device->GetDevice()->presentIndexFamily, &m_commandPool))
		{
			std::cout << "Failed to create command pool" << std::endl;
			return false;
		}

		uint32_t imageCount = static_cast<uint32_t>(m_swapChain->GetSwapChain()->image.size());

		for (std::size_t i = 0; i < m_renderingResources.size(); i++)
		{
			if (!AllocateCommandBuffers(1, m_commandPool, &m_renderingResources[i].commandBuffer))
			{
				std::cout << "Failed to allocate command buffers" << std::endl;
				return false;
			}
		}
		

		return true;
	}

	//------------------------------------------------------------------------

	bool CommandBuffers::CreateCommandPool(uint32_t indexFamily, VkCommandPool* commandPool)
	{
		VkCommandPoolCreateInfo commandPoolInfo =
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
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
}