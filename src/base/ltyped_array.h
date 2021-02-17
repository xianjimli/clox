#ifndef TK_LTYPED_ARRAY_H
#define TK_LTYPED_ARRAY_H

#include "base/lvalue.h"

BEGIN_C_DECLS

typedef struct _ltyped_array_t {
  uint32_t size;
  uint32_t capacity;
  uint8_t* data;
  lvalue_type_t type;
  uint32_t element_size;
} ltyped_array_t;

ltyped_array_t* ltyped_array_create(lvalue_type_t type, uint32_t capacity);

ret_t ltyped_array_extend(ltyped_array_t* ltyped_array, uint32_t capacity);

ret_t ltyped_array_get(ltyped_array_t* ltyped_array, uint32_t index, lvalue_t* v);

ret_t ltyped_array_set(ltyped_array_t* ltyped_array, uint32_t index, const lvalue_t* v);

ret_t ltyped_array_insert(ltyped_array_t* ltyped_array, uint32_t index, const lvalue_t* v);

ret_t ltyped_array_remove(ltyped_array_t* ltyped_array, uint32_t index);

ret_t ltyped_array_pop(ltyped_array_t* ltyped_array, lvalue_t* v);

ret_t ltyped_array_tail(ltyped_array_t* ltyped_array, lvalue_t* v);

ret_t ltyped_array_push(ltyped_array_t* ltyped_array, const lvalue_t* v);

ret_t ltyped_array_clear(ltyped_array_t* ltyped_array);

ret_t ltyped_array_destroy(ltyped_array_t* ltyped_array);

END_C_DECLS

#endif /*TK_LTYPED_ARRAY_H*/
