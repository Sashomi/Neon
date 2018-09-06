#include <iostream>

#include <Zanix/Renderer/CommandBuffers.hpp>
#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/Sync.hpp>

namespace Zx
{
	/*
	@brief : Constructs semaphores and fence
	@param : A reference to the Device
	@param : A reference to the rendering ressources
	*/
	Sync::Sync(Device& device, std::vector<RenderingResourcesData>& renderingResources)
	{
		m_device = std::make_shared<Device>(device);
		m_renderingResources = std::make_shared<std::vector<RenderingResourcesData>>(renderingResources);

		if (!CreateSemaphores())
			std::cout << "Failed to create semaphores" << std::endl;

		if(!CreateFence())
			std::cout << "Failed to create fence" << std::endl;

		device = std::move(*m_device);
		renderingResources = std::move(*m_renderingResources);
	}

	//-----------------------------Private methods-----------------------------
	bool Sync::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo =
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0
		};
		
		for (std::size_t i = 0; i < m_renderingResources->size(); i++)
		{
			if ((vkCreateSemaphore(m_device->GetDevice()->logicalDevice, &semaphoreInfo, nullptr, &(*m_renderingResources)[i].imageAvailableSemaphore) != VK_SUCCESS)
				|| (vkCreateSemaphore(m_device->GetDevice()->logicalDevice, &semaphoreInfo, nullptr, &(*m_renderingResources)[i].finishedRenderingSemaphore) != VK_SUCCESS))
			{
				std::cout << "Failed to create semaphore" << std::endl;
				return false;
			}
		}
		
		return true;
	}
	
	//---------------------------------------------------------------

	bool Sync::CreateFence()
	{
		VkFenceCreateInfo fenceCreateInfo =
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};

		for (std::size_t i = 0; i < m_renderingResources->size(); i++)
		{
			if (vkCreateFence(m_device->GetDevice()->logicalDevice, &fenceCreateInfo, nullptr, &(*m_renderingResources)[i].fence) != VK_SUCCESS)
			{
				std::cout << "Failed to create fence" << std::endl;
				return false;
			}
		}

		return true;
	}
}