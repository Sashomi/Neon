#include <Neon/Core/Exception.hpp>

namespace Zx
{
	/*
	@brief : Constructs of Exception
	@param : A constant reference to the message of the exception
	*/
	Exception::Exception(const String& message) : m_message(message)
	{
	}

	/*
	@brief : Displays the file, the line, the message of the assert exception
	@param : A constant reference to the file where the ZAssertException is throw
	@param : The line where the ZAssertException is throw
	@param : A constant reference to the message to display
	*/
	ZAssertException::ZAssertException(const String& file, int line, const String& message)
	{
		std::cout << "[EXCEPTION] File : " << file << " - Line : " << line << " - Message : " << message << std::endl;
	}

	/*
	@brief : Displays the file and the message of the operation failed exception
	@param : A constant reference to the file where ZOperationFailed is throw
	@param : A constant reference to the message to display
	*/
	ZOperationFailed::ZOperationFailed(const String& file, const String& message)
	{
		std::cout << "[EXCEPTION] File : " << file << " - Message : " << message << std::endl;
	}

	/*
	@brief : Displays the window's name and the message of the operation window exception
	@param : A constant reference to the name of the window where WindowException is throw
	@param : A constant reference to the message to display
	*/
	WindowException::WindowException(const String& windowName, const String& message)
	{
		std::cout << "[EXCEPTION] Window : " << windowName << " - Message : " << message << std::endl;
	}
}