#pragma once

#ifndef ZEXCEPTION_HPP
#define ZEXCEPTION_HPP

#include <iostream>

#include <Zanix/Core/ZString.hpp>
#include <Zanix/ZUtils.hpp>

namespace Zx
{
	class ZException : public std::exception
	{
	public : 
		ZException(const ZString& message = "");

		virtual ~ZException() noexcept = default;

	protected :
		ZString m_message;
	};

	struct ZAssertException : public ZException
	{
		ZAssertException(const ZString& file, int line, const ZString& message);
	};

	/*
	@Note : If the condition is true, then this is throw an exception
	*/
	#ifndef NDEBUG
		#define ZAssert(condition, message) if (!(condition)) \
				throw ZAssertException(__FILE__, __LINE__, message)
	#endif

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