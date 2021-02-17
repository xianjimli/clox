#ifndef LOX_CALLABLE_FUNC_H
#define LOX_CALLABLE_FUNC_H

#include "base/lmap.h"
#include "interpreter/lcallable.h"

BEGIN_C_DECLS

typedef struct _lcallable_func_t {
  lcallable_t callable;
  lobject_t* thiz;
  lstmt_t* func;
} lcallable_func_t;

lcallable_t* lcallable_func_create(lobject_t* thiz, lstmt_t* func);

lcallable_func_t* lcallable_func_cast(lobject_t* obj);
#define LCALLABLE_FUNC(o) lcallable_func_cast(LOBJECT(o))

END_C_DECLS

#endif /*LOX_CALLABLE_FUNC_H*/
