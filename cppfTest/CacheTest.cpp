#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

CF_BEGIN_TEST_GROUP(CacheTest)

class CacheTestObj : public Object {
};

CF_DEF_TEST(test) {
  Cache<StrRef, Ref<CacheTestObj> > cache(2);
  StrRef str = "123";
  StrRef str2 = "124";
  StrRef str3 = "125";
  Ref<CacheTestObj> obj = _new CacheTestObj();
  Ref<CacheTestObj> obj2 = _new CacheTestObj();
  Ref<CacheTestObj> obj3 = _new CacheTestObj();

  cache.set(str, obj);
  cache.set(str2, obj2);
  cache.set(str3, obj3);

  cf_verify(cache.size() == 2);
  cf_verify(cache.contains(str) == false);
  cf_verify(cache.contains(str2));
  cf_verify(cache.contains(str3));
}

CF_DEF_TEST(testRef) {
  Cache<StrRef, CacheTestObj* > cache(2);
  StrRef str = "123";
  StrRef str2 = "124";
  StrRef str3 = "125";
  CacheTestObj *obj = _new CacheTestObj();
  CacheTestObj *obj2 = _new CacheTestObj();
  CacheTestObj *obj3 = _new CacheTestObj();

  cache.set(str, obj);
  cache.set(str2, obj2);
  cache.set(str3, obj3);

  cf_verify(cache.size() == 2);
  cf_verify(cache.contains(str) == false);
  cf_verify(cache.contains(str2));
  cf_verify(cache.contains(str3));
}

CF_DEF_TEST(testKeyRef) {
  Cache<Str, CacheTestObj* > cache(2);
  Str str = "123";
  Str str2 = "124";
  Str str3 = "125";
  CacheTestObj *obj = _new CacheTestObj();
  CacheTestObj *obj2 = _new CacheTestObj();
  CacheTestObj *obj3 = _new CacheTestObj();
  
  cache.set(str, obj);
  cache.set(str2, obj2);
  cache.set(str3, obj3);
  
  cf_verify(cache.size() == 2);
  cf_verify(cache.contains(str) == false);
  cf_verify(cache.contains(str2));
  cf_verify(cache.contains(str3));
}

CF_END_TEST_GROUP
