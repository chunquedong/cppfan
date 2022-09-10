/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MISS_H_
#define _CF_MISS_H_

/*========================================================================
 * WIN
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) \
    || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
  #define CF_WIN
#endif

/*========================================================================
 * Fixed int type
 */

#ifdef  __STDC__
  #include <stdint.h>
#else
  typedef signed char       int8_t;
  typedef signed short      int16_t;
  typedef signed long       int32_t;
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned long     uint32_t;

  #if defined(_MSC_VER) || defined(__BORLANDC__)
    typedef signed __int64      int64_t;
    typedef unsigned __int64    uint64_t;
  #else
    typedef signed long long    int64_t;
    typedef unsigned long long  uint64_t;
  #endif

  #ifndef _MSC_VER
    #define _Bool char
  #endif
#endif

/*========================================================================
 * Boolean
 */

#ifndef __cplusplus
  #define bool    _Bool
  #define true    1
  #define false   0
#endif

/*========================================================================
 * misc
 */
#ifdef  _MSC_VER
  //#define inline      __inline
  #define __func__    __FUNCTION__
  //#define snprintf    _snprintf
  //#define vsnprintf   _vsnprintf
  #define strcasecmp  _stricmp
  #define strtoll     _strtoi64
  #define tzset       _tzset
  #define isnan(x)   _isnan(x)
#endif


#ifndef NULL
  #define NULL ((void*)0)
#endif


//#ifndef offsetof
//  #define offsetof(s, m)   (size_t)&(((s *)0)->m)
//#endif

#endif //_CF_MISS_H_
