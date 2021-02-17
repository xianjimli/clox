#ifndef LOX_OBJECT_LOX_H
#define LOX_OBJECT_LOX_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lstmt.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

typedef struct _lobject_lox_t {
  lobject_t object;
  lmap_t* props;
  lstmt_t* clazz;
  lobject_t* super;
} lobject_lox_t;

lobject_t* lobject_lox_create(lstmt_t* clazz, linterpreter_t* interpreter, lvalue_t* argv,
                              uint32_t argc);

lobject_lox_t* lobject_lox_cast(lobject_t* obj);

#define LOBJECT_LOX(obj) lobject_lox_cast((lobject_t*)obj)

END_C_DECLS

#endif /*LOX_OBJECT_LOX_H*/
