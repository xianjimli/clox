#ifndef LOX_CALLABLE_H
#define LOX_CALLABLE_H

#include "base/larray.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

typedef ret_t (*lcallable_call_t)(lcallable_t* callable, linterpreter_t* interpreter,
                                  lvalue_t* argv, uint32_t argc, lvalue_t* result);

typedef struct _lcallable_t {
  lobject_t object;
  lcallable_call_t call;
} lcallable_t;

ret_t lcallable_call(lcallable_t* callable, linterpreter_t* interpreter, lvalue_t* argv,
                     uint32_t argc, lvalue_t* result);

#define LCALLABLE(o) (lcallable_t*)(o)

END_C_DECLS

#endif /*LOX_CALLABLE_H*/
