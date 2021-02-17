#include "parser/lparams.h"
#include "parser/lstmt.h"
#include "interpreter/lstmt_eval.h"
#include "compiler/lobject_func.h"
#include "compiler/lcompiler_types.h"

static ret_t lobject_object_func_on_destroy(lobject_t* o) {
  lobject_func_t* object_func = LOBJECT_FUNC(o);

  LMEM_FREE(object_func->name);
  LOBJECT_UNREF(object_func->consts);
  if (object_func->code != NULL) {
    lwbuffer_destroy(object_func->code);
  }

  return RET_OK;
}

static lobject_vtable_t s_object_object_func_vtable = {
    .etype = 0,
    .size = sizeof(lobject_func_t),
    .type = "object_object_func",
    .desc = "object_object_func",
    .on_destroy = lobject_object_func_on_destroy};

lobject_func_t* lobject_func_create(const char* name, uint32_t arity) {
  lobject_t* o = lobject_create(&s_object_object_func_vtable);
  lobject_func_t* object_func = LOBJECT_FUNC(o);
  return_value_if_fail(object_func != NULL, NULL);

  object_func->arity = arity;
  object_func->name = lstrdup(name);
  object_func->consts = larray_create(5);
  goto_error_if_fail(object_func->consts != NULL);
  object_func->code = lwbuffer_create(1024);
  goto_error_if_fail(object_func->code != NULL);

  return object_func;
error:
  LOBJECT_UNREF(o);
  return NULL;
}

lobject_func_t* lobject_func_cast(lobject_t* obj) {
  return_value_if_fail(obj != NULL, NULL);
  return_value_if_fail(obj->vt == &s_object_object_func_vtable, NULL);

  return (lobject_func_t*)(obj);
}

ret_t lobject_func_emit_byte(lobject_func_t* func, uint8_t data) {
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  return lwbuffer_write_uint8(func->code, data);
}

ret_t lobject_func_emit_byte2(lobject_func_t* func, uint8_t data1, uint8_t data2) {
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  lwbuffer_write_uint8(func->code, data1);

  return lwbuffer_write_uint8(func->code, data2);
}

ret_t lobject_func_emit_const(lobject_func_t* func, const lvalue_t* value) {
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);
  return_value_if_fail(larray_push(func->consts, value) == RET_OK, RET_OOM);

  return lobject_func_emit_byte2(func, OP_CONSTANT, func->consts->size - 1);
}
