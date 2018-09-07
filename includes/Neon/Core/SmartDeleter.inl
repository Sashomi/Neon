#include <Neon/Core/SmartDeleter.hpp>

namespace Zx
{
	template <class T, class F>
	SmartDeleter<T, F>::SmartDeleter() : m_object(VK_NULL_HANDLE), m_deleter(nullptr), m_device(VK_NULL_HANDLE)
	{}

	template <class O, class F>
	SmartDeleter<O, F>::SmartDeleter(O object, F deleter, VkDevice device) : m_object(object), m_deleter(deleter), m_device(device)
	{}

	template <class O, class F>
	SmartDeleter<O, F>::SmartDeleter(SmartDeleter&& smartDeleter) noexcept
	{
		std::swap(this, smartDeleter);
	}

	template <class O, class F>
	SmartDeleter<O, F>::~SmartDeleter()
	{
		if((m_object != VK_NULL_HANDLE) && (m_deleter != nullptr) && (m_device != VK_NULL_HANDLE))
			m_deleter(m_device, m_object, nullptr);
	}

	template <class O, class F>
	SmartDeleter<O, F>& SmartDeleter<O, F>::operator=(const SmartDeleter& smartDeleter)
	{
		if (this != &smartDeleter)
		{
		m_object = smartDeleter.m_object;
		m_deleter = smartDeleter.m_deleter;
		m_device = smartDeleter.m_device;
		}

		return (*this);
	}

	template <class O, class F>
	SmartDeleter<O, F>& SmartDeleter<O, F>::operator=(SmartDeleter&& smartDeleter)
	{
		if (this != &smartDeleter)
		{
		m_object = smartDeleter.m_object;
		m_deleter = smartDeleter.m_deleter;
		m_device = smartDeleter.m_device;
		}

		return (*this);
	}
}