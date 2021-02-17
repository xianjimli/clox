#include "gtest/gtest.h"
#include "interpreter/lresolver.h"
#include "parser/lstmt_to_string.h"

static ret_t lresolver_on_result_to_str(void* ctx, lexpr_t* expr, const char* name, int32_t level) {
  char temp[64];
  snprintf(temp, sizeof(temp) - 1, "%s:%d;", name, level);
  lstr_append(LSTR(ctx), temp, -1);

  return RET_OK;
}

TEST(LResolver, basic) {
  lvalue_t v;
  sized_str_t foo = {3, "foo"};
  sized_str_t bar = {3, "bar"};
  lstr_t* log = lstr_create(100);
  lresolver_t* r = lresolver_create(lresolver_on_result_to_str, log);

  ASSERT_EQ(lresolver_begin_scope(r), RET_OK);

  ASSERT_EQ(lresolver_declare(r, &foo), RET_OK);
  ASSERT_EQ(lresolver_get(r, &foo, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), FALSE);

  ASSERT_EQ(lresolver_define(r, &foo), RET_OK);
  ASSERT_EQ(lresolver_get(r, &foo, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), TRUE);

  ASSERT_EQ(lresolver_declare(r, &bar), RET_OK);
  ASSERT_EQ(lresolver_get(r, &bar, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), FALSE);

  ASSERT_EQ(lresolver_define(r, &bar), RET_OK);
  ASSERT_EQ(lresolver_get(r, &bar, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), TRUE);

  ASSERT_EQ(lresolver_end_scope(r), RET_OK);
  ASSERT_EQ(lresolver_destroy(r), RET_OK);
  LOBJECT_UNREF(log);
}

TEST(LResolver, basic2) {
  lvalue_t v;
  sized_str_t foo = {3, "foo"};
  sized_str_t bar = {3, "bar"};
  lstr_t* log = lstr_create(100);
  lresolver_t* r = lresolver_create(lresolver_on_result_to_str, log);

  ASSERT_EQ(lresolver_begin_scope(r), RET_OK);

  ASSERT_EQ(lresolver_declare(r, &foo), RET_OK);
  ASSERT_EQ(lresolver_get(r, &foo, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), FALSE);

  ASSERT_EQ(lresolver_define(r, &foo), RET_OK);
  ASSERT_EQ(lresolver_get(r, &foo, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), TRUE);

  ASSERT_EQ(lresolver_begin_scope(r), RET_OK);

  ASSERT_EQ(lresolver_declare(r, &bar), RET_OK);
  ASSERT_EQ(lresolver_get(r, &bar, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), FALSE);

  ASSERT_EQ(lresolver_define(r, &bar), RET_OK);
  ASSERT_EQ(lresolver_get(r, &bar, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_bool(&v), TRUE);

  ASSERT_EQ(lresolver_end_scope(r), RET_OK);

  ASSERT_EQ(lresolver_end_scope(r), RET_OK);
  ASSERT_EQ(lresolver_destroy(r), RET_OK);
  LOBJECT_UNREF(log);
}

static void test_resolve(const char* input, const char* output, const char* result,
                         uint32_t error) {
  lparser_t p;
  lstmt_t* stmt = NULL;
  lparser_init(&p, input);
  lstr_t* log = lstr_create(100);
  lresolver_t* r = lresolver_create(lresolver_on_result_to_str, log);
  stmt = lparser_parse(&p);
  ASSERT_EQ(lstmt_to_string(stmt, log), RET_OK);
  ASSERT_STREQ(log->str, output);
  lstr_clear(log);
  ASSERT_EQ(lresolver_resolve(r, stmt), RET_OK);
  ASSERT_STREQ(log->str, result);
  ASSERT_EQ(r->error, error);
  lresolver_destroy(r);
  lstmt_destroy(stmt);
  LOBJECT_UNREF(log);
  lparser_deinit(&p);
}

TEST(LResolver, binary) {
  test_resolve("var a=1; a = a + 1", "{\nvar a=1\na=a+1\n}\n", "a:0;a:0;", 0);
  test_resolve("var a = 0; var a=1; a = a + 1", "{\nvar a=0\nvar a=1\na=a+1\n}\n", "a:0;a:0;", 1);
  test_resolve("a=1", "{\na=1\n}\n", "a:-1;", 0);
}

TEST(LResolver, embed) {
  test_resolve("var a=1; {var b = 2; var c = a + b;", "{\nvar a=1\n{\nvar b=2\nvar c=a+b\n}\n\n}\n",
               "a:1;b:0;", 0);
}

TEST(LResolver, func) {
  test_resolve("fun test(a, b) {var c = a + b; c=1}", "{\nfun test(a,b){\nvar c=a+b\nc=1\n}\n\n}\n",
               "a:1;b:1;c:0;", 0);
}

TEST(LResolver, func_name) {
  test_resolve("fun test() {}; test()", "{\nfun test(){\n}\n\ntest()\n}\n", "test:0;", 0);
}

TEST(LResolver, class) {
  test_resolve("class foo {\n test(a, b) {this.value = a+b} } ",
               "{\nclass foo{\ntest(a,b){\nthis.value=a+b\n}\n\n}\n\n}\n", "this:2;a:1;b:1;", 0);
}

TEST(LResolver, super) {
  test_resolve("class base {}\nclass foo < base {\n test(a, b) {super.name=a} } ",
               "{\nclass base{\n}\n\nclass foo<base{\ntest(a,b){\nsuper.name=a\n}\n\n}\n\n}\n",
               "base:0;super:3;a:1;", 0);
}

TEST(LResolver, return ) {
  test_resolve("return 1", "{\nreturn 1\n}\n", "", 1);
}

TEST(LResolver, break) {
  test_resolve("break", "{\nbreak\n}\n", "", 1);
}

TEST(LResolver, continue) {
  test_resolve("continue", "{\ncontinue\n}\n", "", 1);
}

TEST(LResolver, continue_ok) {
  test_resolve("while(1) {continue}", "{\nwhile(1) {\ncontinue\n}\n\n}\n", "", 0);
}

TEST(LResolver, break_ok) {
  test_resolve("while(1) {break}", "{\nwhile(1) {\nbreak\n}\n\n}\n", "", 0);
}
