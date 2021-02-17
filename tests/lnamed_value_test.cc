
#include "gtest/gtest.h"
#include "base/lnamed_value.h"

TEST(LNamedValue, basic) {
  lvalue_t v;
  lnamed_value_t* nv = lnamed_value_create("age", lvalue_set_uint32(&v, 123));

  ASSERT_STREQ(nv->name, "age");
  ASSERT_EQ(lvalue_uint32(&(nv->value)), 123u);

  ASSERT_EQ(lnamed_value_set_name(nv, "abc"), RET_OK);
  ASSERT_STREQ(nv->name, "abc");

  ASSERT_EQ(lnamed_value_set_value(nv, lvalue_set_uint32(&v, 100)), RET_OK);
  ASSERT_EQ(lvalue_uint32(&(nv->value)), 100u);

  LOBJECT_UNREF(nv);
}

TEST(LNamedValue, basic1) {
  lvalue_t v;
  lnamed_value_t* nv = lnamed_value_create("name", lvalue_set_string(&v, "lox"));
  ASSERT_EQ(nv != NULL, true);

  ASSERT_STREQ(nv->name, "name");
  ASSERT_STREQ(lvalue_string(&(nv->value)), "lox");

  ASSERT_EQ(lnamed_value_set_name(nv, "abc"), RET_OK);
  ASSERT_STREQ(nv->name, "abc");

  ASSERT_EQ(lnamed_value_set_value(nv, lvalue_set_string(&v, "clox")), RET_OK);
  ASSERT_STREQ(lvalue_string(&(nv->value)), "clox");

  LOBJECT_UNREF(nv);
}

TEST(LNamedValue, value) {
  lvalue_t v;
  lnamed_value_t* nv = lnamed_value_create("name", lvalue_set_string(&v, "lox"));
  lvalue_set_object(&v, LOBJECT(nv));
  ASSERT_EQ(lvalue_object(&v) == LOBJECT(nv), true);
  lvalue_reset(&v);
  LOBJECT_UNREF(nv);
}
