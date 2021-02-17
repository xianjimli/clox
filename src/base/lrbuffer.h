#ifndef LOX_RBUFFER_H
#define LOX_RBUFFER_H

#include "base/ltypes_def.h"

BEGIN_C_DECLS

typedef struct _lrbuffer_t {
  uint32_t cursor;
  uint32_t capacity;
  const uint8_t* data;
} lrbuffer_t;

lrbuffer_t* lrbuffer_create(const uint8_t* data, uint32_t capacity);
lrbuffer_t* lrbuffer_init(lrbuffer_t* rb, const uint8_t* data, uint32_t capacity);

ret_t lrbuffer_read_int8(lrbuffer_t* rb, int8_t* v);
ret_t lrbuffer_read_uint8(lrbuffer_t* rb, uint8_t* v);
ret_t lrbuffer_read_int16(lrbuffer_t* rb, int16_t* v);
ret_t lrbuffer_read_uint16(lrbuffer_t* rb, uint16_t* v);
ret_t lrbuffer_read_int32(lrbuffer_t* rb, int32_t* v);
ret_t lrbuffer_read_uint32(lrbuffer_t* rb, uint32_t* v);
ret_t lrbuffer_read_int64(lrbuffer_t* rb, int64_t* v);
ret_t lrbuffer_read_uint64(lrbuffer_t* rb, uint64_t* v);
ret_t lrbuffer_read_float(lrbuffer_t* rb, float* v);
ret_t lrbuffer_read_double(lrbuffer_t* rb, double* v);
ret_t lrbuffer_read_string(lrbuffer_t* rb, const char** str);
ret_t lrbuffer_read_binary(lrbuffer_t* rb, void* data, uint32_t size);

ret_t lrbuffer_deinit(lrbuffer_t* rb);
ret_t lrbuffer_destroy(lrbuffer_t* rb);

END_C_DECLS

#endif /*LOX_RBUFFER_H*/
