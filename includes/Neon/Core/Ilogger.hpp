#pragma once

#ifndef ZALogger_HPP
#define ZALogger_HPP

#include <iostream>
#include <memory>

namespace Zx
{
	class ILogger
	{
		class String;

	public:
		ILogger() = default;

		virtual ~ILogger() = default;

		static void SetLogger(std::shared_ptr<ILogger>);

		virtual void Write(const String& message) = 0;

		static std::shared_ptr<ILogger>s_instance;
	};
}

#endif //ZALogger_HPP