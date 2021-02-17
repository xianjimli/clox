#include "gtest/gtest.h"
#include "base/lwbuffer.h"
#include "base/lrbuffer.h"

TEST(LWbuffer, basic) {
  int8_t v8 = 0;
  uint8_t vu8 = 0;
  int16_t v16 = 0;
  uint16_t vu16 = 0;
  int32_t v32 = 0;
  uint32_t vu32 = 0;
  int64_t v64 = 0;
  uint64_t vu64 = 0;
  float vf = 0;
  double vd = 0;
  const char* str = NULL;
  uint8_t buff[32];
  uint32_t size = 0;
  lrbuffer_t* rb = NULL;
  lwbuffer_t* wb = lwbuffer_create(100);

  ASSERT_EQ(lwbuffer_write_int8(wb, 1), RET_OK);
  size += sizeof(int8_t);
  ASSERT_EQ(wb->cursor, size);
  ASSERT_EQ(lwbuffer_write_uint8(wb, 2), RET_OK);
  size += sizeof(int8_t);
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_int16(wb, 3), RET_OK);
  size += sizeof(int16_t);
  ASSERT_EQ(wb->cursor, size);
  ASSERT_EQ(lwbuffer_write_uint16(wb, 4), RET_OK);
  size += sizeof(int16_t);
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_int32(wb, 5), RET_OK);
  size += sizeof(int32_t);
  ASSERT_EQ(wb->cursor, size);
  ASSERT_EQ(lwbuffer_write_uint32(wb, 6), RET_OK);
  size += sizeof(int32_t);
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_int64(wb, 7), RET_OK);
  size += sizeof(int64_t);
  ASSERT_EQ(wb->cursor, size);
  ASSERT_EQ(lwbuffer_write_uint64(wb, 8), RET_OK);
  size += sizeof(int64_t);
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_float(wb, 9), RET_OK);
  size += sizeof(float);
  ASSERT_EQ(wb->cursor, size);
  ASSERT_EQ(lwbuffer_write_double(wb, 10), RET_OK);
  size += sizeof(double);
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_string(wb, "abc", -1), RET_OK);
  size += 4;
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_string(wb, "123", 2), RET_OK);
  size += 3;
  ASSERT_EQ(wb->cursor, size);

  ASSERT_EQ(lwbuffer_write_binary(wb, "123", 2), RET_OK);
  size += 2;
  ASSERT_EQ(wb->cursor, size);

  rb = lrbuffer_create(wb->data, wb->cursor);
  ASSERT_EQ(lrbuffer_read_int8(rb, &v8), RET_OK);
  ASSERT_EQ(v8, 1);
  ASSERT_EQ(lrbuffer_read_uint8(rb, &vu8), RET_OK);
  ASSERT_EQ(vu8, 2);

  ASSERT_EQ(lrbuffer_read_int16(rb, &v16), RET_OK);
  ASSERT_EQ(v16, 3);
  ASSERT_EQ(lrbuffer_read_uint16(rb, &vu16), RET_OK);
  ASSERT_EQ(vu16, 4);

  ASSERT_EQ(lrbuffer_read_int32(rb, &v32), RET_OK);
  ASSERT_EQ(v32, 5);
  ASSERT_EQ(lrbuffer_read_uint32(rb, &vu32), RET_OK);
  ASSERT_EQ(vu32, 6u);

  ASSERT_EQ(lrbuffer_read_int64(rb, &v64), RET_OK);
  ASSERT_EQ(v64, 7);
  ASSERT_EQ(lrbuffer_read_uint64(rb, &vu64), RET_OK);
  ASSERT_EQ(vu64, 8u);

  ASSERT_EQ(lrbuffer_read_float(rb, &vf), RET_OK);
  ASSERT_EQ(vf, 9);
  ASSERT_EQ(lrbuffer_read_double(rb, &vd), RET_OK);
  ASSERT_EQ(vd, 10);

  ASSERT_EQ(lrbuffer_read_string(rb, &str), RET_OK);
  ASSERT_STREQ(str, "abc");

  ASSERT_EQ(lrbuffer_read_binary(rb, buff, 2), RET_OK);
  buff[2] = '\0';
  ASSERT_STREQ((char*)buff, "12");

  lwbuffer_destroy(wb);
  lrbuffer_destroy(rb);
}
