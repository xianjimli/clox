#include "gtest/gtest.h"
#include "interpreter/lnative_ext.h"
#include "interpreter/linterpreter.h"

static void test_interpreter(const char* code, lvalue_t* result) {
  linterpreter_t* ip = NULL;
  lobject_t* global = LOBJECT(lmap_create());
  lnative_ext_register(global);
  ip = linterpreter_create(global, code, strlen(code));
  linterpreter_exec(ip, result);
  linterpreter_destroy(ip);
  LOBJECT_UNREF(global);
}

TEST(LInterpreter, basic) {
  lvalue_t result;

  char buff[256];

  test_interpreter("var a = 123; a += 1", &result);
  ASSERT_EQ(lvalue_to_int(&result), 124);
  lvalue_reset(&result);

  test_interpreter("var a = 123; a *= 2 ", &result);
  ASSERT_EQ(lvalue_to_int(&result), 246);
  lvalue_reset(&result);

  test_interpreter("var a = 100; a /= 2 ", &result);
  ASSERT_EQ(lvalue_to_int(&result), 50);
  lvalue_reset(&result);

  test_interpreter("var a = 100; a -= 2 ", &result);
  ASSERT_EQ(lvalue_to_int(&result), 98);
  lvalue_reset(&result);

#if 1
  test_interpreter("123", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123);
  lvalue_reset(&result);

  test_interpreter("\"abc\"+123", &result);
  ASSERT_STREQ(lvalue_to_string(&result, buff, sizeof(buff)), "abc123");
  lvalue_reset(&result);

  test_interpreter("123+123", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123 + 123);
  lvalue_reset(&result);

  test_interpreter("var a = 123; a", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123);
  lvalue_reset(&result);

  test_interpreter("var a = 123; var b = 456; a = a+b", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123 + 456);
  lvalue_reset(&result);

  test_interpreter("{var a = 123; {var b = 456; a = a+b}}", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123 + 456);
  lvalue_reset(&result);

  test_interpreter("if(1) {100} else {200}", &result);
  ASSERT_EQ(lvalue_to_int(&result), 100);
  lvalue_reset(&result);

  test_interpreter("if(0) {100} else {200}", &result);
  ASSERT_EQ(lvalue_to_int(&result), 200);
  lvalue_reset(&result);

  test_interpreter("var a = 0; while(a < 10) { a = a + 1}", &result);
  ASSERT_EQ(lvalue_to_int(&result), 10);
  lvalue_reset(&result);

  test_interpreter("var a = 0; while(a < 10) { a = a + 1; if(a>5) {break}} a", &result);
  ASSERT_EQ(lvalue_to_int(&result), 6);
  lvalue_reset(&result);

  test_interpreter(
      "var b = 0; var a = 0; while(a < 10) { a = a + 1; if(a>5) {continue}; b = b + 1} b", &result);
  ASSERT_EQ(lvalue_to_int(&result), 5);
  lvalue_reset(&result);

  test_interpreter("var a = 0; var b = 0; for(a = 0; a < 10; a = a + 1) {b = b + 1} b", &result);
  ASSERT_EQ(lvalue_to_int(&result), 10);
  lvalue_reset(&result);
  test_interpreter(
      "var a = 0; var b = 0; for(a = 0; a < 10; a = a + 1) {if(a < 5) {continue}\n b = b + 1} b",
      &result);
  ASSERT_EQ(lvalue_to_int(&result), 5);
  lvalue_reset(&result);

  test_interpreter("fun test(a, b) {return a+b} test(1, 2)", &result);
  ASSERT_EQ(lvalue_to_int(&result), 3);
  lvalue_reset(&result);

  test_interpreter(
      "var g = 0;fun foo(a, b) {return a+b}\n fun bar(a,b,c) {return a+b+c} \ng=foo(1, "
      "2)+bar(3,4,5)",
      &result);
  ASSERT_EQ(lvalue_to_int(&result), 15);
  lvalue_reset(&result);

  test_interpreter("class A {test(a, b) {return a+b}} var a = A(1,2,3); a.age=123;a.age", &result);
  ASSERT_EQ(lvalue_to_int(&result), 123);
  lvalue_reset(&result);

  test_interpreter("class A {test(a, b) {return a+b}} var a = A(1,2,3); a.test(1,2)", &result);
  ASSERT_EQ(lvalue_to_int(&result), 3);
  lvalue_reset(&result);

  test_interpreter(
      "class A {test(a, b) {this.result = 123; return a+b}} var a = A(1,2,3); a.test(1,2); "
      "a.result",
      &result);
  ASSERT_EQ(lvalue_to_int(&result), 123);
  lvalue_reset(&result);

  test_interpreter("class A{init(a,b) {this.value=a+b}}; var a = A(12,34); a.value", &result);
  ASSERT_EQ(lvalue_to_int(&result), 46);
  lvalue_reset(&result);

  test_interpreter(
      "class A{init(a,b) {this.value=a+b}}; class B < A {init(a, b) {super.init(a, b)}}; var b = "
      "B(12,34); b.value",
      &result);
  ASSERT_EQ(lvalue_to_int(&result), 46);
  lvalue_reset(&result);

  test_interpreter("print('hello')", &result);
  ASSERT_EQ(lvalue_to_bool(&result), TRUE);
  lvalue_reset(&result);
#endif
}
