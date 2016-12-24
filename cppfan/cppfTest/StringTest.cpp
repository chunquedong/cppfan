#include "cppfan/cppfan.h"

CF_USING_NAMESPACE


CF_DEF_TEST(StringTest_test){
  StrRef s = "hello world";
  StrRef s1 = "hello world";
  StrRef s2 = s;
  cf_verify(s1 == s2);
}

CF_DEF_TEST(StringTest_test2){
  StrRef s;
  StrRef s1 = "hello world";
  s = s1;
  cf_verify(s1 == s);
}

CF_DEF_TEST(StringTest_testAdd){
  StrRef s = "hello";
  s += " world";
  StrRef s2 = "hello world";
  cf_verify(s == s2);
}
