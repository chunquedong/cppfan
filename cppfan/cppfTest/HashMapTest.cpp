#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class HashMapTestObj : public Object {
public:
  int val;

  HashMapTestObj() {
    val = 10;
    printf("HashMapTestObj ctor\n");
  }

  HashMapTestObj(const HashMapTestObj &other) {
    *this = other;
    printf("copy HashMapTestObj ctor\n\n");
  }

  bool operator== (const HashMapTestObj &other) const {
    return val == other.val;
  }
};

namespace std {
  template <> struct hash<HashMapTestObj> {
    size_t operator()(const HashMapTestObj &key) const {
      return key.hashCode();
    }
  };
}

CF_DEF_TEST(HashMapTest_testObj) {
  HashMap<HashMapTestObj, HashMapTestObj> map(4);
  HashMapTestObj obj;
  obj.val = 10;
  map[obj] = obj;
  HashMapTestObj t = map[obj];

  cf_verify(t.val == 10);
}

CF_DEF_TEST(HashMapTest_testInt) {
  HashMap<int, int> map(4);
  map[1] = 10;
  int t = map[1];

  cf_verify(t == 10);
}

CF_DEF_TEST(HashMapTest_testRemove) {
  HashMap<int, int> map(4);
  map[1] = 10;

  cf_verify(map.contains(1));
  map.remove(1);
  cf_verify(!map.contains(1));
}

CF_DEF_TEST(HashMapTest_testInit) {
  HashMap<int, int> map(4);
  int i = map[1];
  cf_verify(i == 0);
}

CF_DEF_TEST(HashMapTest_testGetOrAdd) {
  HashMap<int, int> map(4);
  int i = map.getOrAdd(3, [](const int &i){ return i*i; });
  cf_verify(i == 9);
}
