#include "parser/lstmt.h"
#include "parser/lstmts.h"

lstmts_t* lstmts_create(uint16_t capacity) {
  lstmts_t* stmts = NULL;
  return_value_if_fail(capacity > 0, NULL);
  stmts = LMEM_NEW(lstmts_t);
  return_value_if_fail(stmts != NULL, NULL);
  stmts->capacity = capacity;
  stmts->elms = LMEM_NEW_ARRAY(lstmt_t*, capacity);
  if (stmts->elms == NULL) {
    LMEM_FREE(stmts);
  }

  return stmts;
}

static ret_t lstmts_extend(lstmts_t* stmts) {
  return_value_if_fail(stmts != NULL, RET_BAD_PARAMS);
  if (stmts->size < stmts->capacity) {
    return RET_OK;
  } else {
    uint16_t capacity = stmts->capacity * 1.5 + 1;
    lstmt_t** elms = (lstmt_t**)LMEM_REALLOC(stmts->elms, capacity * sizeof(lstmt_t*));
    if (elms != NULL) {
      stmts->elms = elms;
      stmts->capacity = capacity;
      return RET_OK;
    } else {
      return RET_OOM;
    }
  }
}

ret_t lstmts_push(lstmts_t* stmts, lstmt_t* stmt) {
  return_value_if_fail(stmts != NULL && stmt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lstmts_extend(stmts) == RET_OK, RET_OOM);

  stmts->elms[stmts->size++] = stmt;

  return RET_OK;
}

ret_t lstmts_destroy(lstmts_t* stmts) {
  if (stmts != NULL && stmts->elms != NULL) {
    uint16_t i = 0;
    for (i = 0; i < stmts->size; i++) {
      lstmt_t* iter = stmts->elms[i];
      if (iter != NULL) {
        lstmt_destroy(iter);
      }
    }
    LMEM_FREE(stmts->elms);
    memset(stmts, 0x00, sizeof(lstmts_t));
    LMEM_FREE(stmts);
  }

  return RET_OK;
}
