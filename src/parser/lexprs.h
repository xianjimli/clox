#ifndef LOX_EXPRS_H
#define LOX_EXPRS_H

#include "base/lmem.h"

BEGIN_C_DECLS

struct _lexprs_t {
  uint16_t size;
  uint16_t capacity;
  lexpr_t** elms;
};

lexprs_t* lexprs_create(uint16_t capacity);
ret_t lexprs_push(lexprs_t* exprs, lexpr_t* expr);
ret_t lexprs_destroy(lexprs_t* exprs);

END_C_DECLS

#endif /*LOX_EXPRS_H*/
