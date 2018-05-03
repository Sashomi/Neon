#pragma once

#ifndef ZSTRING_HPP
#define ZSTRING_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

namespace Zx {

  class ZString {
  
  public :
    ZString();
    ZString(const char*);
    ZString(char);
    ZString(const ZString&);
	ZString(const std::string&);
    ZString(ZString&&) noexcept;
    
    virtual ~ZString();
    
    std::size_t GetSize() const;
    
    char* GetPtr() const;    

    bool IsEmpty() const;
    bool IsNull() const;

    void Insert(const char*, std::size_t pos);
    void Insert(char, std::size_t pos);
    void Insert(const ZString&, std::size_t pos);

    void Replace(const char*, std::size_t begin);
    void Replace(const ZString&, std::size_t begin);
    void Replace(char, std::size_t begin);

    bool Search(const char*, std::size_t* pos) const;
    bool Search(char character, std::size_t* pos) const;
    bool Search(const ZString&, std::size_t* pos) const;

	void Clear();

    friend std::ostream& operator<<(std::ostream& flux, const ZString&);

    ZString operator+(const ZString&) const;
    ZString operator+(const char*) const;
    ZString operator+(char) const;

    ZString& operator=(const ZString&);
    ZString& operator=(const char*);
    ZString& operator=(ZString&&);

    ZString& operator+=(const ZString&);
    ZString& operator+=(const char*);
    ZString& operator+=(char);

    bool operator==(const char*) const;
    bool operator==(const ZString&) const;
    bool operator==(char) const;

    bool operator!=(const char*) const;
    bool operator!=(const ZString&) const;
    bool operator!=(char) const;

    static char ToUpper(char);
    static ZString ToUpper(const ZString&);

    static char ToLower(char);
    static ZString ToLower(const ZString&);
  
  private :
    struct String;

    std::shared_ptr<String> m_string;
    
    struct String {
      inline String(std::size_t);    
      
      std::size_t size;
      std::unique_ptr<char[]> str;
    };
  };
  
}

#include "ZString.inl"

#endif //ZSTRING_HPP
