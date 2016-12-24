/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_ACTOR_H
#define _CPPF_ACTOR_H

#include "cppfan/base/Object.h"
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <unordered_map>

#include "cppfan/thread/Timer.h"
#include "cppfan/thread/Executor.h"

CF_BEGIN_NAMESPACE

typedef std::lock_guard<std::recursive_mutex> lock_guard;

/**
 * Current Object
 */
class Actor : public Task {
public:
  struct Message {
    std::string name;
    int id;
    void *param;
  };
private:
  class ActorTimeEvent : public Timer::TimerEvent {
    Actor *parent;
  public:
    Message msg;
    
    ActorTimeEvent(Actor *parent) : Timer::TimerEvent(false), parent(parent) {}
    virtual void timeOut();
    virtual void done();
  };
  
private:
  bool isRuning;
  Queue<Message> queue;
  
  int maxMsgsBeforeYield;
  
  std::recursive_mutex mutex;
  std::vector<ActorTimeEvent*> schedulerEvent;
protected:
  Executor *threadPool;
  Timer *timer;
public:
  Actor();
  virtual void start(Executor *threadPool, int maxMsgsBeforeYield = 2, Timer *timer = NULL);
  
  virtual ~Actor();
  
  virtual void run();
  
  virtual void stop();
  
  void send(Message &msg);
  void sendLater(Message &msg, Time ms);
  virtual void onReceive(Message &msg);
  virtual void onCancel(Message &msg);
  
  static void sleep(long millitm);
  
private:
  void waitDone();
};

class SimpleActor : public Actor {
public:
  typedef void (*Func)(SimpleActor *, Actor::Message &);
  std::unordered_map<std::string, Func> slotMap;
public:
  virtual void start(int threadNum);
  virtual void stop();
  virtual void onReceive(Message &msg);
};


#define CF_ACTOR_SLOT(Type, FuncName)  private: void FuncName(void *arg);\
  private: static void static_##FuncName(Type *self, Actor::Message &msg) {\
    self->FuncName(msg.param);\
  }\
  public: void send_##FuncName(void *arg) {\
      Actor::Message msg;\
      msg.param = arg;\
      msg.id = __LINE__;\
      msg.name = #FuncName;\
      this->send(msg);\
  }

#define CF_ACTOR_REGISTER(FuncName) \
  slotMap[#FuncName] = (SimpleActor::Func)static_##FuncName;


CF_END_NAMESPACE

#endif // ACTOR_H

