#ifndef ZPLATFORM_HPP
#define ZPLATFORM_HPP

#include <Windows.h>
#include <memory>
#include <vulkan/vulkan.h>

namespace Zx
{
	class Renderer;
	class String;

	class Platform
	{
	public:
		Platform() = default;
		Platform(const VkInstance& instance);
		~Platform() = default;

		bool CreateSurface();
		bool CreateZWindow(int width, int height, const String& title);

	public:
		inline void SetVkInstance(const VkInstance& instance)
		{
			m_instance = instance;
		}

		inline const VkSurfaceKHR& GetSurface() const
		{
			return m_windowSurface;
		}

		inline const HWND& GetHandle() const
		{
			return m_windowHandle;
		}

		inline const HINSTANCE& GetInstance() const
		{
			return m_windowInstance;
		}

	private:
		VkInstance m_instance;

		HINSTANCE m_windowInstance;
		HWND m_windowHandle;
		VkSurfaceKHR m_windowSurface;
	};
}

#endif //ZPLATFORM_HPP