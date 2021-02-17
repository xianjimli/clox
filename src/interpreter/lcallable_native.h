#ifndef LOX_CALLABLE_NATIVE_H
#define LOX_CALLABLE_NATIVE_H

#include "base/lmap.h"
#include "interpreter/lcallable.h"

BEGIN_C_DECLS

typedef struct _lcallable_native_t {
  lcallable_t callable;
} lcallable_native_t;

lcallable_t* lcallable_native_create(lcallable_call_t call);

lcallable_native_t* lcallable_native_cast(lobject_t* obj);
#define LCALLABLE_NATIVE(o) lcallable_native_cast(LOBJECT(o))

END_C_DECLS

#endif /*LOX_CALLABLE_NATIVE_H*/
