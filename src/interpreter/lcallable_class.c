#include "parser/lparams.h"
#include "parser/lstmt.h"
#include "interpreter/lcallable.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lobject_lox.h"
#include "interpreter/lcallable_class.h"

static ret_t lobject_callable_clazz_on_destroy(lobject_t* o) {
  return RET_OK;
}

static lobject_vtable_t s_object_callable_clazz_vtable = {
    .etype = 0,
    .size = sizeof(lcallable_clazz_t),
    .type = "object_callable_clazz",
    .desc = "object_callable_clazz",
    .on_destroy = lobject_callable_clazz_on_destroy};

static ret_t lcallable_clazz_call(lcallable_t* callable, linterpreter_t* interpreter,
                                  lvalue_t* argv, uint32_t argc, lvalue_t* result) {
  lobject_t* obj = NULL;
  lcallable_clazz_t* callable_clazz = LCALLABLE_CLASS(callable);
  linterpreter_enter_block(interpreter);
  obj = lobject_lox_create(callable_clazz->clazz, interpreter, argv, argc);
  lvalue_ref_object(result, obj);
  LOBJECT_UNREF(obj);
  linterpreter_leave_block(interpreter);

  return RET_OK;
}

lcallable_t* lcallable_clazz_create(lstmt_t* clazz) {
  lobject_t* o = lobject_create(&s_object_callable_clazz_vtable);
  lcallable_clazz_t* callable_clazz = LCALLABLE_CLASS(o);
  lcallable_t* callable = LCALLABLE(o);
  return_value_if_fail(callable_clazz != NULL, NULL);

  callable_clazz->clazz = clazz;
  callable->call = lcallable_clazz_call;

  return (lcallable_t*)callable_clazz;
}

lcallable_clazz_t* lcallable_clazz_cast(lobject_t* obj) {
  return_value_if_fail(obj != NULL, NULL);
  return_value_if_fail(obj->vt == &s_object_callable_clazz_vtable, NULL);

  return (lcallable_clazz_t*)(obj);
}
