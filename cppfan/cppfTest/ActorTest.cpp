#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

#if 1
class MyActor : public SimpleActor {
  CF_ACTOR_SLOT(MyActor, foo)

  MyActor() {
    CF_ACTOR_REGISTER(foo);
  }
};

void MyActor::foo(void *arg) {
  printf("%s\n", (char *)arg);
  fflush(stdout);
}

CF_DEF_TEST(ActorTest_test){
  MyActor actor;
  actor.start(2);
  actor.send_foo((void*)"hi");

  System::sleep(500);
}

#endif
