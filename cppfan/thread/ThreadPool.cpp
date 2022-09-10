/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cppfan/thread/ThreadPool.h"

CF_USING_NAMESPACE

ThreadPool::ThreadPool(int threadSize) : threadSize(threadSize), onlyRunLatest(false) {
  threadList.reserve(threadSize);
}

ThreadPool::~ThreadPool() {
  threadList.clear();
}

bool ThreadPool::start() {
  for (int i = 0; i < threadSize; ++i)
  {
    threadList.push_back(std::thread(ThreadPool::enterPoint, this));
  }
  return true;
}

void ThreadPool::stop() {
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

void ThreadPool::each() {
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

void ThreadPool::addTask(Task *task) {
  task->_done = false;
  queue.enqueue(task);
}

void ThreadPool::remove(Task *task) {
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

void ThreadPool::run() {
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

int ThreadPool::enterPoint(void *arg) {
  ThreadPool *self = (ThreadPool *)arg;
  self->run();
  //#if JUCE_ANDROID
  //    juce::detachEnv();
  //#endif
  return 0;
}

