#include "parser/lparams.h"
#include "parser/lstmt.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lobject_lox.h"
#include "interpreter/lcallable_func.h"

static ret_t lobject_callable_func_on_destroy(lobject_t* o) {
  return RET_OK;
}

static lobject_vtable_t s_object_callable_func_vtable = {
    .etype = 0,
    .size = sizeof(lcallable_func_t),
    .type = "object_callable_func",
    .desc = "object_callable_func",
    .on_destroy = lobject_callable_func_on_destroy};

static ret_t lcallable_func_call(lcallable_t* callable, linterpreter_t* interpreter, lvalue_t* argv,
                                 uint32_t argc, lvalue_t* result) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  lcallable_func_t* callable_func = LCALLABLE_FUNC(callable);
  lparams_t* params = callable_func->func->data.stmt_func.params;
  lobject_lox_t* obj = LOBJECT_LOX(callable_func->thiz);

  if (obj != NULL) {
    if (obj->super != NULL) {
      linterpreter_enter_block(interpreter);
      linterpreter_define_object(interpreter, "super", obj->super);
    }
    linterpreter_enter_block(interpreter);
    linterpreter_define_object(interpreter, "this", LOBJECT(obj));
  }

  linterpreter_enter_block(interpreter);
  for (i = 0; i < params->size && i < argc; i++) {
    lparam_t* iter = params->elms + i;
    linterpreter_define_var(interpreter, iter, argv + i);
  }
  interpreter->meet_return = FALSE;
  ret = lstmt_eval(callable_func->func->data.stmt_func.stmt_body, interpreter, result);
  interpreter->meet_return = FALSE;
  linterpreter_leave_block(interpreter);

  if (obj != NULL) {
    linterpreter_leave_block(interpreter);
    if (obj->super != NULL) {
      linterpreter_leave_block(interpreter);
    }
  }

  return ret;
}

lcallable_t* lcallable_func_create(lobject_t* thiz, lstmt_t* func) {
  lobject_t* o = lobject_create(&s_object_callable_func_vtable);
  lcallable_func_t* callable_func = LCALLABLE_FUNC(o);
  lcallable_t* callable = LCALLABLE(o);
  return_value_if_fail(callable_func != NULL, NULL);

  callable_func->thiz = thiz;
  callable_func->func = func;
  callable->call = lcallable_func_call;

  return (lcallable_t*)callable_func;
}

lcallable_func_t* lcallable_func_cast(lobject_t* obj) {
  return_value_if_fail(obj != NULL, NULL);
  return_value_if_fail(obj->vt == &s_object_callable_func_vtable, NULL);

  return (lcallable_func_t*)(obj);
}
