#include <string>
#include <iostream>
#include <vulkan/vulkan.h>

#include <Neon/Renderer/Posix/Platform.hpp>

namespace Zx
{
	Platform::Platform(const VkInstance& instance)
	{
		m_instance = instance;
	}

	Platform::~Platform()
	{
		xcb_destroy_windows(m_connection, m_handle);
		xcb_disconnect(m_connection);
	}

	bool Platform::CreateZWindow(int width, int height, const char* title)
	{
		int index = 0;
		m_connection = xcb_connect(nullptr, &index);

		if (m_connection == nullptr)
			return false;

		const xcb_setup_t* setup = xcb_get_setup(m_connection);
		xcb_screen_iterator_t screenIterator = xcb_setups_roots_iterator(setup);

		while (index-- > 0)
		{
			xcb_screen_next(&screenIterator);
		}

		xcb_screen_t* screen = screenIterator.data;

		m_handle = xcb_generate_id(m_connection);
		
		uint32_t valueList[] = 
		{
			screen->white_pixel,
			XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY
		};

		xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, m_handle, screen->root, 0, 0, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
			screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, valueList);

		xcb_flush(m_connection);

		xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_handle, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, std::strlen(title), title);

		return true;
	}
	/*
	@brief : Create a window's surface
	@return : Returns true if the creation of the window surface is a success, false otherwise
	*/
	bool Platform::CreateSurface()
	{
		VkXcbSurfaceCreateInfoKHR xcbSurfaceInfo =
		{
			VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			m_connection,
			m_handle
		};

		if (vkCreateXcbSurfaceKHR(m_instance, &xcbSurfaceInfo, nullptr, &m_surface) == VK_SUCCESS)
			return true;

		std::cout << "Could not create presentation surface (xcb)." << std::endl;

		return false;
	}
}