/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_STRING_H_
#define _CPPF_STRING_H_

#include <string.h>
#include <stdlib.h>
#include <string>

#include "cppfan/base/Object.h"
#include "cppfan/base/Ref.h"

CF_BEGIN_NAMESPACE

class String : public Object {
public:
  std::wstring str;
public:
  String() {
  }

  String(const char *cstr);

  String(const wchar_t *cstr) : str(cstr) {
  }
  
  String(const std::wstring &cstr) : str(cstr) {
  }
  
  String(const std::wstring &&cstr) : str(cstr) {
  }
  
  String(const String &s) : str(s.str) {
  }
  
  String(String &&s) : str(std::move(s.str)) {
  }
  
  String& operator=(const String &s) {
    str = s.str;
    return *this;
  }
  
  String& operator=(const String &&s) {
    str = std::move(s.str);
    return *this;
  }
  
  std::wstring &stdstr() { return str; }
  const std::wstring &stdstr() const { return str; }
  
  const wchar_t *c_str() const { return str.c_str(); }
  
  size_t size() const { return str.size(); }
  
  wchar_t operator[](size_t i) { return str.at(i); }

  bool isEmpty() const { return size() == 0; }
  
  void clear() { str.clear(); }
  
  void reserve(size_t s) { str.resize(s); }
  
  void reseize(size_t s) { str.resize(s); }

  int toInt() const { return (int)wcstol(c_str(), NULL, 10); }
  
  int64_t toLong() const;

  float toFloat() const { return wcstof(c_str(), NULL); }

  double toDouble() const { return wcstod(c_str(), NULL); }

  virtual size_t hashCode() const;
  virtual int compare(const Object &other) const { return (*this) - (const String &)other; }
  virtual bool equals(const Object &other) const { return (*this) == (const String &)other; }
  
  int operator-(const String &s) const { return str.compare(s.str); }
  
  String &operator+=(const wchar_t *s) { return operator+=(String(s)); }
  
  String &operator+=(const String &s);
  
  String operator+(const wchar_t *s) { return operator+(String(s)); }
  
  String operator+(const String &s);
  
  bool operator==(const String &other) const { return str == other.str; }
  
  /*equals ignore case */
  bool iequals(const String &other) const;
  
  bool contains(const String& s) const { return index(s) >= 0; }
  
  long index(const String& s, size_t pos = 0) const { return str.find(s.str, pos); }
  
  long lastIndex(const String& s) const { return str.rfind(s.str); }
  
  bool startsWith(const String& s) const { return index(s) == 0; }
  
  bool endsWith(const String& s) const { return lastIndex(s) == (size()-s.size()); }
  
  void replace(const String& src, const String& dst);
  
  std::vector<String> split(const String &sep) const;
  
  String substr(size_t pos, size_t len = -1) const;
  
  void trimEnd();
  
  void trimStart();
  
  void trim() { trimStart(); trimEnd(); }
  
  /**
   * 移除最后一个字
   */
  void removeLastChar();
  
  /**
   * Return this string with all uppercase characters replaced to lowercase
   */
  String toLower() const;
  
  /**
   * Return this string with all lowercase characters replaced to uppercase
   */
  String toUpper() const;
  
  /**
   * convert to UTF-8 charset encode
   */
  std::string toUtf8();
  static String fromUtf8(const char *);
  
  /**
   * make from int
   */
  static String fromInt(int i);
  static String fromInt(int64_t i);
  
  /**
   * make from float
   */
  static String fromFloat(double f);
  static String fromFloat(float f);
  
  /**
   * 'printf' style format
   */
  static String format(const wchar_t *fmt, ...);
  
  void print();
};

typedef String Str;
/*
inline Str operator +( const Str& lhs, const Str& rhs )
{
  Str result(lhs) ;
  
  result += rhs;
  
  return result;
}

inline bool operator == ( const Str& lhs, const Str& rhs )
{
  return lhs.stdstr() == rhs.stdstr();
}

inline bool operator !=( const Str& lhs, const Str& rhs )
{
  return lhs.stdstr() != rhs.stdstr();
}

inline bool operator<(const Str& lhs, const Str& rhs) {
  return lhs.stdstr() < rhs.stdstr();
}
*/

class StrRef : public Ref<Str> {
public:
  StrRef() : Ref<Str>(new Str()){}
  StrRef(const wchar_t *cstr) : Ref<Str>(new Str(cstr)) {
  }
  StrRef(const char *cstr) : Ref<Str>(new Str(cstr)) {
  }

  StrRef &operator+=(const wchar_t *s) {
    (*get())+=(s);
    return *this;
  }

  bool operator== (const StrRef& r) const {
    const Str *self = (const_cast<StrRef*>(this))->get();
    return self->operator== (*((StrRef&)r).get());
  }
};



CF_END_NAMESPACE
#endif // STRING_H
