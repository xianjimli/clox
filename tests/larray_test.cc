#include "gtest/gtest.h"
#include "base/larray.h"

TEST(LArray, basic) {
  lvalue_t v;
  larray_t* a = larray_create(0);

  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "a")), RET_OK);
  ASSERT_EQ(a->size, 1u);

  lvalue_reset(&v);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "a");

  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "b")), RET_OK);
  ASSERT_EQ(a->size, 2u);
  lvalue_reset(&v);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "a");
  ASSERT_EQ(larray_get(a, 1, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "b");

  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "c")), RET_OK);
  ASSERT_EQ(a->size, 3u);

  lvalue_reset(&v);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "a");
  ASSERT_EQ(larray_get(a, 1, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "b");

  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "d")), RET_OK);
  ASSERT_EQ(a->size, 4u);

  ASSERT_EQ(larray_insert(a, 0, lvalue_set_string(&v, "3")), RET_OK);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "3");

  ASSERT_EQ(larray_insert(a, 0, lvalue_set_string(&v, "2")), RET_OK);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "2");

  ASSERT_EQ(larray_insert(a, 0, lvalue_set_string(&v, "1")), RET_OK);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "1");
  ASSERT_EQ(a->size, 7u);

  ASSERT_EQ(larray_remove(a, 0), RET_OK);
  ASSERT_EQ(larray_get(a, 0, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "2");

  ASSERT_EQ(larray_remove(a, 1), RET_OK);
  ASSERT_EQ(larray_get(a, 1, &v), RET_OK);
  ASSERT_STREQ(lvalue_string(&v), "a");

  LOBJECT_UNREF(a);
}

TEST(LArray, find) {
  lvalue_t v;
  larray_t* a = larray_create(0);
  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "aaa")), RET_OK);
  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "123")), RET_OK);
  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "fox")), RET_OK);
  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "lox")), RET_OK);
  ASSERT_EQ(larray_push(a, lvalue_set_string(&v, "jim")), RET_OK);
  ASSERT_EQ(larray_find(a, (lcompare_t)lvalue_compare_with_str, "aaa"), 0);
  ASSERT_EQ(larray_find(a, (lcompare_t)lvalue_compare_with_str, "123"), 1);
  ASSERT_EQ(larray_find(a, (lcompare_t)lvalue_compare_with_str, "fox"), 2);
  ASSERT_EQ(larray_find(a, (lcompare_t)lvalue_compare_with_str, "lox"), 3);
  ASSERT_EQ(larray_find(a, (lcompare_t)lvalue_compare_with_str, "loxaaa"), -1);

  LOBJECT_UNREF(a);
}
