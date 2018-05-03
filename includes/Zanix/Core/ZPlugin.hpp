#pragma once

#include <iostream>
#include <Windows.h>

#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	template <typename T>
	class ZPlugin
	{
	public :
		ZPlugin();
		~ZPlugin();

		T* LoadPlugin(const ZString& pluginName);
	private :
		typedef T* (*m_fnptr);
		HMODULE m_library;
	};
}