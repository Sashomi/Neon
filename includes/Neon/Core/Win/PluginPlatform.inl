#include <Neon/Core/String.hpp>
#include <Neon/Core/Exception.hpp>

namespace Zx
{
	template <typename T>
	PluginPlatform<T>::PluginPlatform() : m_library(nullptr)
	{
	}

	template <typename T>
	T* PluginPlatform<T>::LoadPlugin(const String& pluginName)
	{
		m_library = LoadLibrary(pluginName.GetPtr());

		if (!m_library)
		{
			throw ZOperationFailed(__FILE__, "Failed to load library");
		}

		T* f = reinterpret_cast<T*> (GetProcAddress(m_library, "StartPlugin"));

		if (!f)
			throw ZOperationFailed(__FILE__, "Failed to found the 'StartPlugin' function in the dynamic library");

		return f;
	}

	template <typename T>
	void PluginPlatform<T>::UnloadPlugin()
	{
		if (IsLoad())
			FreeLibrary(m_library);
	}

	template <typename T>
	bool PluginPlatform<T>::IsLoad()
	{
		return (m_library);
	}

}