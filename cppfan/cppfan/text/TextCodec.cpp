//
//  TextCodec.cpp
//  cppfan
//
//  Created by 杨建东 on 2016/12/22.
//  Copyright © 2016年 yangjiandong. All rights reserved.
//

#include "TextCodec.h"

CF_USING_NAMESPACE

#ifdef CF_WIN

#include <stdio.h>
#include <windows.h>
#include <locale.h>

int TextCodec::utf8ToUnicode(const char *s, wchar_t *out, int limit ) {
  return MultiByteToWideChar(CP_UTF8,0,s,-1,out,limit);
}

int TextCodec::unicodeToUtf8(const wchar_t *w, char *out, int limit ) {
  return WideCharToMultiByte( CP_UTF8, 0, w, -1, out, limit, NULL, NULL );
}

int TextCodec::ansiToUnicode( const char *s, wchar_t *out, int limit ) {
  return MultiByteToWideChar(CP_ACP,0,s,-1,out,limit);
}

int TextCodec::unicodeToAnsi( const wchar_t *w, char *out, int limit ) {
  return WideCharToMultiByte(CP_ACP, 0, w, -1, out, limit, NULL, NULL );
}

int TextCodec::ansiToUtf8( const char *in, char *out, int limit ) {
  int rc;
  wchar_t *wbuf = new wchar_t[limit];
  rc = TextCodec::ansiToUnicode(in, wbuf, limit);
  if (rc == 0) return 0;
  wbuf[limit-1] = 0;
  rc = TextCodec::unicodeToUtf8(wbuf, out, limit);
  out[limit-1] = 0;
  delete[] wbuf;
  return rc;
}

int TextCodec::utf8ToAnsi( const char *in, char *out, int limit ) {
  int rc;
  wchar_t *wbuf = new wchar_t[limit];
  rc = TextCodec::utf8ToUnicode(in, wbuf, limit);
  if (rc == 0) return 0;
  wbuf[limit-1] = 0;
  rc = TextCodec::nicodeToAnsi(wbuf, out, limit);
  out[limit-1] = 0;
  delete[] wbuf;
  return rc;
}

#else

size_t TextCodec::utf8ToUnicode(const char *s, wchar_t *out, size_t limit ) {
  return ansiToUnicode(s, out, limit);
}
size_t TextCodec::unicodeToUtf8(const wchar_t *w, char *out, size_t limit ) {
  return unicodeToAnsi(w, out, limit);
}

size_t TextCodec::ansiToUnicode(const char *s, wchar_t *out, size_t limit ) {
  return mbstowcs(out, s, limit) ;
}
size_t TextCodec::unicodeToAnsi(const wchar_t *w, char *out, size_t limit ) {
  return wcstombs(out, w, limit) ;
}

size_t ansiToUtf8(const char *in, char *out, size_t limit ) {
  int i=0;
  for (; i<limit; ++i) {
    char c = in[i];
    out[i] = c;
    if (!c) {
      return i-1;
    }
  }
  out[i-1] = 0;
  return i-1;
}

size_t utf8ToAnsi(const char *in, char *out, size_t limit ) {
  int i=0;
  for (; i<limit; ++i) {
    char c = in[i];
    out[i] = c;
    if (!c) {
      return i-1;
    }
  }
  out[i-1] = 0;
  return i-1;
}


#endif
