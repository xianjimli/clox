#include "base/lmem.h"
#include "base/lrbuffer.h"
#include "base/lstr_utils.h"

lrbuffer_t* lrbuffer_create(const uint8_t* data, uint32_t capacity) {
  lrbuffer_t* rb = NULL;
  return_value_if_fail(data != NULL && capacity > 0, NULL);
  rb = LMEM_NEW(lrbuffer_t);
  return_value_if_fail(rb != NULL, NULL);

  return lrbuffer_init(rb, data, capacity);
}

lrbuffer_t* lrbuffer_init(lrbuffer_t* rb, const uint8_t* data, uint32_t capacity) {
  return_value_if_fail(rb != NULL && data != NULL, NULL);
  memset(rb, 0x00, sizeof(lrbuffer_t));

  rb->data = data;
  rb->capacity = capacity;

  return rb;
}

ret_t lrbuffer_read_int8(lrbuffer_t* rb, int8_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_uint8(lrbuffer_t* rb, uint8_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_int16(lrbuffer_t* rb, int16_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_uint16(lrbuffer_t* rb, uint16_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_int32(lrbuffer_t* rb, int32_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_uint32(lrbuffer_t* rb, uint32_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_int64(lrbuffer_t* rb, int64_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_uint64(lrbuffer_t* rb, uint64_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_float(lrbuffer_t* rb, float_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_double(lrbuffer_t* rb, double_t* v) {
  return_value_if_fail(rb != NULL && v != NULL, RET_BAD_PARAMS);

  memcpy(v, rb->data + rb->cursor, sizeof(*v));
  rb->cursor += sizeof(*v);

  return RET_OK;
}

ret_t lrbuffer_read_string(lrbuffer_t* rb, const char** str) {
  return_value_if_fail(rb != NULL && str != NULL, RET_BAD_PARAMS);

  *str = (const char*)(rb->data) + rb->cursor;
  rb->cursor += strlen(*str) + 1;

  return RET_OK;
}

ret_t lrbuffer_read_binary(lrbuffer_t* rb, void* data, uint32_t size) {
  return_value_if_fail(rb != NULL && data != NULL, RET_BAD_PARAMS);

  memcpy(data, rb->data + rb->cursor, size);
  rb->cursor += size;

  return RET_OK;
}

ret_t lrbuffer_deinit(lrbuffer_t* rb) {
  return_value_if_fail(rb != NULL, RET_BAD_PARAMS);
  memset(rb, 0x00, sizeof(lrbuffer_t));

  return RET_OK;
}

ret_t lrbuffer_destroy(lrbuffer_t* rb) {
  return_value_if_fail(rb != NULL, RET_BAD_PARAMS);
  lrbuffer_deinit(rb);
  LMEM_FREE(rb);

  return RET_OK;
}
