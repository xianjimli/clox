#include "gtest/gtest.h"
#include "parser/lparser.h"
#include "interpreter/lobject_lox.h"

TEST(LObjectFox, basic) {
  lvalue_t v;
  lvalue_t v1;
  lparser_t p;
  lstmt_t* stmt = NULL;
  lparser_init(&p, "class foo {bar() {return 0}}");
  stmt = lparser_parse_declaration(&p);
  ASSERT_EQ(stmt->type, LTOKEN_CLASS);

  lobject_t* o = lobject_lox_create(stmt, NULL, &v, 1);
  ASSERT_EQ(lobject_set(o, "name", lvalue_set_string(&v, "lox")), RET_OK);
  ASSERT_EQ(lobject_get(o, "name", &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "lox");

  lvalue_set_object(&v, o);
  lvalue_deep_copy(&v1, &v);
  ASSERT_EQ(o->refcount, 2u);
  lvalue_reset(&v1);
  ASSERT_EQ(o->refcount, 1u);
  lstmt_destroy(stmt);
  LOBJECT_UNREF(o);
  lparser_deinit(&p);
}
