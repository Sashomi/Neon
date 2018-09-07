#include <Neon/Core/ILogger.hpp>

namespace Zx
{
	/*
	@brief : Change the instance of the logger
	@param : The new instance of the logger
	*/
	void ILogger::SetLogger(std::shared_ptr<ILogger> instance)
	{
		s_instance = instance;
	}

	std::shared_ptr<ILogger> ILogger::s_instance = nullptr;
}