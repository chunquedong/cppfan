#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

CF_BEGIN_TEST_GROUP(StringTest)

CF_DEF_TEST(test){
  StrRef s = "hello world";
  StrRef s1 = "hello world";
  StrRef s2 = s;
  cf_verify(s1 == s2);
}

CF_DEF_TEST(test2){
  StrRef s;
  StrRef s1 = "hello world";
  s = s1;
  cf_verify(s1 == s);
}

CF_DEF_TEST(testAdd){
  StrRef s = "hello";
  s += " world";
  StrRef s2 = "hello world";
  cf_verify(s == s2);
}

CF_DEF_TEST(testFormat){
  Str s = Str::format("%s,%d", "hi", 2);
  cf_verify(s == "hi,2");
}

CF_END_TEST_GROUP
