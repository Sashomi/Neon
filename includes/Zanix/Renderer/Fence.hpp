#ifndef FENCE_HPP
#define FENCE_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include <memory>

namespace Zx
{
	class Device;
	struct RenderingResourcesData;

	class Fence
	{
	public:
		Fence(const Device& device, const std::vector<RenderingResourcesData>& renderingResources);

		bool CreateFence();
	private:
		std::shared_ptr<Device> m_device;
		std::shared_ptr<std::vector<RenderingResourcesData>> m_renderingResources;
	};
}

#endif //FENCE_HPP