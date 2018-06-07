#pragma once

#ifndef ZWINDOW_HPP
#define ZWINDOW_HPP

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Zanix/Core/ZString.hpp>

namespace Zx
{
	class ZWindow
	{
	public :
		ZWindow();
		ZWindow(int width, int height, ZString title);
		ZWindow(const ZWindow&);
		ZWindow(ZWindow&&);

		bool IsOpen() const;

		void CreateZWindow(int width, int height, const ZString& title);
		void CloseWindow();

		void SetWindowTitle(const ZString& title);
		void SetWindowSize(int width, int height);

		ZString GetWindowTitle() const;
		void GetWindowSize(int *width, int *height) const;
		void GetWindowSize(uint32_t *width, uint32_t *height) const;

		GLFWwindow* GetWindow() const;

		ZWindow& operator=(const ZWindow&);
		ZWindow& operator=(ZWindow&&);

	private :
		void DestroyWindow();

		ZString m_title;

		int m_width;
		int m_height;

		bool m_open;

		GLFWwindow* m_window;
	};
}

#endif //ZWINDOW_HPP