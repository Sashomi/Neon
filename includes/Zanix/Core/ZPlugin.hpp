#pragma once

#ifndef ZPLUGIN_HPP
#define ZPLUGIN_HPP

#include <iostream>
#include <memory>

#include <Zanix/ZUtils.hpp>

namespace Zx
{
	class ZString;

	template <typename T>
	class ZPluginPlatform;

	template <typename T>
	class ZPlugin
	{
	public :
		ZPlugin() = delete;
		~ZPlugin() = delete;

		static T* LoadPlugin(const ZString& pluginName);
		static void UnloadPlugin();

		static bool IsLoad();

	private :
		static std::shared_ptr<ZPluginPlatform<T>> m_platform;
	};
}

#include "ZPlugin.inl"

#endif //ZPLUGIN_HPP