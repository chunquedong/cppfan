/*
 * Copyright (c) 2012-2016, chunquedong
 *
 * This file is part of cppfan project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CPPF_TEST_H_
#define _CPPF_TEST_H_

#include "cppfan/base/common.h"
#include <vector>

CF_BEGIN_NAMESPACE

/**
 * Test Case
 * usage:
    CF_DEF_TEST(StringTest_test){
      ...
      cf_verify(s1 == s2);
    }
 */
struct Test {
  void (*func)();
  const char *group;
  const char *name;
  const char *file;
  unsigned int line;
};

/**
 * cf_Test_add
 * cf_Test_run
 */
class TestRunner {
  int errorCount = 0;
  int successCount = 0;
  std::vector<Test> testList;
  static TestRunner *instance;
public:
  
  static TestRunner &cur();
  
  void addResult(bool success) {
    if (success) successCount++;
    else errorCount++;
  }
  
  void add(Test &test) { testList.push_back(test); }
  
  void run(const char *nameFilter);
};

/**
 * add a test case
 *
 */
inline void cf_Test_doAdd(void (*func)(), const char * group, const char *name
                                 , const char *file, const unsigned int line) {
  Test test = { func, group, name, file, line };
  TestRunner::cur().add(test);
}

/**
 * run test case which starts with nameFilter and equals attrFilter
 *
 */
inline void cf_Test_run(const char *nameFilter) {
  TestRunner::cur().run(nameFilter);
}

/**
 * convenience add test case.
 *
 */
#define cf_Test_add(name)\
cf_Test_doAdd(name, "", #name , __FILE__ , __LINE__ )

/**
 * convenience add test case with group.
 *
 */
#define cf_Test_addAttr(name, group)\
cf_Test_doAdd(name, group, #name , __FILE__ , __LINE__ )

/**
 * verify the conditon.
 * It's like assert but special for test.
 *
 */
#define cf_verify(exp) do {\
  TestRunner::cur().addResult(exp);\
  if (!(exp)) {\
    cf_Log_error("test", "test fail: %s", #exp);\
  }\
} while(0);


#define CF_DEF_TEST(name) \
  static void name(void);\
  class __##name##_class {\
  public:\
    __##name##_class() {\
      cf_Test_doAdd(name, test_group_name, #name , __FILE__ , __LINE__ );\
    }\
  };\
  static __##name##_class __##name##_instance;\
  static void name(void)

/**
 * Test Group define a namespace for Test
 */
#define CF_BEGIN_TEST_GROUP(group) \
  namespace group {\
    static const char * test_group_name = #group;

#define CF_END_TEST_GROUP }

CF_END_NAMESPACE
#endif // _CPPF_TEST_H_
