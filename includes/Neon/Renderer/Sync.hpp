#ifndef SYNC_HPP
#define SYNC_HPP

#include <memory>
#include <vector>

namespace Zx
{
	struct RenderingResourcesData;
	class Device;

	class Sync
	{
	public:
		Sync(Device&, std::vector<RenderingResourcesData>&);

	private:
		std::shared_ptr<std::vector<RenderingResourcesData>> m_renderingResources;
		std::shared_ptr<Device> m_device;

	private:
		bool CreateSemaphores();
		bool CreateFence();
	};
}

#endif //SYNC_HPP