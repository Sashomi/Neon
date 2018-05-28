#include <Zanix/Core/ZString.hpp>
#include <Zanix/ZUtils.hpp>

#include <memory>
#include <cstdlib>
#include <cstring>

namespace Zx
{

	/*
	  @brief : Constructs an empty ZString
	 */
	ZString::ZString()
	{
		m_string = std::shared_ptr<String>();
	}

	/*
	  @brief : Contructs an ZString base on "C" string
	  @param : The string
	 */
	ZString::ZString(const char* string)
	{
		if (string)
		{
			PutZString(string, strlen(string));
		}
	}

	/*
	  @brief : Constructs an ZString with a character
	  @param : The character
	 */
	ZString::ZString(char character)
	{
		m_string = std::make_shared<String>(1);
		m_string->str[0] = character;
	}

	/*
	  @brief : Copy constructor
	  @param : A constant reference to the ZString to copy
	 */
	ZString::ZString(const ZString& string)
	{
		PutZString(string.GetPtr(), string.GetSize());
	}

	/*
	@brief : Constructs ZString with std::string
	@param : A constant reference to the std::string to copy
	*/
	ZString::ZString(const std::string& string)
	{
		PutZString(string.c_str(), string.size());
	}

	/*
	  @brief : Movement construct
	  @param : The ZString to move
	 */
	ZString::ZString(ZString&& string) noexcept
	{
		m_string = std::make_shared<String>(string.GetSize());
		std::swap(m_string, string.m_string);
	}

	/*
	  @brief : The destructor
	 */
	ZString::~ZString()
	{
		m_string = nullptr;
	}

	/*
	  @brief : Gets the size of a ZString
	 */
	std::size_t ZString::GetSize() const
	{
		return (m_string->size);
	}

	/*
	  @brief : Gets a pointer to the string
	 */
	char* ZString::GetPtr() const
	{
		return (m_string->str.get());
	}

	/*
	  @brief : Returns true if the ZString is empty, false otherwise
	 */
	bool ZString::IsEmpty() const
	{
		return (GetSize() == 0);
	}

	/*
	  @brief : Returns null if a ZString is null, false otherwise
	 */
	bool ZString::IsNull() const
	{
		return (m_string == nullptr);
	}

	/*
	  @brief : Inserts a "C" string into this ZString
	  @param : The string to insert
	  @param : The position where it should be inserted
	 */
	void ZString::Insert(const char * string, std::size_t pos)
	{
		if (pos > GetSize())
		{
			std::cout << "Error: Position [" << pos << "] invalid !" << std::endl;
			return;
		}

		auto buffer = std::make_shared<ZString>(*this);

		m_string = std::make_shared<String>(std::strlen(string) + GetSize());
		std::memcpy(GetPtr(), buffer->GetPtr(), pos);
		std::memcpy(&m_string->str[pos], string, std::strlen(string));
	}

	/*
	  @brief : Inserts a character into this ZString
	  @param : The character to insert
	  @param : The position where it should be inserted
	 */
	void ZString::Insert(char character, std::size_t pos)
	{
		Insert(&character, pos);
	}

	/*
	  @brief : Inserts a ZString into this ZString
	  @param : A constant reference to the ZString to insert
	  @param : The position where it should be inserted
	 */
	void ZString::Insert(const ZString& string, std::size_t pos)
	{
		Insert(string.GetPtr(), pos);
	}

	/*
	  @brief : Inserts a std::string into this ZString
	  @param : A constant reference to the std::string to insert
	  @param : The position where it should be inserted
	*/
	void ZString::Insert(const std::string& string, std::size_t pos)
	{
		Insert(string.c_str(), pos);
	}

	/*
	  @brief : Replaces a ZString by a "C" string
	  @param : The new "C" string
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of ZString - 1
	 */
	void ZString::Replace(const char* string, std::size_t begin)
	{
		ZAssert(begin <= GetSize() - 1, "The beginning of the replacement is overflowing");

		auto buffer = std::make_shared<ZString>(*this);

		m_string = std::make_shared<String>(begin + std::strlen(string));
		std::memcpy(GetPtr(), buffer->m_string->str.get(), buffer->GetSize());
		std::memcpy(&m_string->str[begin], string, std::strlen(string));
	}

	/*
	 @brief : Replaces a ZString by a ZString
	 @param : The new ZString
	 @param : The position to begin the replace
	 @Note :An exception is throw if begin is superior to the size of ZString - 1
	*/
	void ZString::Replace(const ZString& string, std::size_t begin)
	{
		Replace(string.GetPtr(), begin);
	}

	/*
	  @brief : Replaces a ZString by a character
	  @param : The new character
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of ZString - 1
	*/
	void ZString::Replace(char character, std::size_t begin)
	{
		Replace(&character, begin);
	}

	/*
	  @brief : Replaces a ZString by a std::string
	  @param : The new std::string
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of ZString - 1
	*/
	void ZString::Replace(std::string string, std::size_t begin)
	{
		Replace(string.c_str(), begin);
	}

	/*
	  @brief : Returns true if the "C" string is contain into this ZString, false otherwise
	  @param : The "C" string to search
	  @param : A pointer to where to start the search
	 */
	bool ZString::Search(const char* string, std::size_t* pos) const
	{
		for (std::size_t i = 0; i < GetSize(); i++)
		{
			if (m_string->str[i] == string[0])
			{
				for (std::size_t j = 0; j < std::strlen(string); j++)
				{
					if (m_string->str[i + j] != string[j])
						break;

					if (j == std::strlen(string) - 1)
					{
						*pos = i;
						return true;
					}
				}
			}
		}

		return false;
	}

	/*
	  @brief : Returns true if the character is contain into this ZString, false otherwise
	  @param : The character to search
	  @param : A pointer to where to start the search
	 */
	bool ZString::Search(char character, std::size_t* pos) const
	{
		for (std::size_t i = 0; i < GetSize(); i++)
		{
			if (m_string->str[i] == character) {
				*pos = i;
				return true;
			}
		}

		return false;
	}

	/*
	  @brief : Returns true if the ZString is contain into this ZString, false otherwise
	  @param : A constant reference to the ZString to search
	  @param : A pointer to where to start the search
	 */
	bool ZString::Search(const ZString& string, std::size_t* pos) const
	{
		return (Search(string.GetPtr(), pos));
	}

	/*
	  @brief : Returns true if the std::string is contain into this ZString, false otherwise
	  @param : A constant reference to the std::string to search
	  @param : A pointer to where to start the search
	*/
	bool ZString::Search(const std::string& string, std::size_t* pos) const
	{
		return (Search(string.c_str(), pos));
	}

	/*
	  @brief : Returns a upper character
	  @param : The character
	*/
	char ZString::ToUpper(char character)
	{
		if ((character >= 'a') && (character <= 'z'))
			return (('Z' - 'z') + character);

		return (character);
	}

	/*
	  @brief : Returns a upper ZString
	  @param : A constant reference to the ZString 
	*/
	ZString ZString::ToUpper(const ZString& string)
	{
		ZString s;
		s.m_string = std::make_shared<String>(string.GetSize());

		for (std::size_t i = 0; i < string.GetSize(); i++)
		{
			s.m_string->str[i] = ToUpper(string.m_string->str[i]);
		}

		return (s);
	}

	/*
	  @brief : Returns a lower character
	  @param : The character
	*/
	char ZString::ToLower(char character)
	{
		if ((character >= 'A') && (character <= 'Z'))
			return (('a' - 'A') + character);

		return (character);
	}

	/*
	  @brief : Returns a upper ZString
	  @param : A constant reference to the ZString
	*/
	ZString ZString::ToLower(const ZString& string)
	{
		ZString s;
		s.m_string = std::make_shared<String>(string.GetSize());

		for (std::size_t i = 0; i < string.GetSize(); i++)
		{
			s.m_string->str[i] = ToLower(string.m_string->str[i]);
		}

		return (s);
	}

	/*
	@brief : Deletes the content of a ZString
	*/
	void ZString::Clear()
	{
		m_string = std::shared_ptr<String>();
	}

	//-------------------------------------------------------------------
	/*
	  @brief : Displays a ZString to the out stream
	  @param : The stream to display it
	  @param : A constant reference to the ZString to display
	*/
	std::ostream& operator<<(std::ostream& stream, const ZString& string)
	{
		if (string.IsNull())
			return (stream << "Error : ZString invalid (nullptr)");
		return (stream << string.GetPtr());
	}

	/*
	  @brief : Affects a ZString to this ZString
	  @param : A constant reference to the ZString to copy
	*/
	ZString& ZString::operator=(const ZString& string)
	{
		if (!m_string)
		{
			PutZString(string.GetPtr(), string.GetSize());
		}
		else
			m_string = string.m_string;

		return (*this);
	}

	/*
	  @brief : Affects a "C" string to this ZString
	  @param : The "C" string to copy
	*/
	ZString& ZString::operator=(const char* string)
	{
		PutZString(string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Moves the ZString to this ZString
	  @param : The ZString to move
	*/
	ZString& ZString::operator=(ZString&& string)
	{
		int size = string.GetSize();
		m_string = std::make_shared<String>(string.GetSize());
		std::swap(m_string, string.m_string);

		return (*this);
	}

	/*
	@brief : Affects a std::string to this ZString
	@param : The std::string to copy
	*/
	ZString& ZString::operator=(const std::string& string)
	{
		PutZString(string.c_str(), string.size());

		return (*this);
	}

	/*
	  @brief : Adds a ZString to the current object
	  @param : A constant reference to the ZString to add
	*/
	ZString ZString::operator+(const ZString& string) const
	{
		if (GetSize() == 0)
			return string;

		if (string.GetSize() == 0)
			return *this;

		ZString s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a "C" string to the current object
	  @param : The "C" string to add
	*/
	ZString ZString::operator+(const char* string) const
	{
		if (GetSize() == 0)
			return ZString(string);

		if (strlen(string) == 0)
			return *this;

		ZString s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a character to this ZString
	  @param : The character to add
	*/
	ZString ZString::operator+(char character) const
	{
		if (GetSize() == 0)
			return ZString(character);

		ZString s(*this);
		s.Insert(character, GetSize());

		return (s);
	}

	/*
	@brief : Adds a std::string to this ZString
	@param : The std::string to add
	*/
	ZString ZString::operator+(std::string string) const
	{
		if (GetSize() == 0)
			return ZString(string);

		ZString s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a ZString to this ZString (macro)
	  @param : A constant reference to the ZString to add
	*/
	ZString& ZString::operator+=(const ZString& string)
	{
		std::memcpy(&m_string->str[GetSize()], string.GetPtr(), string.GetSize());

		return (*this);
	}

	/*
	  @brief : Adds a "C" string to this ZString (macro)
	  @param : The "C" string to add
	*/
	ZString& ZString::operator+=(const char* string)
	{
		std::memcpy(&m_string->str[GetSize()], string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Adds a character to this ZString (macro)
	  @param : The character to add
	*/
	ZString& ZString::operator+=(char character)
	{
		std::memcpy(&m_string->str[GetSize()], &character, std::strlen(&character));

		return (*this);
	}

	/*
	@brief : Adds a std::string to this ZString (macro)
	@param : The std::string to add
	*/
	ZString& ZString::operator+=(const std::string& string)
	{
		std::memcpy(&m_string->str[GetSize()], string.c_str(), string.size());

		return (*this);
	}

	/*
	  @brief : Returns true if the "C" string is equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool ZString::operator==(const char* string) const
	{
		return (std::strcmp(string, GetPtr()) == 0);
	}

	/*
	  @brief : Returns true if the ZString is equal to the current object, false otherwise
	  @param : A constant reference to the ZString to compare
	*/
	bool ZString::operator==(const ZString& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) == 0);
	}

	/*
	  @brief : Returns true if the character is equal to the current object, false otherwise
	  @param : The character to compare
	*/
	bool ZString::operator==(char character) const
	{
		return (character == m_string->str[0]);
	}

	/*
      @brief : Returns true if this ZString is equal to a std::string
	  @param : The std::string to compare
	*/
	bool ZString::operator==(std::string string) const
	{
		return (std::strcmp(GetPtr(), string.c_str()) == 0);
	}

	/*
	  @brief : Returns true if the "C" string is not equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool ZString::operator!=(const char* string) const
	{
		return (std::strcmp(GetPtr(), string) != 0);
	}

	/*
	  @brief : Returns true if the ZString is not equal to this ZString, false otherwise
	  @param : A constant reference to the ZString to compare
	*/
	bool ZString::operator!=(const ZString& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) != 0);
	}

	/*
	  @brief : Returns true if the character is not equal to this ZString, false otherwise
	  @param : The character to compare
	*/
	bool ZString::operator!=(char character) const
	{
		return (m_string->str[0] != character);
	}

	/*
	@brief : Returns true if the std::string is not equal to this ZString, false otherwise
	@param : The std::string to compare
	*/
	bool ZString::operator!=(const std::string& string) const
	{
		return (std::strcmp(GetPtr(), string.c_str()) == 0);
	}

	//Private method
	void ZString::PutZString(const char* string, std::size_t size)
	{
		m_string = std::make_shared<String>(size);
		std::memcpy(GetPtr(), string, size);
	}
}
