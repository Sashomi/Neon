#include <Zanix/Core/ZException.hpp>

namespace Zx
{
	/*
	@brief : Constructs of ZException
	@param : A constant reference to the message of the exception
	*/
	ZException::ZException(const ZString& message) : m_message(message)
	{
	}

	/*
	@brief : Returns the message of the exception
	*/
	ZString ZException::GetMessage()
	{
		return m_message;
	}

	/*
	@brief : Displays the file, the line, the message of the assert exception
	@param : A constant reference to the file where the ZAssertException is throw
	@param : The line where the ZAssertException is throw
	@param : A constant reference to the message to display
	*/
	ZAssertException::ZAssertException(const ZString& file, int line, const ZString& message)
	{
		std::cout << "[EXCEPTION] File : " << file << " - Line : " << line << " - Message : " << message << std::endl;
	}

	/*
	@brief : Displays the file and the message of the operation failed exception
	@param : A constant reference to the file where ZOperationFailed is throw
	@param : A constant reference to the message to display
	*/
	ZOperationFailed::ZOperationFailed(const ZString& file, const ZString& message)
	{
		std::cout << "[EXCEPTION] File : " << file << " - Message : " << message << std::endl;
	}

	/*
	@brief : Displays the window's name and the message of the operation window exception
	@param : A constant reference to the name of the window where ZWindowException is throw
	@param : A constant reference to the message to display
	*/
	ZWindowException::ZWindowException(const ZString& windowName, const ZString& message)
	{
		std::cout << "[EXCEPTION] Window : " << windowName << " - Message : " << message << std::endl;
	}
}