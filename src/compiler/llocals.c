#include "base/lstr_utils.h"
#include "compiler/llocals.h"

llocals_t* llocals_create(uint16_t capacity) {
  llocals_t* locals = NULL;
  return_value_if_fail(capacity > 0, NULL);
  locals = LMEM_NEW(llocals_t);
  return_value_if_fail(locals != NULL, NULL);
  locals->capacity = capacity;
  locals->elms = LMEM_NEW_ARRAY(llocal_t, capacity);
  if (locals->elms == NULL) {
    LMEM_FREE(locals);
  }

  return locals;
}

static ret_t llocals_extend(llocals_t* locals) {
  return_value_if_fail(locals != NULL, RET_BAD_PARAMS);
  if (locals->size < locals->capacity) {
    return RET_OK;
  } else {
    uint16_t capacity = locals->capacity * 1.5 + 1;
    llocal_t* elms = (llocal_t*)LMEM_REALLOC(locals->elms, capacity * sizeof(llocal_t));
    if (elms != NULL) {
      locals->elms = elms;
      locals->capacity = capacity;
      return RET_OK;
    } else {
      return RET_OOM;
    }
  }
}

ret_t llocals_add(llocals_t* locals, const char* name) {
  llocal_t* iter = NULL;
  return_value_if_fail(locals != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(llocals_extend(locals) == RET_OK, RET_OOM);

  iter = locals->elms + locals->size;
  locals->size++;
  iter->name = lstrdup(name);
  iter->depth = -1;
  iter->captured = FALSE;

  return RET_OK;
}

int32_t llocals_resolve(llocals_t* locals, const char* name) {
  return_value_if_fail(locals != NULL && name != NULL, -1);

  if (locals->size > 0) {
    int32_t i = 0;
    for (i = locals->size - 1; i >= 0; i--) {
      llocal_t* iter = locals->elms + i;
      if (strcmp(iter->name, name) == 0) {
        return i;
      }
    }
  }

  return -1;
}

ret_t llocals_declare(llocals_t* locals, const char* name, int32_t depth) {
  return_value_if_fail(locals != NULL && name != NULL, RET_BAD_PARAMS);

  if (locals->size > 0) {
    int32_t i = 0;
    for (i = locals->size - 1; i >= 0; i--) {
      llocal_t* iter = locals->elms + i;
      if (iter->depth != -1 && iter->depth < depth) {
        break;
      }

      if (strcmp(iter->name, name) == 0) {
        log_debug("duplicate var %s\n", name);
        return RET_FAIL;
      }
    }
  }

  return llocals_add(locals, name);
}

ret_t llocals_destroy(llocals_t* locals) {
  if (locals != NULL && locals->elms != NULL) {
    int32_t i = 0;
    for (i = 0; i < locals->size; i++) {
      llocal_t* iter = locals->elms + i;
      LMEM_FREE(iter->name);
    }
    LMEM_FREE(locals->elms);
    memset(locals, 0x00, sizeof(llocals_t));
    LMEM_FREE(locals);
  }

  return RET_OK;
}
