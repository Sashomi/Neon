#include <Zanix/Core/Exception.hpp>
#include <Zanix/Core/String.hpp>

#include <memory>
#include <cstdlib>
#include <cstring>

namespace Zx
{

	/*
	  @brief : Constructs an empty String
	 */
	String::String()
	{
		m_string = std::shared_ptr<Str>();
	}

	/*
	  @brief : Contructs an String base on "C" string
	  @param : The string
	 */
	String::String(const char* string)
	{
		if (string)
		{
			PutString(string, strlen(string));
		}
	}

	/*
	  @brief : Constructs an String with a character
	  @param : The character
	 */
	String::String(char character)
	{
		m_string = std::make_shared<Str>(1);
		m_string->str[0] = character;
	}

	/*
	  @brief : Copy constructor
	  @param : A constant reference to the String to copy
	 */
	String::String(const String& string)
	{
		PutString(string.GetPtr(), string.GetSize());
	}

	/*
	@brief : Constructs String with std::string
	@param : A constant reference to the std::string to copy
	*/
	String::String(const std::string& string)
	{
		PutString(string.c_str(), string.size());
	}

	/*
	  @brief : Movement construct
	  @param : The String to move
	 */
	String::String(String&& string) noexcept
	{
		m_string = std::make_shared<Str>(string.GetSize());
		std::swap(m_string, string.m_string);
	}

	/*
	  @brief : The destructor
	 */
	String::~String()
	{
		m_string = nullptr;
	}

	/*
	  @brief : Gets the size of a String
	 */
	std::size_t String::GetSize() const
	{
		return (m_string->size);
	}

	/*
	  @brief : Gets a pointer to the string
	 */
	char* String::GetPtr() const
	{
		return (m_string->str.get());
	}

	/*
	  @brief : Returns true if the String is empty, false otherwise
	 */
	bool String::IsEmpty() const
	{
		return (GetSize() == 0);
	}

	/*
	  @brief : Returns null if a String is null, false otherwise
	 */
	bool String::IsNull() const
	{
		return (m_string == nullptr);
	}

	/*
	  @brief : Inserts a "C" string into this String
	  @param : The string to insert
	  @param : The position where it should be inserted
	 */
	void String::Insert(const char * string, std::size_t pos)
	{
		if (pos > GetSize())
		{
			std::cout << "Error: Position [" << pos << "] invalid !" << std::endl;
			return;
		}

		auto buffer = std::make_shared<String>(*this);

		m_string = std::make_shared<Str>(std::strlen(string) + GetSize());
		std::memcpy(GetPtr(), buffer->GetPtr(), pos);
		std::memcpy(&m_string->str[pos], string, std::strlen(string));
	}

	/*
	  @brief : Inserts a character into this String
	  @param : The character to insert
	  @param : The position where it should be inserted
	 */
	void String::Insert(char character, std::size_t pos)
	{
		Insert(&character, pos);
	}

	/*
	  @brief : Inserts a String into this String
	  @param : A constant reference to the String to insert
	  @param : The position where it should be inserted
	 */
	void String::Insert(const String& string, std::size_t pos)
	{
		Insert(string.GetPtr(), pos);
	}

	/*
	  @brief : Inserts a std::string into this String
	  @param : A constant reference to the std::string to insert
	  @param : The position where it should be inserted
	*/
	void String::Insert(const std::string& string, std::size_t pos)
	{
		Insert(string.c_str(), pos);
	}

	/*
	  @brief : Replaces a String by a "C" string
	  @param : The new "C" string
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of String - 1
	 */
	void String::Replace(const char* string, std::size_t begin)
	{
		ZAssert(begin <= GetSize() - 1, "The beginning of the replacement is overflowing");

		auto buffer = std::make_shared<String>(*this);

		m_string = std::make_shared<Str>(begin + std::strlen(string));
		std::memcpy(GetPtr(), buffer->m_string->str.get(), buffer->GetSize());
		std::memcpy(&m_string->str[begin], string, std::strlen(string));
	}

	/*
	 @brief : Replaces a String by a String
	 @param : The new String
	 @param : The position to begin the replace
	 @Note :An exception is throw if begin is superior to the size of String - 1
	*/
	void String::Replace(const String& string, std::size_t begin)
	{
		Replace(string.GetPtr(), begin);
	}

	/*
	  @brief : Replaces a String by a character
	  @param : The new character
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of String - 1
	*/
	void String::Replace(char character, std::size_t begin)
	{
		Replace(&character, begin);
	}

	/*
	  @brief : Replaces a String by a std::string
	  @param : The new std::string
	  @param : The position to begin the replace
	  @Note : An exception is throw if begin is superior to the size of String - 1
	*/
	void String::Replace(std::string string, std::size_t begin)
	{
		Replace(string.c_str(), begin);
	}

	/*
	  @brief : Returns true if the "C" string is contain into this String, false otherwise
	  @param : The "C" string to search
	  @param : A pointer to where to start the search
	 */
	bool String::Search(const char* string, std::size_t* pos) const
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
	  @brief : Returns true if the character is contain into this String, false otherwise
	  @param : The character to search
	  @param : A pointer to where to start the search
	 */
	bool String::Search(char character, std::size_t* pos) const
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
	  @brief : Returns true if the String is contain into this String, false otherwise
	  @param : A constant reference to the String to search
	  @param : A pointer to where to start the search
	 */
	bool String::Search(const String& string, std::size_t* pos) const
	{
		return (Search(string.GetPtr(), pos));
	}

	/*
	  @brief : Returns true if the std::string is contain into this String, false otherwise
	  @param : A constant reference to the std::string to search
	  @param : A pointer to where to start the search
	*/
	bool String::Search(const std::string& string, std::size_t* pos) const
	{
		return (Search(string.c_str(), pos));
	}

	/*
	  @brief : Returns a upper character
	  @param : The character
	*/
	char String::ToUpper(char character)
	{
		if ((character >= 'a') && (character <= 'z'))
			return (('Z' - 'z') + character);

		return (character);
	}

	/*
	  @brief : Returns a upper String
	  @param : A constant reference to the String 
	*/
	String String::ToUpper(const String& string)
	{
		String s;
		s.m_string = std::make_shared<Str>(string.GetSize());

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
	char String::ToLower(char character)
	{
		if ((character >= 'A') && (character <= 'Z'))
			return (('a' - 'A') + character);

		return (character);
	}

	/*
	  @brief : Returns a upper String
	  @param : A constant reference to the String
	*/
	String String::ToLower(const String& string)
	{
		String s;
		s.m_string = std::make_shared<Str>(string.GetSize());

		for (std::size_t i = 0; i < string.GetSize(); i++)
		{
			s.m_string->str[i] = ToLower(string.m_string->str[i]);
		}

		return (s);
	}

	/*
	@brief : Deletes the content of a String
	*/
	void String::Clear()
	{
		m_string = std::shared_ptr<Str>();
	}

	//-------------------------------------------------------------------
	/*
	  @brief : Displays a String to the out stream
	  @param : The stream to display it
	  @param : A constant reference to the String to display
	*/
	std::ostream& operator<<(std::ostream& stream, const String& string)
	{
		if (string.IsNull())
			return (stream << "Error : String invalid (nullptr)");
		return (stream << string.GetPtr());
	}

	/*
	  @brief : Affects a String to this String
	  @param : A constant reference to the String to copy
	*/
	String& String::operator=(const String& string)
	{
		if (!m_string)
		{
			PutString(string.GetPtr(), string.GetSize());
		}
		else
			m_string = string.m_string;

		return (*this);
	}

	/*
	  @brief : Affects a "C" string to this String
	  @param : The "C" string to copy
	*/
	String& String::operator=(const char* string)
	{
		PutString(string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Moves the String to this String
	  @param : The String to move
	*/
	String& String::operator=(String&& string)
	{
		std::size_t size = string.GetSize();
		m_string = std::make_shared<Str>(string.GetSize());
		std::swap(m_string, string.m_string);

		return (*this);
	}

	/*
	@brief : Affects a std::string to this String
	@param : The std::string to copy
	*/
	String& String::operator=(const std::string& string)
	{
		PutString(string.c_str(), string.size());

		return (*this);
	}

	/*
	  @brief : Adds a String to the current object
	  @param : A constant reference to the String to add
	*/
	String String::operator+(const String& string) const
	{
		if (GetSize() == 0)
			return string;

		if (string.GetSize() == 0)
			return *this;

		String s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a "C" string to the current object
	  @param : The "C" string to add
	*/
	String String::operator+(const char* string) const
	{
		if (GetSize() == 0)
			return String(string);

		if (strlen(string) == 0)
			return *this;

		String s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a character to this String
	  @param : The character to add
	*/
	String String::operator+(char character) const
	{
		if (GetSize() == 0)
			return String(character);

		String s(*this);
		s.Insert(character, GetSize());

		return (s);
	}

	/*
	@brief : Adds a std::string to this String
	@param : The std::string to add
	*/
	String String::operator+(std::string string) const
	{
		if (GetSize() == 0)
			return String(string);

		String s(*this);
		s.Insert(string, GetSize());

		return (s);
	}

	/*
	  @brief : Adds a String to this String (macro)
	  @param : A constant reference to the String to add
	*/
	String& String::operator+=(const String& string)
	{
		std::memcpy(&m_string->str[GetSize()], string.GetPtr(), string.GetSize());

		return (*this);
	}

	/*
	  @brief : Adds a "C" string to this String (macro)
	  @param : The "C" string to add
	*/
	String& String::operator+=(const char* string)
	{
		std::memcpy(&m_string->str[GetSize()], string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Adds a character to this String (macro)
	  @param : The character to add
	*/
	String& String::operator+=(char character)
	{
		std::memcpy(&m_string->str[GetSize()], &character, std::strlen(&character));

		return (*this);
	}

	/*
	@brief : Adds a std::string to this String (macro)
	@param : The std::string to add
	*/
	String& String::operator+=(const std::string& string)
	{
		std::memcpy(&m_string->str[GetSize()], string.c_str(), string.size());

		return (*this);
	}

	/*
	  @brief : Returns true if the "C" string is equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool String::operator==(const char* string) const
	{
		return (std::strcmp(string, GetPtr()) == 0);
	}

	/*
	  @brief : Returns true if the String is equal to the current object, false otherwise
	  @param : A constant reference to the String to compare
	*/
	bool String::operator==(const String& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) == 0);
	}

	/*
	  @brief : Returns true if the character is equal to the current object, false otherwise
	  @param : The character to compare
	*/
	bool String::operator==(char character) const
	{
		return (character == m_string->str[0]);
	}

	/*
      @brief : Returns true if this String is equal to a std::string
	  @param : The std::string to compare
	*/
	bool String::operator==(std::string string) const
	{
		return (std::strcmp(GetPtr(), string.c_str()) == 0);
	}

	/*
	  @brief : Returns true if the "C" string is not equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool String::operator!=(const char* string) const
	{
		return (std::strcmp(GetPtr(), string) != 0);
	}

	/*
	  @brief : Returns true if the String is not equal to this String, false otherwise
	  @param : A constant reference to the String to compare
	*/
	bool String::operator!=(const String& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) != 0);
	}

	/*
	  @brief : Returns true if the character is not equal to this String, false otherwise
	  @param : The character to compare
	*/
	bool String::operator!=(char character) const
	{
		return (m_string->str[0] != character);
	}

	/*
	@brief : Returns true if the std::string is not equal to this String, false otherwise
	@param : The std::string to compare
	*/
	bool String::operator!=(const std::string& string) const
	{
		return (std::strcmp(GetPtr(), string.c_str()) == 0);
	}

	//Private method
	void String::PutString(const char* string, std::size_t size)
	{
		m_string = std::make_shared<Str>(size);
		std::memcpy(GetPtr(), string, size);
	}
}
