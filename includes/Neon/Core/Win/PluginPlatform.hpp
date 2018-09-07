#pragma once

#ifdef ZPLUGINPLATFORM_HPP
#define ZPLUGINPLATFORM_HPP

#include <Windows.h>

namespace Zx
{
	class String;

	template <typename T>
	class PluginPlatform
	{
	public : 
		PluginPlatform();
		~PluginPlatform() = default;

		T* LoadPlugin(const String& pluginName);
		void UnloadPlugin();

		bool IsLoad();
	private :
		HMODULE m_library;
	};
}

#include "PluginPlatform.inl"

#endif //ZPLUGINPLATFORM_HPP