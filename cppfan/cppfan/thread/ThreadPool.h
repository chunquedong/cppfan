/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <mutex>
#include <thread>
#include <vector>

#include "cppfan/base/Object.h"
#include "cppfan/thread/Queue.h"

CF_BEGIN_NAMESPACE


class Task : public Object {
  friend class ThreadPool;
protected:
  bool _done;
  bool canceled;
  bool autoDelete;
  
public:
  Task(bool autoDelete = true)
  : _done(false) , canceled(false) {
    this->autoDelete = autoDelete;
  }
  
  virtual ~Task() {}
  
  virtual void run() {}
  
  virtual void cancel() { canceled = true; }
  bool isCanceled() { return canceled; }
  bool isDone() { return _done; }
  
  virtual void done() {
    _done = true;
    if (autoDelete) {
      this->release();
    }
  }
};

/**
 * Thread Pool
 */
class ThreadPool {
public:
  
  
protected:
  BlockingQueue<Task*> queue;
  bool onlyRunLatest;
  
private:
  std::vector<std::thread> threadList;
  int threadSize;
  
public:
  ThreadPool(int threadSize);
  virtual ~ThreadPool();
  
  bool start();
  
  void stop();
  
  void addTask(Task *task);
  
  /**
   * cancel and remove the task
   */
  void remove(Task *task);
  
  /**
   * call visit for each one task
   */
  void each();
  
protected:
  virtual bool visit(Task *t, int index) { return true; }
  
private:
  void run();
  static int enterPoint(void *arg);
};

CF_END_NAMESPACE

#endif /* ThreadPool_hpp */
