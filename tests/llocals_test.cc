

#include "gtest/gtest.h"
#include "compiler/llocals.h"

TEST(LLocals, basic) {
  llocals_t* locals = llocals_create(5);

  ASSERT_EQ(llocals_add(locals, "a"), RET_OK);
  ASSERT_EQ(llocals_add(locals, "aa"), RET_OK);
  ASSERT_EQ(llocals_add(locals, "aaa"), RET_OK);

  ASSERT_EQ(llocals_resolve(locals, "a"), 0);
  ASSERT_EQ(llocals_resolve(locals, "aa"), 1);
  ASSERT_EQ(llocals_resolve(locals, "aaa"), 2);

  ASSERT_EQ(llocals_resolve(locals, "b"), -1);

  llocals_destroy(locals);
}

TEST(LLocals, declare) {
  llocals_t* locals = llocals_create(5);

  ASSERT_EQ(llocals_declare(locals, "a", 2), RET_OK);
  ASSERT_EQ(llocals_declare(locals, "a", 2), RET_FAIL);
  ASSERT_EQ(llocals_declare(locals, "a", 2), RET_FAIL);

  ASSERT_EQ(llocals_resolve(locals, "b"), -1);

  llocals_destroy(locals);
}
