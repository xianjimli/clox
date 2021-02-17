#include "parser/lexpr.h"
#include "parser/lexprs.h"

lexprs_t* lexprs_create(uint16_t capacity) {
  lexprs_t* exprs = NULL;
  return_value_if_fail(capacity > 0, NULL);
  exprs = LMEM_NEW(lexprs_t);
  return_value_if_fail(exprs != NULL, NULL);
  exprs->capacity = capacity;
  exprs->elms = LMEM_NEW_ARRAY(lexpr_t*, capacity);
  if (exprs->elms == NULL) {
    LMEM_FREE(exprs);
  }

  return exprs;
}

static ret_t lexprs_extend(lexprs_t* exprs) {
  return_value_if_fail(exprs != NULL, RET_BAD_PARAMS);
  if (exprs->size < exprs->capacity) {
    return RET_OK;
  } else {
    uint16_t capacity = exprs->capacity * 1.5 + 1;
    lexpr_t** elms = (lexpr_t**)LMEM_REALLOC(exprs->elms, capacity * sizeof(lexpr_t*));
    if (elms != NULL) {
      exprs->elms = elms;
      exprs->capacity = capacity;
      return RET_OK;
    } else {
      return RET_OOM;
    }
  }
}

ret_t lexprs_push(lexprs_t* exprs, lexpr_t* expr) {
  return_value_if_fail(exprs != NULL && expr != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lexprs_extend(exprs) == RET_OK, RET_OOM);

  exprs->elms[exprs->size++] = expr;

  return RET_OK;
}

ret_t lexprs_destroy(lexprs_t* exprs) {
  if (exprs != NULL && exprs->elms != NULL) {
    uint16_t i = 0;
    for (i = 0; i < exprs->size; i++) {
      lexpr_t* iter = exprs->elms[i];
      if (iter != NULL) {
        lexpr_destroy(iter);
      }
    }
    LMEM_FREE(exprs->elms);
    memset(exprs, 0x00, sizeof(lexprs_t));
    LMEM_FREE(exprs);
  }

  return RET_OK;
}
