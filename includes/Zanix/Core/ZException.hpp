#pragma once

#ifndef ZEXCEPTION_HPP
#define ZEXCEPTION_HPP

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
	#define ZAssert(condition) if ((condition)) \
		throw ZAssertException(__FILE__, __LINE__, "False condition\n -> " #condition)

	struct ZOperationFailed : public ZException
	{
		ZOperationFailed(const ZString& file, const ZString& message);
	};

	struct ZWindowException : public ZException
	{
		ZWindowException(const ZString& windowName, const ZString& message);
	};
}

#endif //ZEXCEPTION_HPP