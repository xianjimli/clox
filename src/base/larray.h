#ifndef LOX_ARRAY_H
#define LOX_ARRAY_H

#include "base/lvalue.h"
#include "base/lobject.h"

BEGIN_C_DECLS

typedef struct _larray_t {
  lobject_t object;
  uint32_t size;
  uint32_t capacity;
  lvalue_t* data;
} larray_t;

larray_t* larray_create(uint32_t capacity);

ret_t larray_clear(larray_t* array);

lvalue_t* larray_top(larray_t* array);
ret_t larray_pop(larray_t* array, lvalue_t* v);
ret_t larray_push(larray_t* array, const lvalue_t* v);

ret_t larray_remove(larray_t* array, uint32_t index);
ret_t larray_insert(larray_t* array, uint32_t index, const lvalue_t* v);

ret_t larray_get(larray_t* array, uint32_t index, lvalue_t* v);
ret_t larray_set(larray_t* array, uint32_t index, const lvalue_t* v);
int32_t larray_find(larray_t* array, lcompare_t compare, const void* ctx);

bool_t larray_get_bool(larray_t* array, uint32_t index, bool_t defval);
ret_t larray_set_bool(larray_t* array, uint32_t index, bool_t value);

int8_t larray_get_int8(larray_t* array, uint32_t index, int8_t defval);
ret_t larray_set_int8(larray_t* array, uint32_t index, int8_t value);

uint8_t larray_get_uint8(larray_t* array, uint32_t index, uint8_t defval);
ret_t larray_set_uint8(larray_t* array, uint32_t index, uint8_t value);

int16_t larray_get_int16(larray_t* array, uint32_t index, int16_t defval);
ret_t larray_set_int16(larray_t* array, uint32_t index, int16_t value);

uint16_t larray_get_uint16(larray_t* array, uint32_t index, uint16_t defval);
ret_t larray_set_uint16(larray_t* array, uint32_t index, uint16_t value);

int32_t larray_get_int32(larray_t* array, uint32_t index, int32_t defval);
ret_t larray_set_int32(larray_t* array, uint32_t index, int32_t value);

uint32_t larray_get_uint32(larray_t* array, uint32_t index, uint32_t defval);
ret_t larray_set_uint32(larray_t* array, uint32_t index, uint32_t value);

uint32_t larray_get_int64(larray_t* array, uint32_t index, uint32_t defval);
ret_t larray_set_int64(larray_t* array, uint32_t index, uint32_t value);

uint64_t larray_get_uint64(larray_t* array, uint32_t index, uint64_t defval);
ret_t larray_set_uint64(larray_t* array, uint32_t index, uint64_t value);

float larray_get_float(larray_t* array, uint32_t index, float defval);
ret_t larray_set_float(larray_t* array, uint32_t index, float value);

double larray_get_double(larray_t* array, uint32_t index, double defval);
ret_t larray_set_double(larray_t* array, uint32_t index, double value);

const char* larray_get_string(larray_t* array, uint32_t index, const char* defval);
ret_t larray_set_string(larray_t* array, uint32_t index, const char* value);

lobject_t* larray_get_object(larray_t* array, uint32_t index, lobject_t* defval);
ret_t larray_set_object(larray_t* array, uint32_t index, lobject_t* value);

#define LARRAY(o) (larray_t*)(o)

END_C_DECLS

#endif /*LOX_ARRAY_H*/
