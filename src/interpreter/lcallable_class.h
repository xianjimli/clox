#ifndef LOX_CALLABLE_CLASS_H
#define LOX_CALLABLE_CLASS_H

#include "base/lmap.h"
#include "interpreter/lcallable.h"

BEGIN_C_DECLS

typedef struct _lcallable_clazz_t {
  lcallable_t callable;
  lstmt_t* clazz;
} lcallable_clazz_t;

lcallable_t* lcallable_clazz_create(lstmt_t* clazz);

lcallable_clazz_t* lcallable_clazz_cast(lobject_t* obj);

#define LCALLABLE_CLASS(o) lcallable_clazz_cast(LOBJECT(o))

END_C_DECLS

#endif /*LOX_CALLABLE_CLASS_H*/
