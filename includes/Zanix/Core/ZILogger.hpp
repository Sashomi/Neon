#pragma once

#ifndef ZALogger_HPP
#define ZALogger_HPP

#include <iostream>
#include <memory>

namespace Zx
{
	class ZILogger
	{
		class ZString;

	public:
		ZILogger() = default;

		virtual ~ZILogger() = default;

		static void SetLogger(std::shared_ptr<ZILogger>);

		virtual void Write(const ZString& message) = 0;

		static std::shared_ptr<ZILogger>s_instance;
	};
}

#endif //ZALogger_HPP