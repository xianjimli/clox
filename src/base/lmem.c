#include "base/lmem.h"
void* lmem_alloc(uint32_t size) {
  return malloc(size);
}

void* lmem_calloc(uint32_t size) {
  return calloc(size, 1);
}

void* lmem_realloc(void* ptr, uint32_t size) {
  return realloc(ptr, size);
}

void lmem_free(void* ptr) {
  free(ptr);
}
