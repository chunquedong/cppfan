/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */
#include "cppfan/util/Time.h"

CF_USING_NAMESPACE

#ifndef CF_WIN

  #include <sys/types.h>
  #include <time.h>
  #include <unistd.h>

#ifdef __MACH__
  #include <mach/mach.h>
  #include <mach/mach_time.h>
  //#include <sys/_types/_timespec.h>
  #include <mach/mach.h>
  #include <mach/clock.h>
  #include <sys/timeb.h>
  #include <sys/time.h>
  #include <sys/sysctl.h>

  NanosTime TimeUtil::nanoTicks(void) {
    //            clock_serv_t cclock;
    //            mach_timespec_t mts;
    //
    //            host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    //            clock_get_time(cclock, &mts);
    //            mach_port_deallocate(mach_task_self(), cclock);
    //
    //            return (mts.tv_sec * NanoPerSecond) + mts.tv_nsec;
    //            mach_timebase_info_data_t timebase;
    //            mach_timebase_info(&timebase);
    //            int64_t now = mach_absolute_time();
    //            int64_t elapsedNanoSeconds = now * timebase.numer / timebase.denom;
    //            return elapsedNanoSeconds;
    
    struct timeval boottime;
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    size_t size = sizeof(boottime);
    time_t now;
    time_t uptime = -1;
    (void)time(&now);
    if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 && boottime.tv_sec != 0) {
      uptime = now - boottime.tv_sec;
    }
    return uptime * NanoPerSecond;
  }
#else
  #include <sys/timeb.h>

  NanosTime TimeUtil::nanoTicks(void) {
    //  return clock() / (CLOCKS_PER_SECOND * 1000);
    struct timespec ts;
    static time_t startTime;
    int rc = clock_gettime(CLOCK_BOOTTIME, &ts);
    
    //获取CLOCK_BOOTTIME失败的时候使用time函数代替
    if (rc != 0) {
      if (startTime == 0) {
        time(&startTime);
      }
      time_t now;
      time(&now);
      return ((int64_t)(now - startTime))*NanoPerSecond;
    }
    return (ts.tv_sec * NanoPerSecond) + ts.tv_nsec;
  }
#endif

int64_t TimeUtil::currentTimeMillis() {
  //        struct timeb val;
  //        ftime(&val);
  //        return val.time * (int64_t)1000+ val.millitm;
  
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((int64_t) tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

#else
/*========================================================================
 * Windows
 */

#include <Windows.h>

NanosTime TimeUtil::nanoTicks() {
  /*
   LARGE_INTEGER m_nBeginTime;
   LARGE_INTEGER m_nFreq;
   QueryPerformanceFrequency(&m_nFreq);
   QueryPerformanceCounter(&m_nBeginTime);
   return (m_nBeginTime.QuadPart*NanoPerSecond)/m_nFreq.QuadPart;
   */
  int64_t t = (int64_t)timeGetTime();
  return t * 1000000L;
}

int64_t TimeUtil::currentTimeMillis() {
  SYSTEMTIME st;
  FILETIME ft,ft19700101;
  LARGE_INTEGER *pli,*pft,ms_from_19700101;
  
  st.wYear=1970;
  st.wMonth=1;
  st.wDay=1;
  st.wHour=0;
  st.wMinute=0;
  st.wSecond=0;
  st.wMilliseconds=0;
  SystemTimeToFileTime(&st,&ft19700101);
  pft=(LARGE_INTEGER *)&ft19700101;
  
  GetLocalTime(&st);
  SystemTimeToFileTime(&st,&ft);
  pli=(LARGE_INTEGER *)&ft;
  ms_from_19700101.QuadPart=(pli->QuadPart - pft->QuadPart)/10000;
  return ms_from_19700101.QuadPart;
}
#endif


std::string TimeUtil::formatTimeMillis(int64_t ms, const char *format) {
  char buffer[128], timeStr[128];
  if(!format){
    format = "%Y-%m-%d %H:%M:%S";
  }
  
  struct timeval tms = {(long)ms/1000, ((int32_t)ms*1000)};
  strftime(buffer, sizeof(buffer), format, localtime(&tms.tv_sec));
  sprintf(timeStr, "%s.%03lld", buffer, ms%1000);
  return timeStr;
}
