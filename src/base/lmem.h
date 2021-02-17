#ifndef LOX_MEM_H
#define LOX_MEM_H

#include "base/ltypes_def.h"

BEGIN_C_DECLS

void* lmem_alloc(uint32_t size);
void* lmem_calloc(uint32_t size);
void* lmem_realloc(void* ptr, uint32_t size);
void lmem_free(void* ptr);

#define LMEM_NEW(type) (type*)lmem_calloc(sizeof(type))
#define LMEM_NEW_ARRAY(type, nr) (type*)lmem_calloc(sizeof(type) * nr)
#define LMEM_FREE(ptr) \
  if (ptr != NULL) {   \
    free(ptr);         \
    ptr = NULL;        \
  }
#define LMEM_ALLOC(size) lmem_alloc(size)
#define LMEM_REALLOC(ptr, size) lmem_realloc(ptr, size)

END_C_DECLS

#endif /*LOX_MEM_H*/
