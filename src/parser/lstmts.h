#ifndef LOX_STMTS_H
#define LOX_STMTS_H

#include "base/lmem.h"

BEGIN_C_DECLS

struct _lstmts_t {
  uint16_t size;
  uint16_t capacity;
  lstmt_t** elms;
};

lstmts_t* lstmts_create(uint16_t capacity);
ret_t lstmts_push(lstmts_t* stmts, lstmt_t* stmt);
ret_t lstmts_destroy(lstmts_t* stmts);

END_C_DECLS

#endif /*LOX_STMTS_H*/
