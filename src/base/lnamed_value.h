#ifndef LOX_NAMED_VALUE_H
#define LOX_NAMED_VALUE_H

#include "base/lvalue.h"
#include "base/lobject.h"

BEGIN_C_DECLS

typedef struct _lnamed_value_t {
  lobject_t object;

  char* name;
  lvalue_t value;
} lnamed_value_t;

lnamed_value_t* lnamed_value_create(const char* name, const lvalue_t* v);

ret_t lnamed_value_set_name(lnamed_value_t* nv, const char* name);
ret_t lnamed_value_set_value(lnamed_value_t* nv, const lvalue_t* v);
int32_t lnamed_value_compare_by_name(lnamed_value_t* nv, const char* name);

#define LNAMED_VALUE(o) (lnamed_value_t*)(o)

END_C_DECLS

#endif /*LOX_NAMED_VALUE_H*/
