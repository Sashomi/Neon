#pragma once

#ifndef ZFILE_HPP
#define ZFILE_HPP

#include <iostream>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <Neon/Core/String.hpp>

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
		
		std::vector<char> GetBinaryFileContent() const;

	private :
		boost::filesystem::path m_path;
	};
}

#endif //ZFILE_HPP