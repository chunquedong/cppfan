/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "Timer.h"

#include "cppfan/util/Time.h"

CF_USING_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////
// Timer
//////////////////////////////////////////////////////////////////////////////////

Timer::Timer() : cancel(false) {
}

Timer::~Timer() {
}

void Timer::add(TimerEvent *event) {
  Time now = TimeUtil::millisTicks();
  
  std::lock_guard<std::mutex> guard(mutex);
  event->time_ = now + event->delay;
  list.push_back(event);
  condition.notify_one();
}

void Timer::remove(TimerEvent *event) {
  event->cancel();
  std::lock_guard<std::mutex> guard(mutex);
  
  for (std::vector<TimerEvent*>::iterator it = list.begin();
       it != list.end(); ++it ) {
    TimerEvent* aevent = *it;
    if (aevent == event) {
      aevent->cancel();
      //aevent->done();
      list.erase(it);
      break;
    }
  }
}

void Timer::start() {
  cancel = false;
  thread = std::thread(Timer::enterPoint, this);
}

void Timer::stop() {
  mutex.lock();
  cancel = true;
  condition.notify_all();
  
  for (std::vector<TimerEvent*>::iterator it = list.begin();
       it != list.end(); ++it ) {
    TimerEvent* event = *it;
    event->cancel();
    //event->done();
  }
  list.clear();
  mutex.unlock();
  
  thread.join();
}

Time Timer::doEvent() {
  Time now = TimeUtil::millisTicks();
  TimerEvent *event, *temp;
  Time nextTime, delta;
  bool moved;
  std::vector<TimerEvent*>::iterator it = list.begin();
  nextTime = INT_MAX;
  while (it != list.end()) {
    moved = false;
    event = *it;
    if (event->time_ <= now) {
      event->timeOut();
      event->time_ = now + event->interval;
      
      if (event->repeat > 0) {
        --event->repeat;
      }
      
      if (event->repeat == 0) {
        temp = event;
        it = list.erase(it);
        
        temp->done();
        moved = true;
      }
    }
    
    if (!moved) {
      delta = event->time_ - now;
      if (delta < nextTime) {
        nextTime = delta;
      }
      
      ++it;
    }
  }
  return nextTime;
}

void Timer::run() {
  
  while (!cancel) {
    std::unique_lock<std::mutex> lk(mutex);
    Time waitTime = doEvent();
    Time mill = waitTime;

    condition.wait_for(lk, std::chrono::milliseconds(mill));
  }
  
}

int Timer::enterPoint(void *arg) {
  Timer *self = (Timer *)arg;
  self->run();
  return 0;
}
