#pragma once

#ifndef ZFILE_HPP
#define ZFILE_HPP

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

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

#endif //ZFILE_HPP