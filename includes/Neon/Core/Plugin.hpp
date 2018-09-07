#pragma once

#ifndef ZPLUGIN_HPP
#define ZPLUGIN_HPP

#include <iostream>
#include <memory>

namespace Zx
{
	class String;

	template <typename T>
	class PluginPlatform;

	template <typename T>
	class Plugin
	{
	public :
		Plugin() = delete;
		~Plugin() = delete;

		static T* LoadPlugin(const String& pluginName);
		static void UnloadPlugin();

		static bool IsLoad();

	private :
		static std::shared_ptr<PluginPlatform<T>> m_platform;
	};
}

#include "Plugin.inl"

#endif //ZPLUGIN_HPP