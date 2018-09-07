#ifndef SMARTDELETER_HPP
#define SMARTDELETER_HPP

#include <vulkan/vulkan.h>

namespace Zx
{
	template <class O, class F>
	class SmartDeleter
	{
	public:
		SmartDeleter();
		SmartDeleter(O object, F deleter, VkDevice device);
		SmartDeleter(SmartDeleter&&) noexcept;
		
		~SmartDeleter();

		SmartDeleter& operator=(const SmartDeleter& other);
		SmartDeleter& operator=(SmartDeleter&& other);

	public :
		inline const O& GetObj() const
		{
			return m_object;
		}

		inline bool operator !() const
		{
			return (m_object == VK_NULL_HANDLE);
		}

	private:
		O m_object;
		F m_deleter;
		VkDevice m_device;
	};
}

#include "SmartDeleter.inl"

#endif //SMARTDELETER_HPP