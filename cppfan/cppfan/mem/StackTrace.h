/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_STACKTRACE_H_
#define _CF_STACKTRACE_H_

#include "cppfan/base/Log.h"

CF_BEGIN_NAMESPACE

namespace StackTrace {

  /**
   * print stack trace
   */
  void printTrace();

  /**
   * get stack trace string
   */
  char *getTraceString();

}

CF_END_NAMESPACE

#endif
