#ifndef WINDOWS_PLATFORM_HPP
#define WINDOWS_PLATFORM_HPP

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

		bool CreateZWindow(int width, int height, const String& title);
		bool CreateSurface();

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

		inline const HINSTANCE& GetWindow() const
		{
			return m_windowInstance;
		}

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_windowSurface;

		HINSTANCE m_windowInstance;
		HWND m_windowHandle;
	};
}

#endif //WINDOWS_PLATFORM_HPP