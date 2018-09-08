#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <vector>

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
		//std::filesystem m_path;
	};
}

#endif //FILE_HPP
