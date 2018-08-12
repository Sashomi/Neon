#ifndef ZRENDERER_HPP
#define ZRENDERER_HPP

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Device;
	class Window;
	class SwapChain;

	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const Device& device);
		Renderer(const Renderer& renderer);

		~Renderer() = default;

		bool Initialize(Window& window, Device& device, SwapChain& swapChain);
		void DestroyRenderer();

	public:
		inline const VkInstance& GetVulkanInstance() const
		{
			return m_instance;
		}

	private:
		VkInstance m_instance;
		std::shared_ptr<Device> m_device;

		VkDebugReportCallbackEXT m_callback;
	private:
		void Destroy();

		bool CreateInstance();
		void DestroyInstance();

		bool IsExtensionAvailable();
		bool CheckValidationLayerSupport();
		
		bool SetupDebugCallback();

		VkResult CreateDebugReportCallbackEXT(const VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		void DestroyDebugCallback(const VkInstance& instance, const VkDebugReportCallbackEXT& callback, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj
			, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	};
}

#endif //ZRENDERER_HPP