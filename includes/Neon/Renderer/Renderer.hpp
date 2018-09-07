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
	
	struct RenderingResourcesData;

	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(Device&, Window&, SwapChain&);
		Renderer(const Renderer&);
		Renderer(Renderer&&) noexcept;

		~Renderer();
		
		inline const VkInstance& GetVulkanInstance() const;

		Renderer& operator=(Renderer&&) noexcept;

	private:
		std::shared_ptr<Device> m_device;
		std::shared_ptr<Window> m_window;
		std::shared_ptr<SwapChain> m_swapChain;

		VkInstance m_instance;
		VkDebugReportCallbackEXT m_callback;
	private:
		bool Initialize();
		bool CreateInstance();
		bool IsExtensionAvailable();
		bool CheckValidationLayerSupport();
		bool SetupDebugCallback();

		VkResult CreateDebugReportCallbackEXT(const VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		
		void DestroyDebugCallback(const VkInstance& instance, const VkDebugReportCallbackEXT& callback, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj
			, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	};
}

#include "Renderer.inl"

#endif //ZRENDERER_HPP