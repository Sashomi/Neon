#pragma once

#ifndef ZSTRING_HPP
#define ZSTRING_HPP

#include <iostream>
#include <string>

namespace Zx
{
	class String
	{
	public:
		String();
		String(const char*);
		String(char);
		String(const String&);
		String(const std::string&);
		String(String&&) noexcept;

		virtual ~String();

		std::size_t GetSize() const;

		char* GetPtr() const;

		bool IsEmpty() const;
		bool IsNull() const;

		void Insert(const char*, std::size_t pos);
		void Insert(char, std::size_t pos);
		void Insert(const String&, std::size_t pos);
		void Insert(const std::string&, std::size_t pos);

		void Replace(const char*, std::size_t begin);
		void Replace(const String&, std::size_t begin);
		void Replace(char, std::size_t begin);
		void Replace(std::string, std::size_t begin);

		bool Search(const char*, std::size_t* pos) const;
		bool Search(char character, std::size_t* pos) const;
		bool Search(const String&, std::size_t* pos) const;
		bool Search(const std::string&, std::size_t* pos) const;

		void Clear();

		friend std::ostream& operator<<(std::ostream& flux, const String&);

		String operator+(const String&) const;
		String operator+(const char*) const;
		String operator+(char) const;
		String operator+(std::string) const;

		String& operator=(const String&);
		String& operator=(const char*);
		String& operator=(String&&);
		String& operator=(const std::string&);

		String& operator+=(const String&);
		String& operator+=(const char*);
		String& operator+=(char);
		String& operator+=(const std::string&);

		bool operator==(const char*) const;
		bool operator==(const String&) const;
		bool operator==(char) const;
		bool operator==(std::string) const;

		bool operator!=(const char*) const;
		bool operator!=(const String&) const;
		bool operator!=(char) const;
		bool operator!=(const std::string&) const;

		static char ToUpper(char);
		static String ToUpper(const String&);

		static char ToLower(char);
		static String ToLower(const String&);

	private:
		struct Str;

		void PutString(const char*, std::size_t);

		std::shared_ptr<Str> m_string;

		struct Str 
		{
			inline Str(std::size_t);

			std::size_t size;
			std::unique_ptr<char[]> str;
		};
	};
}

#include "String.inl"

#endif //ZSTRING_HPP
