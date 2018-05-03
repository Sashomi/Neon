#include <Zanix/Core/ZILogger.hpp>

namespace Zx
{
	void ZALogger::SetLogger(std::shared_ptr<ZALogger> instance)
	{
		s_instance = instance;
	}

	std::shared_ptr<ZALogger> ZALogger::s_instance = nullptr;
}