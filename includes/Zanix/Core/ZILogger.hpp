#pragma once

#ifndef ZALogger_HPP
#define ZALogger_HPP

#include <iostream>
#include <memory>

#include "ZString.hpp"

namespace Zx
{
	class ZALogger
	{
	public:
		ZALogger() = default;

		virtual ~ZALogger() = default;

		static void SetLogger(std::shared_ptr<ZALogger>);

		virtual void Write(const ZString& message) = 0;

		static std::shared_ptr<ZALogger>s_instance;
	};
}

#endif //ZALogger_HPP