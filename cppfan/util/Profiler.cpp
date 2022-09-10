/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */
#include "cppfan/util/Profiler.h"

CF_USING_NAMESPACE

void Counter::add(double val) {
  if (min > val) {
    min = val;
  }
  if (max < val) {
    max = val;
  }
  double sum = average * count + val;
  ++count;
  average = sum / count;
}

void Profiler::start() {
  begin = TimeUtil::nanoTicks();
  tagPoint = begin;
}

/**
 * print and return elapsedTime
 */
double Profiler::tag(const char *name) {
  int64_t now = TimeUtil::nanoTicks();
  double elapsedTime = ((now-tagPoint)/1000000);
  cf_Log_debug("cf.proferProfile", "%s const: %.2fms, from start: %.2fms"
               , name, elapsedTime, ((now-begin)/1000000));
  tagPoint = TimeUtil::nanoTicks();
  return elapsedTime;
}
