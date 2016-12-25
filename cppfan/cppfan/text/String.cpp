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

CF_USING_NAMESPACE

size_t String::hashCode() const {
  std::hash<std::string> hash_fn;
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
  str.erase(std::find_if(str.rbegin(), str.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
}

void String::trimStart() {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))));
}

bool String::iequals(const String& b)
{
  size_t sz = size();
  if (b.size() != sz)
    return false;
  for (unsigned int i = 0; i < sz; ++i)
    if (tolower(str[i]) != tolower(b.str[i]))
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

String String::substr(size_t pos, size_t len) {
  return String(str.substr(pos, len));
}

std::vector<String> String::split(const String &sep) {
  std::vector<String> tokens;
  std::size_t start = 0, end = 0;
  while ((end = index(sep, start)) != -1) {
    tokens.push_back(substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(substr(start));
  return tokens;
}


void Str::removeLastChar()
{
  if (str.length() == 0) return;
  str.erase(str.length() - 1);
}

Str Str::toLower()
{
  std::string ret;
  char chrTemp;
  size_t i;
  for (i = 0; i < str.length(); ++i)
  {
    chrTemp = str[i];
    chrTemp = tolower(chrTemp);
    ret.push_back(chrTemp);
  }
  
  return Str(ret);
}

Str Str::toUpper()
{
  std::string ret;
  char chrTemp;
  size_t i;
  for (i = 0; i < str.length(); ++i)
  {
    chrTemp = str[i];
    chrTemp = toupper(chrTemp);
    ret.push_back(chrTemp);
  }
  
  return Str(ret);
}

#define BUF_SIZE 1024

Str Str::fromInt(int i)
{
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf), "%d", i);
  
  return Str(buf);
}

Str Str::fromInt64(int64_t i)
{
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf), "%lld", i);
  
  return Str(buf);
}

Str Str::fromFloat(float f)
{
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf), "%f", f);
  
  return Str(buf);
}

Str Str::fromFloat(double f)
{
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf), "%f", f);
  
  return Str(buf);
}

int64_t Str::toLong() const
{
  if (str.empty()) return 0;
  int64_t nValue=0;
  sscanf(str.c_str(),"%lld",&nValue);
  return nValue;
}

Str Str::format(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char buf[BUF_SIZE];
  char *abuf = NULL;
  int i = vsnprintf(buf, sizeof(buf), fmt, args);
  
  if (i < 0) {
    va_end(args);
    return "";
  }
  if (i >= BUF_SIZE) {
    abuf = (char*)cf_malloc(i+1);
    i = vsnprintf(abuf, i, fmt, args);
    if (i < 0) {
      va_end(args);
      return "";
    }
    if (i>0) {
      Str str(abuf);
      cf_free(abuf);
      va_end(args);
      return str;
    }
  }

  va_end(args);
  return Str(buf);
}

const char *Str::toUtf8()
{
#ifdef CF_WIN
  size_t size = size() * 2 + 1;
  char *buf = (char*)cf_malloc(size);
  int n = TextCodec::ansiToUtf8(cstr(), buf, size);
  Str str(buf);
  cf_free(buf);
  return str;
#else
  return cstr();
#endif
}

Str Str::fromUtf8(const char *d)
{
#ifdef CF_WIN
  size_t size = strlen(d) + 1;
  char *buf = (char*)cf_malloc(size);
  int n = TextCodec::utf8ToAnsi(d, buf, size);
  Str str(buf);
  cf_free(buf);
  return str;
#else
  return Str(d);
#endif
}

void Str::print()
{
  printf("%s\n", str.c_str());
}
