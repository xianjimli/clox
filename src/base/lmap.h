#ifndef LOX_MAP_H
#define LOX_MAP_H

#include "base/larray.h"
#include "base/lobject.h"

BEGIN_C_DECLS

typedef struct _lmap_t {
  lobject_t object;
  larray_t* array;
} lmap_t;

lmap_t* lmap_create(void);

ret_t lmap_clear(lmap_t* map);
ret_t lmap_set(lmap_t* map, const char* name, const lvalue_t* v);
ret_t lmap_get(lmap_t* map, const char* name, lvalue_t* v);
ret_t lmap_exist(lmap_t* map, const char* name);
ret_t lmap_remove(lmap_t* map, const char* name);

bool_t lmap_get_bool(lmap_t* map, const char* name, bool_t defval);
ret_t lmap_set_bool(lmap_t* map, const char* name, bool_t value);

int8_t lmap_get_int8(lmap_t* map, const char* name, int8_t defval);
ret_t lmap_set_int8(lmap_t* map, const char* name, int8_t value);

uint8_t lmap_get_uint8(lmap_t* map, const char* name, uint8_t defval);
ret_t lmap_set_uint8(lmap_t* map, const char* name, uint8_t value);

int16_t lmap_get_int16(lmap_t* map, const char* name, int16_t defval);
ret_t lmap_set_int16(lmap_t* map, const char* name, int16_t value);

uint16_t lmap_get_uint16(lmap_t* map, const char* name, uint16_t defval);
ret_t lmap_set_uint16(lmap_t* map, const char* name, uint16_t value);

int32_t lmap_get_int32(lmap_t* map, const char* name, int32_t defval);
ret_t lmap_set_int32(lmap_t* map, const char* name, int32_t value);

uint32_t lmap_get_uint32(lmap_t* map, const char* name, uint32_t defval);
ret_t lmap_set_uint32(lmap_t* map, const char* name, uint32_t value);

uint32_t lmap_get_int64(lmap_t* map, const char* name, uint32_t defval);
ret_t lmap_set_int64(lmap_t* map, const char* name, uint32_t value);

uint64_t lmap_get_uint64(lmap_t* map, const char* name, uint64_t defval);
ret_t lmap_set_uint64(lmap_t* map, const char* name, uint64_t value);

float lmap_get_float(lmap_t* map, const char* name, float defval);
ret_t lmap_set_float(lmap_t* map, const char* name, float value);

double lmap_get_double(lmap_t* map, const char* name, double defval);
ret_t lmap_set_double(lmap_t* map, const char* name, double value);

const char* lmap_get_string(lmap_t* map, const char* name, const char* defval);
ret_t lmap_set_string(lmap_t* map, const char* name, const char* value);

lobject_t* lmap_get_object(lmap_t* map, const char* name, lobject_t* defval);
ret_t lmap_set_object(lmap_t* map, const char* name, lobject_t* value);

#define LMAP(o) (lmap_t*)(o)
END_C_DECLS

#endif /*LOX_MAP_H*/
