#ifndef LOX_OBJECT_H
#define LOX_OBJECT_H

#include "base/lvalue.h"

BEGIN_C_DECLS

typedef ret_t (*lobject_unset_t)(lobject_t* o, const char* name);
typedef ret_t (*lobject_get_t)(lobject_t* o, const char* name, lvalue_t* v);
typedef ret_t (*lobject_set_t)(lobject_t* o, const char* name, const lvalue_t* v);
typedef ret_t (*lobject_on_destroy_t)(lobject_t* o);

typedef struct _lobject_vtable_t {
  uint32_t etype;
  uint32_t size;
  const char* type;
  const char* desc;
  lobject_get_t get;
  lobject_set_t set;
  lobject_unset_t unset;
  lobject_on_destroy_t on_destroy;
} lobject_vtable_t;

struct _lobject_t {
  const lobject_vtable_t* vt;
  uint32_t refcount;
};

lobject_t* lobject_ref(lobject_t* o);
ret_t lobject_unref(lobject_t* o);

ret_t lobject_unset(lobject_t* o, const char* name);
ret_t lobject_get(lobject_t* o, const char* name, lvalue_t* v);
ret_t lobject_set(lobject_t* o, const char* name, const lvalue_t* v);

lobject_t* lobject_create(const lobject_vtable_t* vt);
ret_t lobject_deinit(lobject_t* o);

#define LOBJECT(o) (lobject_t*)(o)
#define LOBJECT_REF(o) lobject_ref(LOBJECT(o))
#define LOBJECT_UNREF(o)       \
  if (o != NULL) {             \
    lobject_unref(LOBJECT(o)); \
    o = NULL;                  \
  }

#define LOBJECT_PROP_SIZE "size"
#define LOBJECT_PROP_CAPACITY "capacity"
#define LOBJECT_PROP_NAME "name"
#define LOBJECT_PROP_VALUE "value"

bool_t lobject_get_bool(lobject_t* o, const char* name, bool_t defval);
ret_t lobject_set_bool(lobject_t* o, const char* name, bool_t value);

int8_t lobject_get_int8(lobject_t* o, const char* name, int8_t defval);
ret_t lobject_set_int8(lobject_t* o, const char* name, int8_t value);

uint8_t lobject_get_uint8(lobject_t* o, const char* name, uint8_t defval);
ret_t lobject_set_uint8(lobject_t* o, const char* name, uint8_t value);

int16_t lobject_get_int16(lobject_t* o, const char* name, int16_t defval);
ret_t lobject_set_int16(lobject_t* o, const char* name, int16_t value);

uint16_t lobject_get_uint16(lobject_t* o, const char* name, uint16_t defval);
ret_t lobject_set_uint16(lobject_t* o, const char* name, uint16_t value);

int32_t lobject_get_int32(lobject_t* o, const char* name, int32_t defval);
ret_t lobject_set_int32(lobject_t* o, const char* name, int32_t value);

uint32_t lobject_get_uint32(lobject_t* o, const char* name, uint32_t defval);
ret_t lobject_set_uint32(lobject_t* o, const char* name, uint32_t value);

int64_t lobject_get_int64(lobject_t* o, const char* name, int64_t defval);
ret_t lobject_set_int64(lobject_t* o, const char* name, int64_t value);

uint64_t lobject_get_uint64(lobject_t* o, const char* name, uint64_t defval);
ret_t lobject_set_uint64(lobject_t* o, const char* name, uint64_t value);

float lobject_get_float(lobject_t* o, const char* name, float defval);
ret_t lobject_set_float(lobject_t* o, const char* name, float value);

double lobject_get_double(lobject_t* o, const char* name, double defval);
ret_t lobject_set_double(lobject_t* o, const char* name, double value);

const char* lobject_get_string(lobject_t* o, const char* name, const char* defval);
ret_t lobject_set_string(lobject_t* o, const char* name, const char* value);

lobject_t* lobject_get_object(lobject_t* o, const char* name, lobject_t* defval);
ret_t lobject_set_object(lobject_t* o, const char* name, lobject_t* value);

END_C_DECLS

#endif /*LOX_OBJECT_H*/
