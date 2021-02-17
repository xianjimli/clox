#ifndef LOX_ENV_H
#define LOX_ENV_H

#include "base/lmap.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

struct _lenv_t;
typedef struct _lenv_t lenv_t;

typedef struct _lenv_t {
  lenv_t* enclosing;
  lmap_t* values;
  uint32_t level;
} lenv_t;

lenv_t* lenv_create(lenv_t* enclosing);

ret_t lenv_get(lenv_t* env, const char* name, lvalue_t* v);
ret_t lenv_set(lenv_t* env, const char* name, const lvalue_t* v);
ret_t lenv_get_at(lenv_t* env, uint32_t distance, const char* name, lvalue_t* v);
ret_t lenv_set_at(lenv_t* env, uint32_t distance, const char* name, const lvalue_t* v);
ret_t lenv_define(lenv_t* env, const char* name, const lvalue_t* v);
lenv_t* lenv_get_ancestor(lenv_t* env, uint32_t distance);

ret_t lenv_destroy(lenv_t* env);

END_C_DECLS

#endif /*LOX_ENV_H*/
