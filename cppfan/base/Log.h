/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_LOG_H_
#define _CF_LOG_H_

#include <stdarg.h>
#include <vector>
#include <mutex>

#include "cppfan/base/common.h"

CF_BEGIN_NAMESPACE

class Log {
public:
  enum LogLevel {
    Debug, Info, Warn, Err, Silent
  };
  
  struct LogRec {
    const char *tag;
    const char *file;
    const char *func;
    const unsigned int line;
    const LogLevel level;
    const char *msg;
  };
  
  struct LogListener {
    LogLevel level = LogLevel::Debug;
    virtual void onLog(LogRec &rec);
    virtual void print(LogRec &rec, const char *msg);
    virtual ~LogListener(){}
  };
  
  std::vector<LogListener*> listeners;
  CF_FIELD(LogLevel, level);
private:
  std::mutex lock;
  static Log *instance;
public:
  static Log &cur();

  Log();
  ~Log();
  
  bool isDebug() {
    return isEnableLevel(Debug);
  }
  
  bool isEnableLevel(LogLevel alevel) {
    return alevel >= level();
  }
  
  void doLog(const char *tag, const char *file, const char *func, const unsigned int line
                  , const Log::LogLevel level, const char *msg, ...);
};

class FileLogListener : public Log::LogListener {
  FILE *file;
public:
  FileLogListener(const char *path);
  ~FileLogListener();
  virtual void print(Log::LogRec &rec, const char *msg);
};

CF_END_NAMESPACE

/*========================================================================
 * sys logging macro
 */

#define cf_Log_log(tag, level, msg, ...) do {\
  if (Log::cur().isEnableLevel(level))\
    Log::cur().doLog(tag, __FILE__, __func__, __LINE__,  level, msg, ## __VA_ARGS__);\
} while(false)

/**
 * convenience log macro.
 *
 */
#define cf_Log_error(tag, msg, ...) cf_Log_log(tag, Log::Err, msg, ## __VA_ARGS__)
#define cf_Log_warn(tag, msg, ...)  cf_Log_log(tag, Log::Warn, msg, ## __VA_ARGS__)
#define cf_Log_info(tag, msg, ...)  cf_Log_log(tag, Log::Info, msg, ## __VA_ARGS__)

#ifdef CF_DEBUG
  #define cf_Log_debug(tag, msg, ...) cf_Log_log(tag, Log::Debug, msg, ## __VA_ARGS__)
#else
  #define cf_Log_debug(tag, msg, ...)
#endif

#define cf_Log_tag "cf"


/*========================================================================
 * assert
 */

#include <assert.h>

/**
 * Bomb at condition fail.
 * Only effective at CF_DEBUG macro define.
 */
#ifdef CF_DEBUG
#define cf_assert(exp) {\
  if (!(exp)) {\
    cf_Log_error(cf_Log_tag, "error: %s", #exp);\
    assert(exp);\
  }\
}
#else
  #define cf_assert(exp)
#endif

#define cf_abort(msg) {\
  cf_Log_error(cf_Log_tag, "error: %s", msg);\
  abort();\
}

/**
 * exit function if condition is true.
 */
#define cf_returnErrorIf(exp, val) {\
  if (!(exp)) {\
    cf_Log_error(cf_Log_tag, "error: %s", #exp);\
    return val;\
  }\
}

/**
 * goto error label if condition is true.
 */
#define cf_gotoErrorIf(exp) {\
  if (!(exp)) {\
    cf_Log_error(cf_Log_tag, "error: %s", #exp);\
    goto error;\
  }\
}


#endif

