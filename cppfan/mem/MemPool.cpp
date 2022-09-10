/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-18  Jed Young  Creation
 */

#include "cppfan/mem/MemPool.h"
//#include "cppfan/Trace.h"
#include <stdio.h>

CF_BEGIN_NAMESPACE

/**
 * a magic code.
 */
#define cf_MemoryPool_headCheckCode 0xA9C5

MemPool::MemPool(size_t objSize, size_t objCount) {
  size_t i;
  Elem *last;
  Elem *item;
  //CF_ENTRY_FUNC

  this->itemSize = CF_ALIGN(objSize+sizeof(Elem));
  this->buffer = (char *)cf_checkedMalloc((itemSize) * objCount);
  this->size = objCount;
  last = (Elem *)this->buffer;
  last->isBuffered = true;
  last->next = NULL;
  last->name = NULL;
  last->checkCode = cf_MemoryPool_headCheckCode;
  this->list = last;
  for (i=1; i<objCount; ++i) {
    item = (Elem *)(this->buffer + i*this->itemSize);
    item->isBuffered = true;
    item->next = NULL;
    item->name = NULL;
    item->checkCode = cf_MemoryPool_headCheckCode;
    last->next = item;
    last = item;
  }
  this->allocCount = 0;
  //CF_EXIT_FUNC
}

static inline void cf_MemoryPool_doCheck_(MemPool::Elem *item) {
  if (item->checkCode != cf_MemoryPool_headCheckCode) {
    cf_abort("MemPool bad heap, front overflow.");
  }
}

void *MemPool::alloc(const char *name) {
  Elem *item;
  //CF_ENTRY_FUNC
  cf_assert(this);
  this->allocCount++;
  if (this->list != NULL) {
    item = this->list;
    cf_MemoryPool_doCheck_(item);
    this->list = item->next;
    item->name = name;
    item->next = NULL;
    
    //CF_EXIT_FUNC
    return item + 1;
  } else {
    item = (Elem *)cf_checkedMalloc(this->itemSize);
    item->isBuffered = false;
    item->name = name;
    item->next = NULL;
    item->checkCode = cf_MemoryPool_headCheckCode;
    //CF_EXIT_FUNC
    return item + 1;
  }
}

void MemPool::mfree(void *p) {
  Elem *item;
  //CF_ENTRY_FUNC
  cf_assert(this);
  cf_assert(p);
  
  if (!p) return;
  
  this->allocCount--;
  item = ((Elem *)p) - 1;
  item->next = NULL;
  item->name = NULL;
  cf_MemoryPool_doCheck_(item);
  
  item->next = this->list;
  this->list = item;
  //CF_EXIT_FUNC
}

MemPool::~MemPool() {
  Elem *current;
  Elem *next;
  //CF_ENTRY_FUNC
  cf_assert(this);

  if (this->allocCount != 0) {
    cf_abort("MemPool some memory not release");
    //CF_EXIT_FUNC
    return;
  }

  current = this->list;
  while (current != NULL) {
    //cf_MemoryPool_doCheck_(current);
    next = current->next;
    if (!current->isBuffered) {
      cf_free(current);
    }
    current = next;
  }
  cf_free(this->buffer);
  //CF_EXIT_FUNC
}

CF_END_NAMESPACE
