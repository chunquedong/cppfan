#include "cppfan/util/System.h"

CF_USING_NAMESPACE

#ifndef CF_WIN

  #include <sys/types.h>
  #include <time.h>
  #include <unistd.h>

  void System::sleep(MillisTime millitm) {
    usleep(millitm * 1000);
  }

  bool System::getSelfPath(char *selfname) {
    const char *sysfile = "/proc/self/exe";
    int  namelen = 256;
    //memset(selfname, 0, 256);
    if ( -1 != readlink( sysfile, selfname, namelen) ) {
      return true;
    }
    return false;
  }

#else
/*========================================================================
 * Windows
 */

#include <Windows.h>

  void System::sleep(Time millitm) {
    Sleep(millitm);
  }

  bool System::getSelfPath(char *selfname) {
    //TCHAR szFileName[MAX_PATH];
    GetModuleFileNameA(NULL,selfname,MAX_PATH);
    return true;
  }

#endif //CF_WIN

#include <thread>

uint64_t System::currentThreadId() {
#ifdef CF_WIN
  //unsigned long GetCurrentThreadId(void);
  return GetCurrentThreadId();
#else
  //unsigned long pthread_self(void);
  return (uint64_t)pthread_self();
#endif
}

