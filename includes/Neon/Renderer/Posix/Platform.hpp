#ifndef POSIX_PLATFORM_HPP
#define POSIX_PLATFORM_HPP

#include <memory>

#include <xcb/xcb.h>
#include <dlfcn.h>
#include <cstdlib>

namespace Zx
{
	class Platform
	{
	public:
		Platform() = default;
		Platform(const VkInstance&);

		~Platform();

		bool CreateZWindow(int width, int height, const char* title);
		bool CreateSurface();

	public:
		inline void SetVkInstance(const VkInstance& instance)
		{
			m_instance = instance;
		}

		inline const VkSurfaceKHR& GetSurface() const
		{
			return m_surface;
		}

		inline const xcb_window_t& GetHandle() const
		{
			return m_handle;
		}

		inline xcb_connection_t GetWindow() const
		{
			return m_connection;
		}

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;

		xcb_connection_t* m_connection;
		xcb_window_t m_handle;
	};
}

#endif //POSIX_PLATFORM_HPP