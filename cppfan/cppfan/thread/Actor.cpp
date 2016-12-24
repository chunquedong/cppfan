/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cppfan/thread/Actor.h"

CF_USING_NAMESPACE

Actor::Actor()
: Task(false), threadPool(nullptr), maxMsgsBeforeYield(1), timer(nullptr) {
}

void Actor::start(Executor *threadPool, int maxMsgsBeforeYield, Timer *timer) {
  this->threadPool = threadPool;
  this->maxMsgsBeforeYield = maxMsgsBeforeYield;
  this->timer = timer;
}

Actor::~Actor() {
}

void Actor::send(Message &msg) {
  lock_guard guard(mutex);
  
  queue.enqueue(msg);
  
  if (!isRuning && !isCanceled()) {
    threadPool->addTask(this);
    isRuning = true;
  }
}

void Actor::onReceive(Message &msg) {
  
}

void Actor::onCancel(Message &msg) {
  
}

void Actor::run() {
  for (int i=0; i<maxMsgsBeforeYield; ++i) {
    mutex.lock();
    Message msg;
    if (!queue.dequeue(msg)) {
      mutex.unlock();
      break;
    }
    mutex.unlock();
    
    onReceive(msg);
  }
  
  mutex.lock();
  if (queue.size() > 0) {
    threadPool->addTask(this);
  } else {
    isRuning = false;
  }
  mutex.unlock();
}

void Actor::sleep(long millitm) {
  std::chrono::milliseconds dura( millitm );
  std::this_thread::sleep_for( dura );
}

void Actor::sendLater(Message &msg, Time ms) {
  if (timer == NULL) return;
  
  ActorTimeEvent *event = new ActorTimeEvent(this);
  event->delay = ms;
  event->interval = 0;
  event->repeat = 1;
  
  event->msg = msg;
  timer->add(event);
  
  lock_guard guard(mutex);
  schedulerEvent.push_back(event);
}

void Actor::ActorTimeEvent::timeOut() {
  parent->send(msg);
}

void Actor::ActorTimeEvent::done() {
  lock_guard guard(parent->mutex);
  for (auto it = parent->schedulerEvent.begin();
       it != parent->schedulerEvent.end(); ++it) {
    ActorTimeEvent* event = *it;
    if (this == event) {
      parent->schedulerEvent.erase(it);
      break;
    }
  }
  Timer::TimerEvent::done();
}

void Actor::stop() {
  mutex.lock();
  
  this->cancel();
  
  //Message msg;
  std::list<Message> &list = queue.getRawQueue();
  for (std::list<Message>::iterator it = list.begin();
       it != list.end(); ++it) {
    Message &task = *it;
    onCancel(task);
  }
  list.clear();
  
  if (isRuning) {
    threadPool->remove(this);
  }
  
  for (std::vector<ActorTimeEvent*>::iterator it = schedulerEvent.begin();
       it != schedulerEvent.end(); ++it) {
    ActorTimeEvent* event = *it;
    timer->remove(event);
  }
  
  //clear up
  for (std::vector<ActorTimeEvent*>::iterator it = schedulerEvent.begin();
       it != schedulerEvent.end(); ++it) {
    ActorTimeEvent* event = *it;
    event->release();
  }
  schedulerEvent.clear();
  
  mutex.unlock();
  
  if (isRuning) {
    waitDone();
  }
}

void Actor::waitDone() {
  //wait until end
  //if (!autoDelete)
  {
    while (!isDone()) {
      sleep(10);
    }
  }
}

///////////////////////////////////////////////////////////////

void SimpleActor::start(int threadNum) {
  Executor *threadPool = new Executor(threadNum);
  Actor::start(threadPool);
  threadPool->start();
}

void SimpleActor::stop() {
  Actor::stop();
  threadPool->stop();
  delete threadPool;
  threadPool = nullptr;
}

void SimpleActor::onReceive(Message &msg) {
  Func func = slotMap[msg.name];
  if (func) {
    func(this, msg);
  }
}

