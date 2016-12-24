/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "Executor.h"

CF_USING_NAMESPACE

Executor::Executor(int threadSize) : threadSize(threadSize), threadList(threadSize), onlyRunLatest(false) {
}

Executor::~Executor() {
  threadList.clear();
}

bool Executor::start() {
  for (int i = 0; i < threadSize; ++i)
  {
    threadList.push_back(std::thread(Executor::enterPoint, this));
  }
  return true;
}

void Executor::stop() {
  queue.cancel();
  
  for (int i = 0; i < threadSize; ++i)
  {
    threadList[i].join();
  }
  
  //clear queue
  queue.lock();
  std::list<Task*> &list = queue.getRawQueue();
  for (std::list<Task*>::iterator it = list.begin(); it != list.end(); ++it) {
    Task *task = *it;
    task->cancel();
    task->done();
  }
  list.clear();
  queue.unlock();
}

void Executor::each() {
  queue.lock();
  int i = 0;
  std::list<Task*> &list = queue.getRawQueue();
  for (std::list<Task*>::iterator it = list.begin(); it != list.end(); ++it) {
    Task *task = *it;
    if (!visit(task, i)) {
      it = list.erase(it);
      task->cancel();
      task->done();
    }
    ++i;
  }
  queue.unlock();
}

void Executor::addTask(Task *task) {
  task->_done = false;
  queue.enqueue(task);
}

void Executor::remove(Task *task) {
  queue.lock();
  task->cancel();
  std::list<Task*> &list = queue.getRawQueue();
  for (std::list<Task*>::iterator it = list.begin(); it != list.end(); ++it) {
    Task *atask = *it;
    if (atask == task) {
      list.erase(it);
      atask->done();
      break;
    }
  }
  queue.unlock();
}

void Executor::run() {
  Task *task = NULL;
  while (!queue.isCanceled()) {
    task = NULL;
    
    if (onlyRunLatest) {
      while (queue.size() > 1) {
        if (!queue.dequeue(task) || task == NULL) {
          break;
        }
        task->cancel();
        task->done();
        task = NULL;
      }
    }
    
    if (!queue.dequeue(task) || task == NULL) {
      break;
    }
    
    if (!task->isCanceled()) {
      task->run();
    }
    
    task->done();
  }
}

int Executor::enterPoint(void *arg) {
  Executor *self = (Executor *)arg;
  self->run();
  //#if JUCE_ANDROID
  //    juce::detachEnv();
  //#endif
  return 0;
}

