/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef Timer_hpp
#define Timer_hpp


#include <mutex>
#include <thread>
#include <vector>

#include "cppfan/base/Object.h"
#include "cppfan/thread/ThreadPool.h"
#include "cppfan/util/Time.h"

CF_BEGIN_NAMESPACE

/**
 * schedule event to execute
 */
class Timer {
public:
  class TimerEvent : public Task {
    friend class Timer;
    
    MillisTime time_;
  public:
    MillisTime delay;
    MillisTime interval;
    int repeat;
    
    TimerEvent(bool autoDelete) : Task(autoDelete) {}
    ~TimerEvent(){}
    
    virtual void timeOut() = 0;
  };
private:
  std::vector<TimerEvent*> list;
  std::mutex mutex;
  std::condition_variable condition;
  std::thread thread;
  bool cancel;
  
public:
  Timer();
  ~Timer();
  
  void start();
  
  void stop();
  
  void remove(TimerEvent *event);
  
  void add(TimerEvent *event);
  
private:
  MillisTime doEvent();
  void run();
  static int enterPoint(void *arg);
  
};

CF_END_NAMESPACE

#endif /* Timer_hpp */
