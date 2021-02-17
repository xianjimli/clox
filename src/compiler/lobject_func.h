#ifndef LOX_OBJECT_FUNC_H
#define LOX_OBJECT_FUNC_H

#include "base/larray.h"
#include "base/lwbuffer.h"
#include "base/lobject.h"

BEGIN_C_DECLS

typedef struct _lobject_func_t {
  lobject_t object;

  char* name;
  uint32_t arity;
  uint32_t upvalue_count;
  lwbuffer_t* code;
  larray_t* consts;
} lobject_func_t;

lobject_func_t* lobject_func_create(const char* name, uint32_t arity);

ret_t lobject_func_emit_byte(lobject_func_t* func, uint8_t data);
ret_t lobject_func_emit_byte2(lobject_func_t* func, uint8_t data1, uint8_t data2);
ret_t lobject_func_emit_const(lobject_func_t* func, const lvalue_t* value);

lobject_func_t* lobject_func_cast(lobject_t* obj);
#define LOBJECT_FUNC(o) lobject_func_cast(LOBJECT(o))

END_C_DECLS

#endif /*LOX_OBJECT_FUNC_H*/
