#include <Neon/Core/String.hpp>

#ifdef NEON_WINDOWS
	#include <Neon/Core/Win/PluginPlatform.hpp>
#endif

namespace Zx
{
	/*
	@brief : Load a plugin, return true if the library is correctly load, false otherwise
	@param : A constant reference to the name of the plugin to load
	*/
	template <typename T>
	T* Plugin<T>::LoadPlugin(const String& pluginName)
	{
		T* func = m_platform->LoadPlugin(pluginName);

		return func;
	}

	/*
	@brief : Unload a plugin, returns true if the library is correctly unload, false otherwise
	*/
	template <typename T>
	void Plugin<T>::UnloadPlugin()
	{
		m_platform->UnloadPlugin();
	}

	template <typename T>
	bool Plugin<T>::IsLoad()
	{
		return (m_platform->IsLoad());
	}

	template <typename T>
	std::shared_ptr<PluginPlatform<T>> Plugin<T>::m_platform(new PluginPlatform<T>);
}