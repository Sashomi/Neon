#ifndef ZRENDERER_HPP
#define ZRENDERER_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class ZRenderer
	{
	public:
		ZRenderer() = default;
		~ZRenderer() = delete;

		static void Initialize();
		static void Destroy();

		static VkInstance GetVulkanInstance();

	private:
		static VkInstance s_instance;

	private:
		static void CreateInstance();
		static void DestroyInstance();

		static bool IsExtensionAvailable();
	};
}

#endif //ZRENDERER_HPP