#include <fstream>

#include <Neon/Core/Exception.hpp>
#include <Neon/Core/File.hpp>

namespace Zx 
{
	/*
	@brief : Constructs of File
	@param : A constant reference to the file path of the file
	*/
	File::File(const String& filePath) : m_path(filePath.GetPtr())
	{}

	/*
	@brief : Returns true if the File exist, false otherwise
	*/
	bool File::IsExist() const
	{
		return (std::experimental::filesystem::exists(m_path));
	}

	/*
	@brief : Returns the complete file path of the file
	@note : Throw an exception if the file doesn't exist
	*/
	String File::GetFilePath() const
	{
		ZAssert(IsExist(), "File doesn't exist");

		return String(std::experimental::filesystem::canonical(m_path).string());
	}

	/*
	@brief : Returns the name of the file, without extension
	@note : Throw an exception if the file doesn't exist
	*/
	String File::GetFileName() const
	{
		ZAssert(IsExist(), "File doesn't exist");

		return String(m_path.string());
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

	/*
	@brief : Reads the content of a file
	@return : Returns the content of the file in a vector<char>
	*/
	std::vector<char> File::GetBinaryFileContent() const
	{
		ZAssert(IsExist(), "File doesn't exist");

		std::ifstream file(m_path.c_str(), std::ios::binary);

		if (file.fail())
		{
			std::cout << "Failed to open ' " << m_path << " ' file" << std::endl;
			return std::vector<char>();
		}

		std::streampos begin, end;
		
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();
		
		std::vector<char> res(static_cast<std::size_t>(end - begin));

		file.seekg(0, std::ios::beg);
		file.read(&res[0], end - begin);
		
		file.close();

		return res;
	}
}
