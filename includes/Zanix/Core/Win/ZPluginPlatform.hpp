#pragma once

#ifdef ZPLUGINPLATFORM_HPP
#define ZPLUGINPLATFORM_HPP

#include <Windows.h>

namespace Zx
{
	class ZString;

	template <typename T>
	class ZPluginPlatform
	{
	public : 
		ZPluginPlatform();
		~ZPluginPlatform() = default;

		T* LoadPlugin(const ZString& pluginName);
		void UnloadPlugin();

		bool IsLoad();
	private :
		HMODULE m_library;
	};
}

#include "ZPluginPlatform.inl"

#endif //ZPLUGINPLATFORM_HPP