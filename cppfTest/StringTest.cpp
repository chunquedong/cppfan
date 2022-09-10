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
  s += L" world";
  StrRef s2 = "hello world";
  cf_verify(s == s2);
}

CF_DEF_TEST(testFormat){
  Str s = Str::format(L"%ls,%d", L"hi", 2);
  cf_verify(s == "hi,2");
}

CF_DEF_TEST(testTrans){
    Str s = L" aBc ";
    s.trim();
    puts(s.cstr());
    cf_verify(s == L"aBc");
    Str u = s.toUpper();
    cf_verify(s == L"aBc");
    cf_verify(u == L"ABC");
    Str l = s.toLower();
    cf_verify(s == L"aBc");
    cf_verify(l == L"abc");
    
    cf_verify(u.iequals(l));
}

CF_DEF_TEST(testNum){
    Str s64 = Str::fromInt(64);
    Str s32 = Str::fromInt(32);
    
    int val32 = s32.toInt();
    int64_t val64 = s64.toLong();
    cf_verify(val64 == 64);
    cf_verify(val32 == 32);
    
    Str sd = Str::fromFloat(64.0);
    double val = sd.toDouble();
    cf_verify(Math::approx(val, 64));
}

CF_DEF_TEST(testFind){
    Str s = L"abc";
    cf_verify(s.startsWith(L"a"));
    cf_verify(s.endsWith(L"c"));
    cf_verify(s.contains(L"b"));
    
    cf_verify(s.index(L"a") == 0);
    cf_verify(s.index(L"x") == -1);
    cf_verify(s.lastIndex(L"c") == 2);
}

CF_DEF_TEST(testSplit){
    Str s = L",a,b|c,";
    std::vector<Str> list = s.split(",");
    cf_verify(list.at(0) == L"");
    cf_verify(list.at(1) == L"a");
    cf_verify(list.at(2) == L"b|c");
    cf_verify(list.at(3) == L"");
}

CF_DEF_TEST(testReplace){
    Str s = L",a,b|c,";
    s.replace(",", "|");
    cf_verify(s == L"|a|b|c|");
}

CF_END_TEST_GROUP
