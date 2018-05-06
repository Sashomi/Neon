#pragma once

#include <Zanix/Core/ZFile.hpp>

namespace Zx 
{
	/*
	@brief : Construct of ZFile
	@param : A constant reference to the file path of the file
	*/
	ZFile::ZFile(const ZString& filePath) : m_path(filePath.GetPtr())
	{
	}

	/*
	@brief : Return true if the ZFile exist, false otherwise
	*/
	bool ZFile::IsExist() const
	{
		return (boost::filesystem::exists(m_path));
	}

	/*
	@brief : Return the complete file path of the file
	@note : Throw an exception if the file doesn't exist
	*/
	ZString ZFile::GetFilePath() const
	{
		ZAssert(!IsExist(), "File doesn't exist");
		return ZString(boost::filesystem::canonical(m_path).string());
	}

	/*
	@brief : Return the name of the file, without extension
	@note : Throw an exception if the file doesn't exist
	*/
	ZString ZFile::GetFileName() const
	{
		ZAssert(!IsExist(), "File doesn't exist");
		return ZString(boost::filesystem::basename(m_path));
	}

	/*
	@brief : Return the extension of the file
	@note : Throw an exception if the file doesn't exist
	*/
	ZString ZFile::GetFileExtension() const
	{
		ZAssert(!IsExist(), "File doesn't exist");
		return ZString(m_path.extension().string());
	}
}