#include "parser/lparams.h"

lparams_t* lparams_create(uint16_t capacity) {
  lparams_t* params = NULL;
  return_value_if_fail(capacity > 0, NULL);
  params = LMEM_NEW(lparams_t);
  return_value_if_fail(params != NULL, NULL);
  params->capacity = capacity;
  params->elms = LMEM_NEW_ARRAY(lparam_t, capacity);
  if (params->elms == NULL) {
    LMEM_FREE(params);
  }

  return params;
}

static ret_t lparams_extend(lparams_t* params) {
  return_value_if_fail(params != NULL, RET_BAD_PARAMS);
  if (params->size < params->capacity) {
    return RET_OK;
  } else {
    uint16_t capacity = params->capacity * 1.5 + 1;
    lparam_t* elms = (lparam_t*)LMEM_REALLOC(params->elms, capacity * sizeof(lparam_t));
    if (elms != NULL) {
      params->elms = elms;
      params->capacity = capacity;
      return RET_OK;
    } else {
      return RET_OOM;
    }
  }
}

ret_t lparams_push(lparams_t* params, const char* str, uint32_t size) {
  return_value_if_fail(params != NULL && str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lparams_extend(params) == RET_OK, RET_OOM);

  params->elms[params->size].str = str;
  params->elms[params->size++].size = size;

  return RET_OK;
}

ret_t lparams_destroy(lparams_t* params) {
  if (params != NULL && params->elms != NULL) {
    LMEM_FREE(params->elms);
    memset(params, 0x00, sizeof(lparams_t));
    LMEM_FREE(params);
  }

  return RET_OK;
}
