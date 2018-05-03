#pragma once

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZException.hpp>

namespace Zx 
{
	class ZFile 
	{

	public :
		ZFile(const ZString& filePath = "unknown");
		
		bool IsExist() const;
		
		ZString GetFilePath() const;
		ZString GetFileName() const;
		ZString GetFileExtension() const;

	private :
		boost::filesystem::path m_path;

	};
}