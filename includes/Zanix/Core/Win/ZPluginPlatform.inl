#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	template <typename T>
	ZPluginPlatform<T>::ZPluginPlatform() : m_library(nullptr)
	{
	}

	template <typename T>
	T* ZPluginPlatform<T>::LoadPlugin(const ZString& pluginName)
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
	void ZPluginPlatform<T>::UnloadPlugin()
	{
		if (IsLoad())
			FreeLibrary(m_library);
	}

	template <typename T>
	bool ZPluginPlatform<T>::IsLoad()
	{
		return (m_library);
	}

}