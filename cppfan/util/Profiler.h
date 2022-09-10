/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef PERFORPROFILE_H
#define PERFORPROFILE_H

#include "cppfan/util/Time.h"

CF_BEGIN_NAMESPACE

class Counter {
public:
  double min;
  double max;
  double average;
  unsigned long count;
  
  void add(double val);
};

class Profiler
{
  NanosTime begin;
  NanosTime tagPoint;
public:
  Profiler() {
    start();
  }

  void start();

  /**
   * print and return elapsedTime
   */
  double tag(const char *name);
};

CF_END_NAMESPACE
#endif // PERFORPROFILE_H
