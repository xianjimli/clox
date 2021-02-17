#ifndef LOX_STR_H
#define LOX_STR_H

#include "base/ltypes_def.h"
#include "base/lobject.h"

BEGIN_C_DECLS

typedef struct _lstr_t {
  lobject_t object;
  uint32_t size;
  uint32_t capacity;
  char* str;
} lstr_t;

lstr_t* lstr_create(uint32_t capacity);

ret_t lstr_clear(lstr_t* str);
ret_t lstr_set(lstr_t* str, const char* data, int32_t size);
ret_t lstr_append(lstr_t* str, const char* data, int32_t size);
ret_t lstr_append_value(lstr_t* str, const lvalue_t* value);

ret_t lstr_append_int(lstr_t* str, const char* format, int32_t value);
ret_t lstr_append_float(lstr_t* str, const char* format, double value);
ret_t lstr_append_format_str(lstr_t* str, const char* format, const char* value);

#define LSTR(o) (lstr_t*)(o)

END_C_DECLS

#endif /*LOX_STR_H*/
