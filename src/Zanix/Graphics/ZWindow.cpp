#include <Zanix/Graphics/ZWindow.hpp>

namespace Zx
{
	/*
		@brief : Constructor of ZWindow
	*/
	ZWindow::ZWindow() noexcept : m_open(false), m_window(nullptr)
	{
	}

	/*
		@brief : Create a ZWindow
		@param : The width of the window
		@param : The height of the window
		@param : The title of the window
	*/
	void ZWindow::CreateWindow(int width, int height, const ZString& title)
	{
		m_width = width;
		m_height = height;
		m_title = title;

		glfwInit();
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_window = glfwCreateWindow(m_width, m_height, title.GetPtr(), nullptr, nullptr);

		m_open = true;
	}

	/*
		@brief : Destroy the window
		@note : Produce an assert exception if the window is open
	*/
	void ZWindow::DestroyWindow()
	{
		ZAssert(IsOpen());
		glfwDestroyWindow(m_window);
		glfwTerminate();
		m_open = false;
	}

	/*
		@brief : Return true if the windows is close, false otherwise
	*/
	bool ZWindow::IsOpen()
	{
		glfwPollEvents();
		return m_open;
	}

	/*
		@brief : Close the window
		@note : Requires the window to be open
	*/
	void ZWindow::CloseWindow()
	{
		if(IsOpen())
			DestroyWindow();
	}

	/*
		@brief : Destroy the window
		@param : The width of the window
		@param : The height of the window
		@note : Produce an assert exception if the window is not open
	*/
	void ZWindow::SetWindowSize(int width, int height)
	{
		ZAssert(IsOpen());
		m_width = width;
		m_height = height;
		glfwSetWindowSize(m_window, width, height);
	}

	/*
		@brief : Get the size of the window
		@param : A pointer to the width
		@param : A pointer to the height
	*/
	void ZWindow::GetWindowSize(int *width, int *height) const
	{
		*width = m_width;
		*height = m_height;
	}

	/*
		@brief : Change the title of the ZWindow
		@param : The new title of the ZWindow
		@note : Produce an assert exception if the window is not open
	*/
	void ZWindow::SetWindowTitle(const ZString& title)
	{
		ZAssert(IsOpen());
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.GetPtr());
	}

	/*
		@brief : Get the title of the window
		@param : A pointer to the title of the window
	*/
	void ZWindow::GetWindowTitle(ZString* title) const
	{
		*title = m_title;
	}
}