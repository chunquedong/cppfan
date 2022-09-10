/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_COMMON_H_
#define _CPPF_COMMON_H_

/*========================================================================
 * extern "C"
 */

#ifdef  __cplusplus
  #define CF_BEGIN extern  "C" {
#else
  #define CF_BEGIN
#endif

#ifdef  __cplusplus
  #define CF_END }
#else
  #define CF_END
#endif


/*========================================================================
 * Export API
 */

#undef  CF_API
#ifdef  CF_WIN
  #ifdef CF_EXPORT
    #define CF_API __declspec(dllexport)
  #else
    #define CF_API __declspec(dllimport)
  #endif
#else
  #define CF_API
#endif


/*========================================================================
 * debug macro
 */

#ifdef DEBUG
  #undef NDEBUG
#else
  #undef NDEBUG
  #define NDEBUG
#endif

#ifdef NDEBUG
  #undef CF_DEBUG
#else
  #define CF_DEBUG
#endif

/*========================================================================
 * misc
 */

/**
 * suppress unused warning
 */
#define CF_UNUSED(p) ((void)p)

/*========================================================================
 * align
 */
#define CF_ALIGN_SIZE 4
#define CF_ALIGNN(size, align) (((size)+((align)-1))&~((align)-1))
#define CF_ALIGN(size) CF_ALIGNN(size, CF_ALIGN_SIZE)

/*========================================================================
 * Field
 */
#define CF_FIELD(Type, name) private: Type _##name;\
  public: Type name() const { return _##name; }\
  public: void name(Type name_) { _##name = name_; }\
  private:

#define CF_READONLY_FIELD(Type, name) private: Type _##name;\
  public: Type name() const { return _##name; }\
  private:

#define CF_FIELD_POINTER(Type, name) private: Type _##name;\
  public: Type *name() { return &_##name; }\
  private:

#define CF_FIELD_REF(Type, name) private: Type _##name;\
  public: Type &name() { return _##name; }\
  public: void name(Type &name_) { _##name = name_; }\
  private:

#define CF_FIELD_CONST_REF(Type, name) private: Type _##name;\
  public: const Type &name() const { return _##name; } \
  public: void name(Type &name_) { _##name = name_; }\
  private:

/*========================================================================
 * namespace
 */
#define CF_BEGIN_NAMESPACE namespace cf {

#define CF_END_NAMESPACE }

#define CF_USING_NAMESPACE using namespace cf;


#include "cppfan/base/miss.h"
#include "cppfan/base/Log.h"


#endif // COMMON_H
