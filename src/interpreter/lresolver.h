#ifndef LOX_RESOLVER_H
#define LOX_RESOLVER_H

#include "base/lmap.h"
#include "base/larray.h"
#include "parser/lexpr.h"
#include "interpreter/linterpreter.h"

BEGIN_C_DECLS

typedef ret_t (*lresolver_on_result_t)(void* ctx, lexpr_t* expr, const char* name, int32_t level);

typedef struct _lresolver_t {
  larray_t* scopes;
  lstmt_t* stmt;

  char tname[64];
  void* on_result_ctx;
  lresolver_on_result_t on_result;

  uint32_t error;
  lstmt_t* current_func;
  lstmt_t* current_class;
  uint32_t in_loop;
} lresolver_t;

lresolver_t* lresolver_create(lresolver_on_result_t on_result, void* on_result_ctx);
lresolver_t* lresolver_init(lresolver_t* resolver);
ret_t lresolver_deinit(lresolver_t* resolver);
ret_t lresolver_resolve(lresolver_t* resolver, lstmt_t* stmt);
ret_t lresolver_destroy(lresolver_t* resolver);

/*public for test*/
ret_t lresolver_begin_scope(lresolver_t* resolver);
ret_t lresolver_end_scope(lresolver_t* resolver);
ret_t lresolver_declare(lresolver_t* resolver, sized_str_t* name);
ret_t lresolver_define(lresolver_t* resolver, sized_str_t* name);
ret_t lresolver_put(lresolver_t* resolver, sized_str_t* name, bool_t value);
ret_t lresolver_get(lresolver_t* resolver, sized_str_t* name, uint16_t scope_index,
                    lvalue_t* value);

END_C_DECLS

#endif /*LOX_RESOLVER_H*/
