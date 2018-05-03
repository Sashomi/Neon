#pragma once

#include <iostream>
#include <string>

#include <Zanix/Core/ZString.hpp>

namespace Zx
{
	class ZException : public std::exception
	{
	public : 
		ZException(const ZString& message = "");

		virtual ~ZException() noexcept = default;

		virtual ZString GetMessage() noexcept;

	protected :
		ZString m_message;
	};

	struct ZAssertException : public ZException
	{
		ZAssertException(const ZString& file, int line, const ZString& message);
	};
	#define ZAssert(condition) if (!(condition)) \
		throw ZAssertException(__FILE__, __LINE__, "False condition\n\n -> " #condition)
}