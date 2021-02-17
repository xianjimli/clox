#include "interpreter/lcallable.h"

ret_t lcallable_call(lcallable_t* callable, linterpreter_t* interpreter, lvalue_t* argv,
                     uint32_t argc, lvalue_t* result) {
  return_value_if_fail(callable != NULL && callable->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(interpreter != NULL && argv != NULL, RET_BAD_PARAMS);

  return callable->call(callable, interpreter, argv, argc, result);
}
