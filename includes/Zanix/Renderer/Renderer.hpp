#ifndef ZRENDERER_HPP
#define ZRENDERER_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = delete;

		static void Initialize();
		static void Destroy();

		static const VkInstance& GetVulkanInstance();

	private:
		static VkInstance s_instance;

	private:
		static void CreateInstance();
		static void DestroyInstance();

		static bool IsExtensionAvailable();
	};
}

#endif //ZRENDERER_HPP