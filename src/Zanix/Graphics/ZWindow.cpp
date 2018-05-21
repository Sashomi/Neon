#include <Zanix/Renderer/ZVulkan.hpp>
#include <Zanix/Graphics/ZWindow.hpp>

namespace Zx
{
	/*
	@brief : Constructor of ZWindow
	*/
	ZWindow::ZWindow() : m_open(false), m_window(nullptr)
	{
	}

	/*
	@brief : Copy constructor
	@param : The ZWindow to copy
	*/
	ZWindow::ZWindow(const ZWindow& window) : m_open(window.m_open), m_width(window.m_width), m_height(window.m_height),
		m_title(window.m_title), m_window(window.m_window)
	{
	}

	/*
	@brief : Move constructor
	@param : The ZWindow to move
	*/
	ZWindow::ZWindow(ZWindow&& window) 
	{
		std::swap(m_open, window.m_open);
		std::swap(m_height, window.m_height);
		std::swap(m_width, window.m_width);
		std::swap(m_title, window.m_title);
		std::swap(m_window, window.m_window);
	}
	
	/*
	@brief : Creates a ZWindow
	@param : The width of the window
	@param : The height of the window
	@param : A constant reference to the title of the window
	*/
	void ZWindow::CreateZWindow(int width, int height, const ZString& title)
	{
		m_width = width;
		m_height = height;
		m_title = title;

		glfwInit();
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_width, m_height, title.GetPtr(), nullptr, nullptr);

		m_open = true;
	}

	/*
	@brief : Destroys the ZWindow
	@note : Throw an assert exception if the window is open
	*/
	void ZWindow::DestroyWindow()
	{
		ZAssert(IsOpen(), "You can't close a closed window");

		glfwDestroyWindow(m_window);
		glfwTerminate();
		m_open = false;
	}

	/*
	@brief : Returns the window
	*/
	GLFWwindow* ZWindow::GetWindow() const
	{
		return m_window;
	}
	
	/*
	@brief : Returns true if the windows is close, false otherwise
	*/
	bool ZWindow::IsOpen() const
	{
		glfwPollEvents();
		return m_open;
	}

	/*
	@brief : Closes the ZWindow
	@note : Requires the window to be open
	*/
	void ZWindow::CloseWindow()
	{
		if(IsOpen())
			DestroyWindow();
	}

	/*
	@brief : Changes the size of the ZWindow
	@param : The new width of the window
	@param : The new height of the window
	@note : Throw an assert exception if the window is close
	*/
	void ZWindow::SetWindowSize(int width, int height)
	{
		ZAssert(IsOpen(), "You can't resize a closed window");
		m_width = width;
		m_height = height;
		glfwSetWindowSize(m_window, width, height);
	}

	/*
	@brief : Gets the size of the ZWindow
	@param : A pointer to the width
	@param : A pointer to the height
	*/
	void ZWindow::GetWindowSize(int *width, int *height) const
	{
		*width = m_width;
		*height = m_height;
	}

	/*
	@brief : Changes the title of the ZWindow
	@param : A constant reference to the new title of the ZWindow
	@note : Throw an assert exception if the window is close
	*/
	void ZWindow::SetWindowTitle(const ZString& title)
	{
		ZAssert(IsOpen(), "You can't change the title of a closed window");
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.GetPtr());
	}

	/*
	@brief : Returns the title of the ZWindow
	*/
	ZString ZWindow::GetWindowTitle() const
	{
		return (ZString(m_title));
	}

	/*
	@brief : Copy a ZWindow to this ZWindow
	@param : The ZWindow to copy
	*/
	ZWindow& ZWindow::operator=(const ZWindow& window)
	{
		m_open = window.m_open;
		m_height = window.m_height;
		m_width = window.m_width;
		m_title = window.m_title;
		m_window = window.m_window;

		return (*this);
	}
	
	/*
	@brief : Move a ZWindow to this ZWindow
	@param : The ZWindow to move
	*/
	ZWindow& ZWindow::operator=(ZWindow&& window)
	{
		std::swap(m_open, window.m_open);
		std::swap(m_height, window.m_height);
		std::swap(m_width, window.m_width);
		std::swap(m_title, window.m_title);
		std::swap(m_window, window.m_window);

		return (*this);
	}
}