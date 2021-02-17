#include "parser/lparams.h"
#include "parser/lstmt.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lobject_lox.h"
#include "interpreter/lcallable_native.h"

static ret_t lobject_callable_native_on_destroy(lobject_t* o) {
  return RET_OK;
}

static lobject_vtable_t s_object_callable_native_vtable = {
    .etype = 0,
    .size = sizeof(lcallable_native_t),
    .type = "object_callable_native",
    .desc = "object_callable_native",
    .on_destroy = lobject_callable_native_on_destroy};

lcallable_t* lcallable_native_create(lcallable_call_t call) {
  lobject_t* o = lobject_create(&s_object_callable_native_vtable);
  lcallable_t* callable = LCALLABLE(o);
  return_value_if_fail(callable != NULL, NULL);

  callable->call = call;

  return callable;
}

lcallable_native_t* lcallable_native_cast(lobject_t* obj) {
  return_value_if_fail(obj != NULL, NULL);
  return_value_if_fail(obj->vt == &s_object_callable_native_vtable, NULL);

  return (lcallable_native_t*)(obj);
}
