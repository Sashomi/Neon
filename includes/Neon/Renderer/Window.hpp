#ifndef ZWINDOW_HPP
#define ZWINDOW_HPP

#include <memory>
#include <Windows.h>
#include <vulkan/vulkan.h>

#include <Neon/Utils.hpp>

#ifdef NEON_WINDOWS
	#include <Neon/Renderer/Win32/Platform.hpp>
#endif

namespace Zx
{
	class Renderer;
	class String;

	class Window
	{
	public:
		Window();
		Window(const VkInstance& instance);
		Window(const Window& window);
		~Window() = default;

		bool CreatePresentationSurface();
		
		bool CreateZWindow(int width, int height, const String& title);

		Window& operator=(Window&&) noexcept;
	public:
		inline void SetVkInstance(const VkInstance& instance)
		{
			m_instance = instance;
			m_surfacePlatform->SetVkInstance(instance);
		}

		inline const VkSurfaceKHR& GetSurface() const
		{
			return m_surfacePlatform->GetSurface();
		}

		inline const HWND& GetHandle() const
		{
			return m_surfacePlatform->GetHandle();
		}

		inline const HINSTANCE& GetInstance() const
		{
			return m_surfacePlatform->GetInstance();
		}
		
	private:
		VkInstance m_instance;
		std::shared_ptr<Platform> m_surfacePlatform;
	};
}

#endif //ZWINDOW_HPP