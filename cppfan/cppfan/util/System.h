/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_SYSTEM_H_
#define _CF_SYSTEM_H_

#include "cppfan/base/common.h"
#include "cppfan/util/Time.h"

CF_BEGIN_NAMESPACE

namespace System {

/**
 * sleep current thread millitm second
 */
void sleep(MillisTime millitm);

/**
 * get current execable file path
 */
bool getSelfPath(char *selfname);

/**
 * Thread ID
 */
uint64_t currentThreadId();
  
/*========================================================================
 * compare and swap
 */
#if defined(CF_WIN)
  #define cf_compareAndSwap(ptr, old, new) (InterlockedCompareExchange((LPLONG)ptr, new, old) == old)
#elif defined(__IOS__)
#include <libkern/OSAtomic.h>
  #define cf_compareAndSwap(ptr, old, new) OSAtomicCompareAndSwapInt(old, new, ptr)
#else
  #define cf_compareAndSwap(ptr, old, new) __sync_bool_compare_and_swap(ptr, old, new)
#endif

/**
 * fetch old value and increase
 */
inline int increase(volatile int *i) {
  int n, n2;
  do {
    n = *i;
    n2 = n+1;
  } while (!cf_compareAndSwap(i, n, n2));
  return n;
}

/**
 * fetch old value and decrease
 */
inline int decrease(volatile int *i) {
  int n, n2;
  do {
    n = *i;
    n2 = n-1;
  } while (!cf_compareAndSwap(i, n, n2));
  return n;
}

};

CF_END_NAMESPACE

#endif // _CF_SYSTEM_H_
