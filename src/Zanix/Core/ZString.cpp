#include <Zanix/Core/ZString.hpp>

namespace Zx 
{

	/*
	  @brief : Construct an empty ZString
	 */
	ZString::ZString()
	{
		m_string = std::shared_ptr<String>();
	}

	/*
	  @brief : Contruct an ZString base on "C" string
	  @param : The string
	 */
	ZString::ZString(const char* string)
	{
		if (string)
		{
			m_string = std::make_shared<String>(std::strlen(string));
			std::memcpy(GetPtr(), string, std::strlen(string));
		}
	}

	/*
	  @brief : Construct an ZString with a character
	  @param : The character
	 */
	ZString::ZString(char character)
	{
		m_string = std::make_shared<String>(1);
		m_string->str[0] = character;
	}

	/*
	  @brief : Copy construct
	  @param : The ZString to copy
	 */
	ZString::ZString(const ZString& string)
	{
		m_string = std::make_shared<String>(string.GetSize());
		std::memcpy(GetPtr(), string.GetPtr(), string.GetSize());
	}

	/*
	@brief : Copy construct with std::string
	@param : The std::string to copy
	*/
	ZString::ZString(const std::string& string)
	{
		m_string = std::make_shared<String>(string.size());
		std::memcpy(GetPtr(), string.c_str(), string.size());
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
	  @brief : Get the size of a ZString
	 */
	std::size_t ZString::GetSize() const
	{
		return (m_string->size);
	}

	/*
	  @brief : Get a pointer to the string
	 */
	char* ZString::GetPtr() const
	{
		return (m_string->str.get());
	}

	/*
	  @brief : Return true if the ZString is empty, false otherwise
	 */
	bool ZString::IsEmpty() const
	{
		return (GetSize() == 0);
	}

	/*
	  @brief : Return null if a ZString is null, false otherwise
	 */
	bool ZString::IsNull() const
	{
		return (m_string == nullptr);
	}

	/*
	  @brief : Insert a "C" string into a ZString
	  @param : The string
	  @param : The pos to insert
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
	  @brief : Insert a character into a ZString
	  @param : The character
	  @param : The pos to insert
	 */
	void ZString::Insert(char character, std::size_t pos)
	{
		Insert(&character, pos);
	}

	/*
	  @brief : Insert a ZString into a ZString
	  @param : The ZString
	  @param : The pos to insert
	 */
	void ZString::Insert(const ZString& string, std::size_t pos)
	{
		Insert(string.GetPtr(), pos);
	}

	/*
	  @brief : Replace a ZString by a "C" string
	  @param : The "C" string
	  @param : The position to begin
	  @Note : If begin is superior to the size of ZString - 1
	 */
	void ZString::Replace(const char* string, std::size_t begin)
	{
		if (begin > GetSize() - 1)
		{
			std::cout << "Error : begin of the replace [" << begin << "] invalid !" << std::endl;
			return;
		}

		auto buffer = std::make_shared<ZString>(*this);

		m_string = std::make_shared<String>(begin + std::strlen(string));
		std::memcpy(GetPtr(), buffer->m_string->str.get(), buffer->GetSize());
		std::memcpy(&m_string->str[begin], string, std::strlen(string));
	}

	/*
	 @brief : Replace a ZString by a ZString
	 @param : The ZString
	 @param : The position to begin
	 @Note : If begin is superior to the size of ZString - 1
	*/
	void ZString::Replace(const ZString& string, std::size_t begin)
	{
		Replace(string.GetPtr(), begin);
	}

	/*
	 @brief : Replace a ZString by a character
	 @param : The character
	 @param : The position to begin
	 @Note : If begin is superior to the size of ZString - 1
	*/
	void ZString::Replace(char character, std::size_t begin)
	{
		Replace(&character, begin);
	}

	/*
	  @brief : Return true if the "C" string is contain into ZString, false otherwise
	  @param : The "C" string
	  @param : A pointer to the position of the begin of the string
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
	  @brief : Return true if the character is contain into ZString, false otherwise
	  @param : The character
	  @param : A pointer to the position of the character
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
	  @brief : Return true if the ZString is contain into ZString, false otherwise
	  @param : The ZString
	  @param : A pointer to the position of the begin of the ZString
	 */
	bool ZString::Search(const ZString& string, std::size_t* pos) const
	{
		return (Search(string.GetPtr(), pos));
	}

	/*
	  @brief : Return a upper character
	  @param : The character
	*/
	char ZString::ToUpper(char character)
	{
		if ((character >= 'a') && (character <= 'z'))
			return (('Z' - 'z') + character);

		return (character);
	}

	/*
	  @brief : Return a upper ZString
	  @param : The ZString
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
	  @brief : Return a lower character
	  @param : The character
	*/
	char ZString::ToLower(char character)
	{
		if ((character >= 'A') && (character <= 'Z'))
			return (('a' - 'A') + character);

		return (character);
	}

	/*
	  @brief : Return a upper ZString
	  @param : The ZString
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

	void ZString::Clear()
	{
		m_string = std::shared_ptr<String>();
	}

	//-------------------------------------------------------------------
	/*
	  @brief : Display a ZString
	  @param : The stream to display it
	  @param : The ZString to display
	*/
	std::ostream& operator<<(std::ostream& flux, const ZString& string)
	{
		if (string.IsNull())
			return (flux << "Error : ZString invalid (nullptr)");
		return (flux << string.GetPtr());
	}

	/*
	  @brief : Affect a ZString to the current object
	  @param : The ZString to copy
	*/
	ZString& ZString::operator=(const ZString& string)
	{
		if (!m_string)
		{
			m_string = std::make_shared<String>(string.GetSize());
			std::memcpy(GetPtr(), string.GetPtr(), string.GetSize());
		}
		else
			m_string = string.m_string;

		return (*this);
	}

	/*
	  @brief : Affect a "C" string to the current object
	  @param : The "C" string to copy
	*/
	ZString& ZString::operator=(const char* string)
	{
		m_string = std::make_shared<String>(std::strlen(string));
		std::memcpy(GetPtr(), string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Move the ZString to the current object
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
	  @brief : Add a ZString to the current object
	  @param : The ZString to add
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
	  @brief : Add a "C" string to the current object
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
	  @brief : Add a character to the current object
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
	  @brief : Add a ZString to the current object (macro)
	  @param : The ZString to add
	*/
	ZString& ZString::operator+=(const ZString& string)
	{
		std::memcpy(&m_string->str[GetSize()], string.GetPtr(), string.GetSize());

		return (*this);
	}

	/*
	  @brief : Add a "C" string to the current object (macro)
	  @param : The "C" string to add
	*/
	ZString& ZString::operator+=(const char* string)
	{
		std::memcpy(&m_string->str[GetSize()], string, std::strlen(string));

		return (*this);
	}

	/*
	  @brief : Add a character to the current object (macro)
	  @param : The character to add
	*/
	ZString& ZString::operator+=(char character)
	{
		std::memcpy(&m_string->str[GetSize()], &character, std::strlen(&character));

		return (*this);
	}

	/*
	  @brief : Return true if the "C" string is equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool ZString::operator==(const char* string) const
	{
		return (std::strcmp(string, GetPtr()) == 0);
	}

	/*
	  @brief : Return true if the ZString is equal to the current object, false otherwise
	  @param : The ZString to compare
	*/
	bool ZString::operator==(const ZString& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) == 0);
	}

	/*
	  @brief : Return true if the character is equal to the current object, false otherwise
	  @param : The character to compare
	*/
	bool ZString::operator==(char character) const
	{
		return (character == m_string->str[0]);
	}

	/*
	  @brief : Return true if the "C" string is not equal to the current object, false otherwise
	  @param : The "C" string to compare
	*/
	bool ZString::operator!=(const char* string) const
	{
		return (std::strcmp(GetPtr(), string) != 0);
	}

	/*
	  @brief : Return true if the ZString is not equal to the current object, false otherwise
	  @param : The ZString to compare
	*/
	bool ZString::operator!=(const ZString& string) const
	{
		return (std::strcmp(GetPtr(), string.GetPtr()) != 0);
	}

	/*
	  @brief : Return true if the character is not equal to the current object, false otherwise
	  @param : The character to compare
	*/
	bool ZString::operator!=(char character) const
	{
		return (m_string->str[0] != character);
	}
}
