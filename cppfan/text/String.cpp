/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cppfan/text/String.h"

#include "TextCodec.h"

#include <functional>
#include <wctype.h>


CF_USING_NAMESPACE

size_t String::hashCode() const {
  std::hash<std::wstring> hash_fn;
  return hash_fn(str);
}

String &String::operator+=(const String &s) {
  (str += s.str);
  return *this;
}

String String::operator+(const String &s) {
  return String(str + s.str);
}

void String::trimEnd() {
  int i = str.size()-1;
  for (; i >=0; --i) {
    if (!iswspace(str[i])) {
      break;
    }
  }

  if (i < str.size()-1) {
    str.erase(str.begin()+i+1, str.end());
  }
}

void String::trimStart() {
  int i = 0;
  for (; i < str.size(); ++i) {
    if (!iswspace(str[i])) {
      break;
    }
  }

  if (i > 0) {
    str.erase(str.begin(), str.begin()+i);
  }
}

bool String::iequals(const String& b) const
{
  size_t sz = size();
  if (b.size() != sz)
    return false;
  for (unsigned int i = 0; i < sz; ++i)
    if (towlower(str[i]) != towlower(b.str[i]))
      return false;
  return true;
}

void String::replace(const String& src, const String& dst)
{
  if (src.compare(dst) == 0) {
    return;
  }
  size_t srcLen = src.size();
  size_t desLen = dst.size();
  size_t pos = index(src);
  
  while ((pos != -1))
  {
    str.replace(pos, srcLen, dst.str);
    pos = index(src, (pos+desLen));
  }
}

String String::substr(size_t pos, size_t len) const {
  return String(str.substr(pos, len));
}

std::vector<String> String::split(const String &sep) const {
  std::vector<String> tokens;
  std::size_t start = 0, end = 0;
  while ((end = index(sep, start)) != -1) {
    tokens.push_back(substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(substr(start));
  return tokens;
}


void String::removeLastChar()
{
  if (str.length() == 0) return;
  str.erase(str.length() - 1);
}

String String::toLower() const
{
  std::wstring ret;
  char chrTemp;
  size_t i;
  for (i = 0; i < str.length(); ++i)
  {
    chrTemp = str[i];
    chrTemp = towlower(chrTemp);
    ret.push_back(chrTemp);
  }
  
  return String(ret);
}

String String::toUpper() const
{
  std::wstring ret;
  char chrTemp;
  size_t i;
  for (i = 0; i < str.length(); ++i)
  {
    chrTemp = str[i];
    chrTemp = towupper(chrTemp);
    ret.push_back(chrTemp);
  }
  
  return String(ret);
}

#define BUF_SIZE 1024

String String::fromInt(int i)
{
  wchar_t buf[BUF_SIZE];
  swprintf(buf, sizeof(buf), L"%d", i);
  
  return String(buf);
}

String String::fromInt(int64_t i)
{
  wchar_t buf[BUF_SIZE];
  swprintf(buf, sizeof(buf), L"%lld", i);
  
  return String(buf);
}

String String::fromFloat(float f)
{
  wchar_t buf[BUF_SIZE];
  swprintf(buf, sizeof(buf), L"%f", f);
  
  return String(buf);
}

String String::fromFloat(double f)
{
  wchar_t buf[BUF_SIZE];
  swprintf(buf, sizeof(buf), L"%f", f);
  
  return String(buf);
}

int64_t String::toLong() const
{
  if (str.empty()) return 0;
  int64_t nValue=0;
  swscanf(str.c_str(),L"%lld",&nValue);
  return nValue;
}

String String::format(const wchar_t *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  wchar_t buf[BUF_SIZE];
  wchar_t *abuf = NULL;
  int i = vswprintf(buf, sizeof(buf), fmt, args);
  
  if (i < 0) {
    va_end(args);
    return L"";
  }
  if (i >= BUF_SIZE) {
    abuf = (wchar_t*)cf_malloc(i+1);
    i = vswprintf(abuf, i, fmt, args);
    if (i < 0) {
      va_end(args);
      return L"";
    }
    if (i>0) {
      String str(abuf);
      cf_free(abuf);
      va_end(args);
      return str;
    }
  }

  va_end(args);
  return String(buf);
}

std::string &String::toUtf8()
{
  size_t size = this->size() * 4 + 1;
  //char *buf = (char*)cf_malloc(size);
  std::string buf;
  buf.resize(size);
  TextCodec::unicodeToUtf8(c_str(), (char*)buf.data(), size);
  size_t realSize = strlen(buf.c_str());
  buf.resize(realSize);
  utf8 = buf;
  return utf8;
}

String String::fromUtf8(const char *d)
{
    if (d == NULL) return String();
    size_t size = strlen(d) + 1;
    wchar_t *buf = (wchar_t*)cf_malloc(size*sizeof(wchar_t));
    TextCodec::utf8ToUnicode(d, buf, size);
    String str(buf);
    cf_free(buf);
    return str;
}

String::String(const char *cstr) {
    size_t size = strlen(cstr) + 1;
    wchar_t *buf = (wchar_t*)cf_malloc(size*sizeof(wchar_t));
    TextCodec::utf8ToUnicode(cstr, buf, size);
    str = buf;
    cf_free(buf);
}

void String::print()
{
  wprintf(L"%ls\n", str.c_str());
}
