#include <Zanix/Core/Exception.hpp>
#include <Zanix/Core/String.hpp>
#include <Zanix/Core/File.hpp>

namespace Zx 
{
	/*
	@brief : Constructs of File
	@param : A constant reference to the file path of the file
	*/
	File::File(const String& filePath) : m_path(filePath.GetPtr())
	{
	}

	/*
	@brief : Returns true if the File exist, false otherwise
	*/
	bool File::IsExist() const
	{
		return (boost::filesystem::exists(m_path));
	}

	/*
	@brief : Returns the complete file path of the file
	@note : Throw an exception if the file doesn't exist
	*/
	String File::GetFilePath() const
	{
		ZAssert(IsExist(), "File doesn't exist");
		return String(boost::filesystem::canonical(m_path).string());
	}

	/*
	@brief : Returns the name of the file, without extension
	@note : Throw an exception if the file doesn't exist
	*/
	String File::GetFileName() const
	{
		ZAssert(IsExist(), "File doesn't exist");
		return String(boost::filesystem::basename(m_path));
	}

	/*
	@brief : Returns the extension of the file
	@note : Throw an exception if the file doesn't exist
	*/
	String File::GetFileExtension() const
	{
		ZAssert(IsExist(), "File doesn't exist");
		return String(m_path.extension().string());
	}
}