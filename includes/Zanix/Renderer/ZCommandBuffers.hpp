#ifndef ZCOMMANDBUFFERS_HPP
#define ZCOMMANDBUFFERS_HPP

#include <vector>

namespace Zx
{
	class ZCommandBuffers
	{
	public:
		static void CreateCommandBuffers();
		static void DestroyRessources();
		
		static const VkCommandBuffer& GetPresentQueueCommandBuffers(uint32_t imageIndex);
		static const std::vector<VkCommandBuffer>& GetPresentQueueCommandBuffers();

		static const VkCommandPool& GetCommandPool();

	private:
		static VkCommandPool s_commandPool;
		static std::vector<VkCommandBuffer> s_presentQueueCommandBuffers;
	private:
		static bool RecordCommandsBuffers();

		static std::vector<VkCommandBuffer> BuildVectorCommandBuffers();
	};
}

#endif //ZCOMMANDBUFFERS_HPP