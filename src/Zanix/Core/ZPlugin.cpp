#include <Zanix/Core/ZPlugin.hpp>

namespace Zx
{
	/*
	@brief : Constructor of ZPlugin
	*/
	template <typename T>
	ZPlugin<T>::ZPlugin() : m_library(nullptr)
	{
	}
	
	/*
	@brief : Destructor of ZPlugin
	*/
	template <typename T>
	ZPlugin<T>::~ZPlugin()
	{
		if(m_library)
			FreeLibrary(m_library);
	}

	/*
	@brief : Load a plugin
	@param : A constant reference to the name of the plugin to load
	*/
	template <typename T>
	T* ZPlugin<T>::LoadPlugin(const ZString& pluginName)
	{
		m_library = LoadLibrary(pluginName.GetPtr());
		if (!m_library)
			throw ZOperationFailed(pluginName, "Failed to launch the library");

		m_fnptr func = reinterpret_cast<m_fnptr>(GetProcAdress(m_library, "LaunchPlugin"));
		if (!m_fntr)
			throw ZOperationFailed(pluginName, "Failed to found LaunchPlugin function");

		return func;
	}
}