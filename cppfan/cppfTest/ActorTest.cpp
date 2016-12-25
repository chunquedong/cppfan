#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

CF_BEGIN_TEST_GROUP(ActorTest)

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

CF_DEF_TEST(test){
  MyActor actor;
  actor.start(2);
  actor.send_foo((void*)"hi");

  System::sleep(500);
}

CF_END_TEST_GROUP
