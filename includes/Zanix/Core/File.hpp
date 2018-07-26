#pragma once

#ifndef ZFILE_HPP
#define ZFILE_HPP

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace Zx 
{
	class File
	{
	public :
		File(const String& filePath = "unknown");
		
		bool IsExist() const;
		
		String GetFilePath() const;
		String GetFileName() const;
		String GetFileExtension() const;

	private :
		boost::filesystem::path m_path;

	};
}

#endif //ZFILE_HPP