#include <Zanix/Core/ZString.hpp>

#ifdef ZANIX_WINDOWS
#include <Zanix/Core/Win/ZPluginPlatform.hpp>
#endif

namespace Zx
{
	/*
	@brief : Load a plugin, return true if the library is correctly load, false otherwise
	@param : A constant reference to the name of the plugin to load
	*/
	template <typename T>
	T* ZPlugin<T>::LoadPlugin(const ZString& pluginName)
	{
		T* func = m_platform->LoadPlugin(pluginName);

		return func;
	}

	/*
	@brief : Unload a plugin, returns true if the library is correctly unload, false otherwise
	*/
	template <typename T>
	void ZPlugin<T>::UnloadPlugin()
	{
		m_platform->UnloadPlugin();
	}

	template <typename T>
	bool ZPlugin<T>::IsLoad()
	{
		return (m_platform->IsLoad());
	}

	template <typename T>
	std::shared_ptr<ZPluginPlatform<T>> ZPlugin<T>::m_platform(new ZPluginPlatform<T>);
}