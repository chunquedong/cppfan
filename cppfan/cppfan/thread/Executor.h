/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef Executor_hpp
#define Executor_hpp

#include "cppfan/base/Object.h"
#include <mutex>
#include <thread>
#include <vector>

#include "cppfan/thread/Queue.h"

CF_BEGIN_NAMESPACE


class Task : public Object {
  friend class Executor;
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
class Executor {
public:
  
  
protected:
  BlockingQueue<Task*> queue;
  bool onlyRunLatest;
  
private:
  std::vector<std::thread> threadList;
  int threadSize;
  
public:
  Executor(int threadSize);
  virtual ~Executor();
  
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

#endif /* Executor_hpp */
