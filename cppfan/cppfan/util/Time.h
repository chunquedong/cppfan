/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef TimeUtil_hpp
#define TimeUtil_hpp

#include "cppfan/base/common.h"
#include <string>

CF_BEGIN_NAMESPACE

typedef int64_t Time;//millisecond
typedef int64_t NanosTime;

namespace TimeUtil {
  
  const static NanosTime NanoPerSecond = 1000000000L;
  const static NanosTime NanoPerMillis = 1000000L;
  
  const static Time Seconds = 1000L;
  const static Time Minutes = 60L * Seconds;
  const static Time Hour = 60L * Minutes;
  const static Time Day = Hour * 24L;
  
  /**
   * return the milliseconds from midnight, January 1, 1970 UTC.
   */
  Time currentTimeMillis();
  
  
  std::string formatTimeMillis(Time ms, const char *format=NULL);
  
  /**
   * returns a relative time.
   * Typically it is the number of nanosecond ticks which have elapsed since system startup.
   */
  NanosTime nanoTicks();
  inline Time millisTicks() { return nanoTicks()/NanoPerMillis; }
}

CF_END_NAMESPACE

#endif /* TimeUtil_hpp */
