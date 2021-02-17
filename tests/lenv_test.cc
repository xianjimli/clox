
#include "gtest/gtest.h"
#include "interpreter/lenv.h"

TEST(LEnv, basic) {
  lvalue_t v;
  lenv_t* env = lenv_create(NULL);
  ASSERT_EQ(lenv_get(env, "foo", &v), RET_NOT_FOUND);
  ASSERT_EQ(lenv_set(env, "foo", &v), RET_NOT_FOUND);

  lvalue_set_int8(&v, 12);
  ASSERT_EQ(lenv_define(env, "foo", &v), RET_OK);
  ASSERT_EQ(lenv_get(env, "foo", &v), RET_OK);
  ASSERT_EQ(lenv_set(env, "foo", &v), RET_OK);

  lenv_destroy(env);
}
