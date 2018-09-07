#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Device;

	struct VertexData
	{
		float x, y, z, w;
		float r, g, b, a;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(Device& device);

		~VertexBuffer();

		inline const VkBuffer& GetVertexBuffer() const;

	private:
		std::shared_ptr<Device> m_device;

		VkDeviceMemory m_memory;
		VkBuffer m_vertexBuffer;
	private:
		bool CreateVertexBuffer();

		bool AllocateBufferMemory(const VkBuffer& buffer, VkDeviceMemory* memory);
	};
}

#include "VertexBuffer.inl"

#endif //VERTEXBUFFER_HPP