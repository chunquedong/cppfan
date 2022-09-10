//
//  TextCodec.cpp
//  cppfan
//
//  Created by 杨建东 on 2016/12/22.
//  Copyright © 2016年 yangjiandong. All rights reserved.
//

#include "cppfan/text/TextCodec.h"

CF_USING_NAMESPACE

extern "C" {
#include "utf8.h"

size_t utf8decode(char const *str, wchar_t *des, size_t n, int *illegal) {
    if (n == 0)
        return 0;
    
    char *s = (char *)str;
    size_t i = 0;
    wchar_t uc = 0;
    int r_illegal_all = 0, r_illegal;
    
    while ((uc = getu8c(&s, &r_illegal)))
    {
        if (i < (n - 1))
        {
            des[i++] = uc;
            r_illegal_all += r_illegal;
        }
        else
        {
            break;
        }
    }
    
    des[i] = 0;
    if (illegal)
    {
        *illegal = r_illegal_all + r_illegal;
    }
    
    return i;
}

size_t utf8encode(const wchar_t *us, char *des, size_t n, int *illegal)
{
    if (n == 0)
        return 0;
    
    char *s = des;
    size_t left = n;
    size_t len = 0;
    int r_illegal = 0;
    
    *s = 0;
    while (*us)
    {
        int ret = putu8c(*us, &s, &left);
        if (ret > 0)
        {
            len += ret;
        }
        else if (ret == -1)
        {
            r_illegal += 1;
        }
        else
        {
            break;
        }
        
        ++us;
    }
    
    if (illegal)
    {
        *illegal = r_illegal;
    }
    
    return len;
}
}//extern "C"
size_t TextCodec::utf8ToUnicode(const char *s, wchar_t *out, size_t limit ) {
    return utf8decode(s, out, limit, NULL);
}
size_t TextCodec::unicodeToUtf8(const wchar_t *w, char *out, size_t limit ) {
    return utf8encode(w, out, limit, NULL);
}

////////////////////////////////////////////////////////////////////

#ifdef CF_WIN

#include <stdio.h>
#include <windows.h>
#include <locale.h>

//
//int TextCodec::utf8ToUnicode(const char *s, wchar_t *out, int limit ) {
//  return MultiByteToWideChar(CP_UTF8,0,s,-1,out,limit);
//}
//
//int TextCodec::unicodeToUtf8(const wchar_t *w, char *out, int limit ) {
//  return WideCharToMultiByte( CP_UTF8, 0, w, -1, out, limit, NULL, NULL );
//}

size_t TextCodec::ansiToUnicode( const char *s, wchar_t *out, size_t limit ) {
  return MultiByteToWideChar(CP_ACP,0,s,-1,out,limit);
}

size_t TextCodec::unicodeToAnsi( const wchar_t *w, char *out, size_t limit ) {
  return WideCharToMultiByte(CP_ACP, 0, w, -1, out, limit, NULL, NULL );
}

size_t TextCodec::ansiToUtf8( const char *in, char *out, size_t limit ) {
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

size_t TextCodec::utf8ToAnsi( const char *in, char *out, size_t limit ) {
  int rc;
  wchar_t *wbuf = new wchar_t[limit];
  rc = TextCodec::utf8ToUnicode(in, wbuf, limit);
  if (rc == 0) return 0;
  wbuf[limit-1] = 0;
  rc = TextCodec::unicodeToAnsi(wbuf, out, limit);
  out[limit-1] = 0;
  delete[] wbuf;
  return rc;
}

#else
//
//size_t TextCodec::utf8ToUnicode(const char *s, wchar_t *out, size_t limit ) {
//  return ansiToUnicode(s, out, limit);
//}
//size_t TextCodec::unicodeToUtf8(const wchar_t *w, char *out, size_t limit ) {
//  return unicodeToAnsi(w, out, limit);
//}

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
