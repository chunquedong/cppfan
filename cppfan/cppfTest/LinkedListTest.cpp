#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class LinkedListTestObj : public Object {
public:
  int val;
  LinkedListTestObj *previous;
  LinkedListTestObj *next;
};

CF_DEF_TEST(LinkedListTest_test){
  LinkedList<LinkedListTestObj> list;

  LinkedListTestObj obj;
  list.add(&obj);

  cf_verify(list.size() == 1);
}

CF_DEF_TEST(LinkedListTest_testInsert){
  LinkedList<LinkedListTestObj> list;

  LinkedListTestObj obj;
  LinkedListTestObj obj2;
  LinkedListTestObj obj3;
  list.add(&obj);
  list.insertFirst(&obj2);
  list.insertBefore(&obj3, &obj);

  cf_verify(list.size() == 3);

  cf_verify(list.getAt(0) == &obj2);
  cf_verify(list.getAt(1) == &obj3);
  cf_verify(list.getAt(2) == &obj);
}

CF_DEF_TEST(LinkedListTest_testRemove){
  LinkedList<LinkedListTestObj> list;

  LinkedListTestObj obj;
  LinkedListTestObj obj2;
  LinkedListTestObj obj3;
  list.add(&obj);
  list.add(&obj2);
  list.add(&obj3);

  cf_verify(list.size() == 3);

  list.remove(&obj);
  cf_verify(list.size() == 2);
  list.insertFirst(&obj);

  list.remove(&obj2);
  cf_verify(list.size() == 2);

  list.remove(&obj3);
  cf_verify(list.size() == 1);

  list.remove(&obj);
  cf_verify(list.size() == 0);
}
