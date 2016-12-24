/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-18  Jed Young  Creation
 */

#ifndef _CF_MEMORYPOOL_H_
#define _CF_MEMORYPOOL_H_


#include "cppfan/base/common.h"
#include "cppfan/mem/Memory.h"

CF_BEGIN_NAMESPACE

/**
 * Memory Pool is a Fixed size alloctor, support fast memory alloc.
 *
 */
class MemPool {
public:
  /**
   * Memory Pool Item
   *
   */
  struct Elem {
    const char *name;
    bool isBuffered;
    struct Elem *next;
    int checkCode;
  };
  
private:

  size_t    size;
  size_t    itemSize;
  size_t    allocCount;
  Elem *list;
  char *buffer;

public:
  /**
   * constructor
   */
  MemPool(size_t objSize, size_t objCount);
  ~MemPool();

  /**
   * alloc from pool, please using cf_MemoryPool_alloc replace.
   */
  void *alloc(const char *name);

  /**
   * free memory which alloc from this pool.
   */
  void mfree(void *p);
  
};

CF_END_NAMESPACE

#endif

