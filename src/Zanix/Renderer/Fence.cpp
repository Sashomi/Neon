#include <iostream>

#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/CommandBuffers.hpp>
#include <Zanix/Renderer/Fence.hpp>

namespace Zx
{
	Fence::Fence(const Device& device, const std::vector<RenderingResourcesData>& renderingResources)
	{
		m_device = std::make_shared<Device>(device);
		m_renderingResources = std::make_shared<std::vector<RenderingResourcesData>>(renderingResources);
	}

	bool Fence::CreateFence()
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