#include "gtest/gtest.h"
#include "base/lvalue.h"

TEST(LValue, to_int) {
  lvalue_t v;
  ASSERT_EQ(lvalue_to_int(lvalue_set_int8(&v, 1)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_uint8(&v, 2)), 2);
  ASSERT_EQ(lvalue_to_int(lvalue_set_int16(&v, 1)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_uint16(&v, 2)), 2);
  ASSERT_EQ(lvalue_to_int(lvalue_set_int32(&v, 1)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_uint32(&v, 2)), 2);
  ASSERT_EQ(lvalue_to_int(lvalue_set_int64(&v, 1)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_uint64(&v, 2)), 2);
  ASSERT_EQ(lvalue_to_int(lvalue_set_float(&v, 1)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_double(&v, 2)), 2);
  ASSERT_EQ(lvalue_to_int(lvalue_set_bool(&v, TRUE)), 1);
  ASSERT_EQ(lvalue_to_int(lvalue_set_bool(&v, FALSE)), 0);
  ASSERT_EQ(lvalue_to_int(lvalue_set_nil(&v)), 0);
}

TEST(LValue, to_str) {
  lvalue_t v;
  char buff[64];
  ASSERT_STREQ(lvalue_to_string(lvalue_set_int8(&v, 1), buff, sizeof(buff)), "1");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_uint8(&v, 2), buff, sizeof(buff)), "2");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_int16(&v, 1), buff, sizeof(buff)), "1");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_uint16(&v, 2), buff, sizeof(buff)), "2");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_int32(&v, 1), buff, sizeof(buff)), "1");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_uint32(&v, 2), buff, sizeof(buff)), "2");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_int64(&v, 1), buff, sizeof(buff)), "1");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_uint64(&v, 2), buff, sizeof(buff)), "2");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_nil(&v), buff, sizeof(buff)), "nil");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_bool(&v, TRUE), buff, sizeof(buff)), "true");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_bool(&v, FALSE), buff, sizeof(buff)), "false");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_float(&v, 1), buff, sizeof(buff)), "1.000000");
  ASSERT_STREQ(lvalue_to_string(lvalue_set_double(&v, 2), buff, sizeof(buff)), "2.000000");
}

TEST(LValue, str) {
  lvalue_t v;
  lvalue_set_string(&v, "abc");
  ASSERT_STREQ(lvalue_string(&v), "abc");

  lvalue_dup_string(&v, "abc");
  ASSERT_STREQ(lvalue_string(&v), "abc");
  lvalue_reset(&v);

  lvalue_dup_nstring(&v, "abc123", 3);
  ASSERT_STREQ(lvalue_string(&v), "abc");
  lvalue_reset(&v);
}

TEST(LValue, basic) {
  lvalue_t v;
  ASSERT_EQ(lvalue_set_int8(&v, 1), &v);
  ASSERT_EQ(lvalue_int8(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_INT8);

  ASSERT_EQ(lvalue_set_uint8(&v, 1), &v);
  ASSERT_EQ(lvalue_uint8(&v), 1u);
  ASSERT_EQ(v.type, LVALUE_TYPE_UINT8);

  ASSERT_EQ(lvalue_set_int16(&v, 1), &v);
  ASSERT_EQ(lvalue_int16(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_INT16);

  ASSERT_EQ(lvalue_set_uint16(&v, 1), &v);
  ASSERT_EQ(lvalue_uint16(&v), 1u);
  ASSERT_EQ(v.type, LVALUE_TYPE_UINT16);

  ASSERT_EQ(lvalue_set_int32(&v, 1), &v);
  ASSERT_EQ(lvalue_int32(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_INT32);

  ASSERT_EQ(lvalue_set_uint32(&v, 1), &v);
  ASSERT_EQ(lvalue_uint32(&v), 1u);
  ASSERT_EQ(v.type, LVALUE_TYPE_UINT32);

  ASSERT_EQ(lvalue_set_int64(&v, 1), &v);
  ASSERT_EQ(lvalue_int64(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_INT64);

  ASSERT_EQ(lvalue_set_uint64(&v, 1), &v);
  ASSERT_EQ(lvalue_uint64(&v), 1u);
  ASSERT_EQ(v.type, LVALUE_TYPE_UINT64);

  ASSERT_EQ(lvalue_set_float(&v, 1), &v);
  ASSERT_EQ(lvalue_float(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_FLOAT);

  ASSERT_EQ(lvalue_set_double(&v, 1), &v);
  ASSERT_EQ(lvalue_double(&v), 1);
  ASSERT_EQ(v.type, LVALUE_TYPE_DOUBLE);
}
