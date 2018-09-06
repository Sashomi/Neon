#include <iostream>

#include <Zanix/Renderer/Device.hpp>
#include <Zanix/Renderer/VertexBuffer.hpp>

namespace Zx
{
	/*
	@brief : Constructs a vertex buffer
	@param : A reference to the Device
	*/
	VertexBuffer::VertexBuffer(Device& device)
	{
		m_device = std::make_shared<Device>(device);

		if (!CreateVertexBuffer())
			std::cout << "Failed to create vertex buffer" << std::endl;

		device = std::move(*m_device);
	}

	/*
	@brief : Destroys a vertex buffer
	*/
	VertexBuffer::~VertexBuffer()
	{
		if (m_vertexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_device->GetDevice()->logicalDevice, m_vertexBuffer, nullptr);
			m_vertexBuffer = VK_NULL_HANDLE;
		}
	}

	//----------------------------------Private methods----------------------------------

	bool VertexBuffer::CreateVertexBuffer()
	{
		VertexData vertexData[] =
		{
			{
				-0.7f, -0.7f, 0.0f, 1.0f,
				1.0f, 0.0f, 0.0f, 0.0f
			},
			{
				-0.7f, 0.7f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 0.0f
			},
			{
				0.7f, -0.7f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 0.0f
			},
			{
				0.7f, 0.7f, 0.0f, 1.0f,
				0.3f, 0.3f, 0.3f, 0.0f
			}
		};

		VkDeviceSize vertexSize = sizeof(vertexData);

		VkBufferCreateInfo bufferCreateInfo =
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0,
			vertexSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr
		};

		if (vkCreateBuffer(m_device->GetDevice()->logicalDevice, &bufferCreateInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS)
		{
			std::cout << "Failed to create buffer" << std::endl;
			return false;
		}

		if (!AllocateBufferMemory(m_vertexBuffer, &m_memory))
		{
			std::cout << "Failed to allocate buffer memory" << std::endl;
			return false;
		}

		if (vkBindBufferMemory(m_device->GetDevice()->logicalDevice, m_vertexBuffer, m_memory, 0) != VK_SUCCESS)
		{
			std::cout << "Failed to bind buffer memory" << std::endl;
			return false;
		}

		void* vertexBufferMemoryPtr = nullptr;
		if (vkMapMemory(m_device->GetDevice()->logicalDevice, m_memory, 0, vertexSize, 0, &vertexBufferMemoryPtr) != VK_SUCCESS)
		{
			std::cout << "Failed to map vertex buffer memory" << std::endl;
			return false;
		}

		std::memcpy(vertexBufferMemoryPtr, vertexData, vertexSize);

		VkMappedMemoryRange mappedMemoryRange =
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			m_memory,
			0,
			VK_WHOLE_SIZE
		};
		
		vkFlushMappedMemoryRanges(m_device->GetDevice()->logicalDevice, 1, &mappedMemoryRange);

		vkUnmapMemory(m_device->GetDevice()->logicalDevice, m_memory);

		return true;
	}

	//-------------------------------------------------------------------------

	bool VertexBuffer::AllocateBufferMemory(const VkBuffer& buffer, VkDeviceMemory* memory)
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_device->GetDevice()->logicalDevice, buffer, &memoryRequirements);

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(m_device->GetDevice()->physicalDevice, &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if ((memoryRequirements.memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
			{
				VkMemoryAllocateInfo memoryAllocateInfo =
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					0,
					memoryRequirements.size,
					i
				};

				if (vkAllocateMemory(m_device->GetDevice()->logicalDevice, &memoryAllocateInfo, nullptr, memory) == VK_SUCCESS)
					return true;
			}
		}

		return false;
	}
}