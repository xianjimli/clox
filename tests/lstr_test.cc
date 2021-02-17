#include "gtest/gtest.h"
#include "base/lstr.h"

TEST(LStr, set) {
  lstr_t* str = lstr_create(0);
  ASSERT_EQ(str != NULL, true);
  ASSERT_EQ(lstr_set(str, "abc", -1), RET_OK);
  ASSERT_STREQ(str->str, "abc");

  ASSERT_EQ(lstr_set(str, "abc", 1), RET_OK);
  ASSERT_STREQ(str->str, "a");

  ASSERT_EQ(lstr_set(str, "ab", 2), RET_OK);
  ASSERT_STREQ(str->str, "ab");

  ASSERT_EQ(lobject_get_uint32(LOBJECT(str), LOBJECT_PROP_SIZE, 0), 2u);
  ASSERT_EQ(lobject_get_uint32(LOBJECT(str), LOBJECT_PROP_CAPACITY, 0), str->capacity);

  LOBJECT_UNREF(str);
}

TEST(LStr, append) {
  lstr_t* str = lstr_create(0);
  ASSERT_EQ(str != NULL, true);
  ASSERT_EQ(lstr_append(str, "abc", -1), RET_OK);
  ASSERT_STREQ(str->str, "abc");

  ASSERT_EQ(lstr_append(str, "123", 1), RET_OK);
  ASSERT_STREQ(str->str, "abc1");

  ASSERT_EQ(lstr_append(str, "123", 2), RET_OK);
  ASSERT_STREQ(str->str, "abc112");

  ASSERT_EQ(lstr_append(str, "123", 0), RET_OK);
  ASSERT_STREQ(str->str, "abc112");

  LOBJECT_UNREF(str);
}
