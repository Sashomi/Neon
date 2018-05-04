#include <Zanix/Core/ZILogger.hpp>

namespace Zx
{
	/*
		@brief : Change the instance of the logger
		@param : The new instance of the logger
	*/
	void ZILogger::SetLogger(std::shared_ptr<ZILogger> instance)
	{
		s_instance = instance;
	}

	std::shared_ptr<ZILogger> ZILogger::s_instance = nullptr;
}