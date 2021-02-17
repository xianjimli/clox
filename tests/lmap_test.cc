#include "gtest/gtest.h"
#include "base/lmap.h"

TEST(LMap, basic) {
  lmap_t* map = lmap_create();
  ASSERT_EQ(lmap_set_string(map, "name", "lox"), RET_OK);
  ASSERT_EQ(lmap_set_string(map, "foo", "bar"), RET_OK);
  ASSERT_STREQ(lmap_get_string(map, "name", NULL), "lox");
  ASSERT_STREQ(lmap_get_string(map, "foo", NULL), "bar");

  LOBJECT_UNREF(map);
}

TEST(LMap, basic1) {
  lmap_t* map = lmap_create();
  ASSERT_EQ(lmap_set_string(map, "name", "lox"), RET_OK);
  ASSERT_EQ(lmap_set_string(map, "foo", "bar"), RET_OK);
  ASSERT_STREQ(lmap_get_string(map, "name", NULL), "lox");
  ASSERT_STREQ(lmap_get_string(map, "foo", NULL), "bar");

  LOBJECT_UNREF(map);
}
