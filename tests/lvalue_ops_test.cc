
#include "gtest/gtest.h"
#include "parser/lvalue_ops.h"

TEST(LValueOps, add) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_string(&left, "abc");
  lvalue_set_string(&right, "123");
  ASSERT_EQ(lvalue_add(&left, &right, &result), RET_OK);
  ASSERT_STREQ(lvalue_string(&result), "abc123");
  lvalue_reset(&result);
}

TEST(LValueOps, sub) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);

  lvalue_set_int32(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_sub(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), -1);
}

TEST(LValueOps, and) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int32(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);
}

TEST(LValueOps, or) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);

  lvalue_set_int32(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_bool(&result), TRUE);
}
TEST(LValueOps, bit_or) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);

  lvalue_set_int32(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_bit_or(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 3);
}

TEST(LValueOps, bit_and) {
  lvalue_t left;
  lvalue_t right;
  lvalue_t result;

  lvalue_set_uint8(&left, 1);
  lvalue_set_uint8(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_uint16(&left, 1);
  lvalue_set_uint16(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_uint32(&left, 1);
  lvalue_set_uint32(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_uint64(&left, 1);
  lvalue_set_uint64(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_int8(&left, 1);
  lvalue_set_int8(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_int16(&left, 1);
  lvalue_set_int16(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_int32(&left, 1);
  lvalue_set_int32(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_int64(&left, 1);
  lvalue_set_int64(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_float(&left, 1);
  lvalue_set_float(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_double(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);

  lvalue_set_int32(&left, 1);
  lvalue_set_double(&right, 2);
  ASSERT_EQ(lvalue_bit_and(&left, &right, &result), RET_OK);
  ASSERT_EQ(lvalue_to_int(&result), 0);
}
