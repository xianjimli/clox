#ifndef LOX_LOCALS_H
#define LOX_LOCALS_H

#include "base/lmem.h"

BEGIN_C_DECLS

typedef struct _llocal_t {
  char* name;
  int32_t depth;
  bool_t captured;
} llocal_t;

typedef struct _llocals_t {
  uint16_t size;
  uint16_t capacity;
  llocal_t* elms;
} llocals_t;

llocals_t* llocals_create(uint16_t capacity);

ret_t llocals_add(llocals_t* locals, const char* name);
int32_t llocals_resolve(llocals_t* locals, const char* name);
ret_t llocals_declare(llocals_t* locals, const char* name, int32_t depth);

ret_t llocals_destroy(llocals_t* locals);

END_C_DECLS

#endif /*LOX_LOCALS_H*/
