#ifndef LOX_PARAMS_H
#define LOX_PARAMS_H

#include "base/lmem.h"

BEGIN_C_DECLS

typedef sized_str_t lparam_t;

typedef struct _lparams_t {
  uint16_t size;
  uint16_t capacity;
  lparam_t* elms;
} lparams_t;

lparams_t* lparams_create(uint16_t capacity);
ret_t lparams_push(lparams_t* params, const char* str, uint32_t size);
ret_t lparams_destroy(lparams_t* params);

END_C_DECLS

#endif /*LOX_PARAMS_H*/
