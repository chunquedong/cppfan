/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MEMORY_H_
#define _CF_MEMORY_H_


#include "cppfan/base/common.h"

#include <stdlib.h>

CF_USING_NAMESPACE

CF_BEGIN


/**
 * actually do memory alloc.
 */
extern void *cf_Memory_malloc(const char *file, const char *func, const unsigned int line, size_t size);

/**
 * actually do clear memory alloc.
 */
void *cf_Memory_calloc(const char *file, const char *func
                       , const unsigned int line, size_t nobj, size_t size);

/**
 * re alloc memory.
 */
void *cf_Memory_realloc(void *p, size_t size);

/**
 * free memory.
 */
void cf_Memory_free(const char *file, const char *func, const unsigned int line, void *p);

/**
 * dump memory leak.
 */
void cf_Memory_dumpMem();

/**
 * check memory overflow.
 */
void cf_Memory_checkAll();

/**
 * actually do memory check.
 */
void cf_Memory_check(const char *file, const char *func, const unsigned int line, void *p);


/*========================================================================
 * checked alloc
 */

static inline void *cf_Memory_checkedMalloc(const char *file
     , const char *func, const unsigned int line, size_t size) {
  void *t;
  t = cf_Memory_malloc(file, func, line, size);
  if (t == NULL) {
    cf_abort("bad malloc");
  }
  return t;
}

static inline void *cf_Memory_stdCheckedMalloc(size_t size) {
  void *t;
  t = malloc(size);
  if (t == NULL) {
    cf_abort("bad malloc");
  }
  return t;
}

static inline void *cf_Memory_checkedCalloc(const char *file
    , const char *func, const unsigned int line, size_t nobj, size_t size) {
  void *t;
  t = cf_Memory_calloc(file, func, line, nobj, size);
  if (t == NULL) {
    cf_abort("bad calloc");
  }
  return t;
}

static inline void *cf_Memory_stdCheckedCalloc(size_t nobj, size_t size) {
  void *t;
  t = calloc(nobj, size);
  if (t == NULL) {
    cf_abort("bad calloc");
  }
  return t;
}

static inline void *cf_Memory_checkedRealloc(void *p, size_t size) {
  void *t;
  t = cf_Memory_realloc(p, size);
  if (t == NULL) {
    cf_abort("bad realloc");
  }
  return t;
}

static inline void *cf_Memory_stdCheckedRealloc(void *p, size_t size) {
  void *t;
  t = realloc(p, size);
  if (t == NULL) {
    cf_abort("bad realloc");
  }
  return t;
}

/*========================================================================
 * Memory manage macro define
 */

#ifdef CF_DEBUG

  #define cf_malloc(size) cf_Memory_malloc(__FILE__, __func__, __LINE__, size)
  #define cf_calloc(nobj, size) cf_Memory_calloc(__FILE__, __func__, __LINE__, nobj, size)
  #define cf_realloc(p, size) cf_Memory_realloc(p, size)
  #define cf_free(p) cf_Memory_free(__FILE__, __func__, __LINE__, p)

  #define cf_checkMem(p) cf_Memory_check(__FILE__, __func__, __LINE__, p)
  #define cf_dumpMem() cf_Memory_dumpMem()
  #define cf_checkAllMem() cf_Memory_checkAll()

  #define cf_checkedMalloc(size) cf_Memory_checkedMalloc(__FILE__, __func__, __LINE__, size)
  #define cf_checkedCalloc(nobj, size) cf_Memory_checkedCalloc(__FILE__, __func__, __LINE__, nobj, size)
  #define cf_checkedRealloc(p, size) cf_Memory_checkedRealloc(p, size)

#else

  #define cf_malloc(size) malloc(size)
  #define cf_calloc(nobj, size) calloc(nobj, size)
  #define cf_realloc(nobj, size) realloc(nobj, size)
  #define cf_free(p) free(p)

  #define cf_checkMem(p)
  #define cf_dumpMem()
  #define cf_checkAllMem()

  #define cf_checkedMalloc(size) cf_Memory_stdCheckedMalloc(size)
  #define cf_checkedCalloc(nobj, size) cf_Memory_stdCheckedCalloc(nobj, size)
  #define cf_checkedRealloc(p, size) cf_Memory_stdCheckedRealloc(p, size)

#endif

/**
 * free and set to null
 */
#define cf_safeFree(p) { cf_free(p); p = NULL; }

/**
 * malloc by the struct type.
 */
#define cf_new(T) ((T*)cf_checkedCalloc(1, sizeof(T)))

/**
 * malloc T array
 */
#define cf_newArray(T, n) ((T*)cf_checkedCalloc(n, sizeof(T)))

CF_END

/*======================================================================*/

#ifdef __cplusplus
  #ifdef CF_DEBUG
    #include <new>
    #define CF_OVERRIDE_NEW \
      static void* operator new(size_t sz, void *buf = NULL) throw (std::bad_alloc) {\
        if (buf) return buf;\
        void* mem = cf_Memory_malloc(__FILE__, __func__, __LINE__, sz);\
        if (mem) return mem;\
        else throw std::bad_alloc();\
      }\
      \
      static void operator delete(void* ptr) throw() {\
        cf_Memory_free(__FILE__, __func__, __LINE__, ptr);\
      }\
      \
      static void* operator new[](size_t sz, void *buf = NULL) throw (std::bad_alloc) {\
        if (buf) return buf;\
        void* mem = cf_Memory_malloc(__FILE__, __func__, __LINE__, sz);\
        if (mem) return mem;\
        else throw std::bad_alloc();\
      }\
      \
      static void operator delete[](void* ptr) throw() {\
        cf_Memory_free(__FILE__, __func__, __LINE__, ptr);\
      }\
  \
      static void* operator new(size_t sz, const char *file, const char *func, const unsigned int line) throw (std::bad_alloc) {\
        void* mem = cf_Memory_malloc(file, func, line, sz);\
        if (mem) return mem;\
        else throw std::bad_alloc();\
      }\
      static void* operator new[](size_t sz, const char *file, const char *func, const unsigned int line) throw (std::bad_alloc) {\
        void* mem = cf_Memory_malloc(file, func, line, sz);\
        if (mem) return mem;\
        else throw std::bad_alloc();\
      }

    #define CF_NEW new(__FILE__, __func__, __LINE__)
  #else
    #define CF_OVERRIDE_NEW
    #define CF_NEW new
  #endif  //CF_DEBUG

  //shortcut
  #define _new CF_NEW
#endif //__cplusplus


#endif //_CF_MEMORY_H_
