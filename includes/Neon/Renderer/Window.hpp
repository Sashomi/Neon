#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <vulkan/vulkan.h>

#include <Neon/Utils.hpp>

#ifdef NEON_WINDOWS
	#include <Windows.h>
#endif

#if defined (NEON_WINDOWS)
	#include <Neon/Renderer/Win32/Platform.hpp>
#elif defined (NEON_POSIX)
	#include <Neon/Renderer/Posix/Platform.hpp>
#endif

namespace Zx
{
	class Renderer;
	class String;
	//TODO : destructor
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

		#ifdef NEON_WINDOWS
		inline const HWND& GetHandle() const
		{
			return m_surfacePlatform->GetHandle();
		}

		inline const HINSTANCE& GetInstance() const
		{
			return m_surfacePlatform->GetWindow();
		}
		#endif
		
	private:
		VkInstance m_instance;
		std::shared_ptr<Platform> m_surfacePlatform;
	};
}

#endif //WINDOW_HPP