#include "base/ltyped_array.h"
#include "gtest/gtest.h"

TEST(TypedArray, basic) {
  lvalue_t v;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);

  ASSERT_EQ(ltyped_array_insert(a, 0, lvalue_set_uint8(&v, 0)), RET_OK);
  ASSERT_EQ(ltyped_array_insert(a, 1, lvalue_set_uint8(&v, 1)), RET_OK);
  ASSERT_EQ(ltyped_array_insert(a, 2, lvalue_set_uint8(&v, 2)), RET_OK);
  ASSERT_EQ(a->size, 3u);

  ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 0);
  ASSERT_EQ(ltyped_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 1);
  ASSERT_EQ(ltyped_array_get(a, 2, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 2);

  ASSERT_EQ(ltyped_array_set(a, 0, lvalue_set_uint8(&v, 100)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 100);

  ASSERT_EQ(ltyped_array_set(a, 1, lvalue_set_uint8(&v, 200)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 200);

  ASSERT_EQ(ltyped_array_set(a, 2, lvalue_set_uint8(&v, 230)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 2, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 230);

  ASSERT_EQ(ltyped_array_remove(a, 1), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 1, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 230);
  ASSERT_EQ(a->size, 2u);

  ASSERT_EQ(ltyped_array_remove(a, 0), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
  ASSERT_EQ(lvalue_uint8(&v), 230);
  ASSERT_EQ(a->size, 1u);

  ASSERT_EQ(ltyped_array_remove(a, 0), RET_OK);
  ASSERT_EQ(a->size, 0u);
  ASSERT_EQ(ltyped_array_remove(a, 0), RET_BAD_PARAMS);
  ASSERT_EQ(a->size, 0u);

  ASSERT_EQ(ltyped_array_tail(a, &v), RET_BAD_PARAMS);

  ltyped_array_destroy(a);
}

TEST(TypedArray, push) {
  lvalue_t v;
  uint8_t i = 0;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_push(a, lvalue_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(ltyped_array_tail(a, &v), RET_OK);
    ASSERT_EQ(lvalue_uint8(&v), i);
  }

  for (; i > 0; i--) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_tail(a, &v), RET_OK);
    ASSERT_EQ(lvalue_uint8(&v), i - 1);
    ASSERT_EQ(ltyped_array_pop(a, &v), RET_OK);
    ASSERT_EQ(lvalue_uint8(&v), i - 1);
  }
  ltyped_array_destroy(a);
}

TEST(TypedArray, insert) {
  lvalue_t v;
  uint8_t i = 0;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_UINT8, 0);
  ASSERT_EQ(a != NULL, true);
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_insert(a, 0, lvalue_set_uint8(&v, i)), RET_OK);
    ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(lvalue_uint8(&v), i);
  }

  ASSERT_EQ(ltyped_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  ltyped_array_destroy(a);
}

TEST(TypedArray, pointer) {
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_POINTER, 0);
  ASSERT_EQ(a == NULL, true);
}

TEST(TypedArray, insert_uint64) {
  lvalue_t v;
  uint8_t i = 0;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_UINT64, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint64_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_insert(a, 0, lvalue_set_uint64(&v, i)), RET_OK);
    ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(lvalue_uint64(&v), i);
  }

  ASSERT_EQ(ltyped_array_set(a, 10, lvalue_set_uint64(&v, 0x1122334455667788)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(lvalue_uint64(&v), 0x1122334455667788UL);
  ASSERT_EQ(ltyped_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  ltyped_array_destroy(a);
}

TEST(TypedArray, insert_double) {
  lvalue_t v;
  uint8_t i = 0;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_DOUBLE, 10);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(double_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_insert(a, 0, lvalue_set_double(&v, i)), RET_OK);
    ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(lvalue_double(&v), i);
  }

  ASSERT_EQ(ltyped_array_set(a, 10, lvalue_set_double(&v, 0x112233)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(lvalue_double(&v), 0x112233);
  ASSERT_EQ(ltyped_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  ltyped_array_destroy(a);
}

TEST(TypedArray, insert_uint32) {
  lvalue_t v;
  uint8_t i = 0;
  ltyped_array_t* a = ltyped_array_create(LVALUE_TYPE_UINT32, 0);
  ASSERT_EQ(a != NULL, true);
  ASSERT_EQ(a->element_size, sizeof(uint32_t));
  for (i = 0; i < 255; i++) {
    ASSERT_EQ(a->size, i);
    ASSERT_EQ(ltyped_array_insert(a, 0, lvalue_set_uint32(&v, i)), RET_OK);
    ASSERT_EQ(ltyped_array_get(a, 0, &v), RET_OK);
    ASSERT_EQ(lvalue_uint32(&v), i);
  }

  ASSERT_EQ(ltyped_array_set(a, 10, lvalue_set_uint32(&v, 0x11223344)), RET_OK);
  ASSERT_EQ(ltyped_array_get(a, 10, &v), RET_OK);
  ASSERT_EQ(lvalue_uint32(&v), 0x11223344u);
  ASSERT_EQ(ltyped_array_clear(a), RET_OK);
  ASSERT_EQ(a->size, 0u);

  ltyped_array_destroy(a);
}
