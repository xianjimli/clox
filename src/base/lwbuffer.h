#ifndef LOX_WBUFFER_H
#define LOX_WBUFFER_H

#include "base/ltypes_def.h"

BEGIN_C_DECLS

typedef struct _lwbuffer_t {
  uint8_t* data;
  uint32_t cursor;
  uint32_t capacity;
} lwbuffer_t;

lwbuffer_t* lwbuffer_create(uint32_t capacity);
lwbuffer_t* lwbuffer_init(lwbuffer_t* wb, uint32_t capacity);

ret_t lwbuffer_write_int8(lwbuffer_t* wb, int8_t v);
ret_t lwbuffer_write_uint8(lwbuffer_t* wb, uint8_t v);
ret_t lwbuffer_write_int16(lwbuffer_t* wb, int16_t v);
ret_t lwbuffer_write_uint16(lwbuffer_t* wb, uint16_t v);
ret_t lwbuffer_write_int32(lwbuffer_t* wb, int32_t v);
ret_t lwbuffer_write_uint32(lwbuffer_t* wb, uint32_t v);
ret_t lwbuffer_write_int64(lwbuffer_t* wb, int64_t v);
ret_t lwbuffer_write_uint64(lwbuffer_t* wb, uint64_t v);
ret_t lwbuffer_write_float(lwbuffer_t* wb, float v);
ret_t lwbuffer_write_double(lwbuffer_t* wb, double v);
ret_t lwbuffer_write_string(lwbuffer_t* wb, const char* str, int32_t size);
ret_t lwbuffer_write_binary(lwbuffer_t* wb, const void* data, uint32_t size);

ret_t lwbuffer_deinit(lwbuffer_t* wb);
ret_t lwbuffer_destroy(lwbuffer_t* wb);

END_C_DECLS

#endif /*LOX_WBUFFER_H*/
