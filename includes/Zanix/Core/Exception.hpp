#pragma once

#ifndef ZEXCEPTION_HPP
#define ZEXCEPTION_HPP

#include <iostream>

#include <Zanix/ZUtils.hpp>
#include <Zanix/Core/String.hpp>

namespace Zx
{
	class Exception : public std::exception
	{
	public : 
		Exception(const String& message = "");

		virtual ~Exception() noexcept = default;

	protected :
		String m_message;
	};

	struct ZAssertException : public Exception
	{
		ZAssertException(const String& file, int line, const String& message);
	};

	/*
	@Note : If the condition is true, then this is throw an exception
	*/
	#ifdef ZDEBUG
		#define ZAssert(condition, message) if (!(condition)) \
				throw ZAssertException(__FILE__, __LINE__, message)
	#endif

	struct ZOperationFailed : public Exception
	{
		ZOperationFailed(const String& file, const String& message);
	};

	struct WindowException : public Exception
	{
		WindowException(const String& windowName, const String& message);
	};
}

#endif //ZEXCEPTION_HPP