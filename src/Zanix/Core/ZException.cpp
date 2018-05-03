#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	/*
	@brief : Construct of ZException
	@param : The message of the exception
	*/
	ZException::ZException(const ZString& message) : m_message(message)
	{
	}

	/*
	@brief : Return the message of the exception
	*/
	ZString ZException::GetMessage() noexcept
	{
		return m_message;
	}

	/*
	@brief : Display the file, the line, the message of the assert exception
	*/
	ZAssertException::ZAssertException(const ZString& file, int line, const ZString& message)
	{
		std::cout << "[EXCEPTION] File : " << file << " - Line : " << line << " - Message : " << message << std::endl;
	}

	/*
	@brief : Display the file and the message of the operation failed exception
	*/
	ZOperationFailed::ZOperationFailed(const ZString& file, const ZString& message)
	{
		std::cout << "[EXCEPTION] File : " << file  << " - Message : " << message << std::endl;
	}
}