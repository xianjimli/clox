#include "base/lmem.h"
#include "base/lwbuffer.h"
#include "base/lstr_utils.h"

lwbuffer_t* lwbuffer_create(uint32_t capacity) {
  lwbuffer_t* wb = LMEM_NEW(lwbuffer_t);

  return lwbuffer_init(wb, capacity);
}

static ret_t lwbuffer_extend(lwbuffer_t* wb, int32_t capacity) {
  uint8_t* data = NULL;
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);

  if (wb->capacity <= capacity) {
    capacity = capacity + (capacity >> 1) + 1;
    data = (uint8_t*)LMEM_REALLOC(wb->data, capacity);
    return_value_if_fail(data != NULL, RET_OOM);
    wb->data = data;
    wb->capacity = capacity;
  }

  return RET_OK;
}

lwbuffer_t* lwbuffer_init(lwbuffer_t* wb, uint32_t capacity) {
  return_value_if_fail(wb != NULL, NULL);
  memset(wb, 0x00, sizeof(lwbuffer_t));
  lwbuffer_extend(wb, capacity);

  return wb;
}

ret_t lwbuffer_write_int8(lwbuffer_t* wb, int8_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_uint8(lwbuffer_t* wb, uint8_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_int16(lwbuffer_t* wb, int16_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_uint16(lwbuffer_t* wb, uint16_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_int32(lwbuffer_t* wb, int32_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_uint32(lwbuffer_t* wb, uint32_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_int64(lwbuffer_t* wb, int64_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_uint64(lwbuffer_t* wb, uint64_t v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_float(lwbuffer_t* wb, float v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_double(lwbuffer_t* wb, double v) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + sizeof(v)) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, &v, sizeof(v));
  wb->cursor += sizeof(v);

  return RET_OK;
}

ret_t lwbuffer_write_string(lwbuffer_t* wb, const char* str, int32_t size) {
  size = size < 0 ? lstrlen(str) : size;
  return_value_if_fail(wb != NULL && str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + size + 1) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, str, size);
  wb->cursor += size;
  wb->data[wb->cursor++] = 0;

  return RET_OK;
}

ret_t lwbuffer_write_binary(lwbuffer_t* wb, const void* data, uint32_t size) {
  return_value_if_fail(wb != NULL && data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lwbuffer_extend(wb, wb->cursor + size) == RET_OK, RET_OOM);

  memcpy(wb->data + wb->cursor, data, size);
  wb->cursor += size;

  return RET_OK;
}

ret_t lwbuffer_deinit(lwbuffer_t* wb) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  LMEM_FREE(wb->data);
  memset(wb, 0x00, sizeof(lwbuffer_t));

  return RET_OK;
}

ret_t lwbuffer_destroy(lwbuffer_t* wb) {
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  lwbuffer_deinit(wb);
  LMEM_FREE(wb);

  return RET_OK;
}
