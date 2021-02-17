#ifndef LOX_VALUE_H
#define LOX_VALUE_H

#include "base/lstr_utils.h"
#include "base/ltypes_def.h"

BEGIN_C_DECLS

typedef enum _lvalue_type_t {
  LVALUE_TYPE_NONE,
  LVALUE_TYPE_NIL,
  LVALUE_TYPE_BOOL,
  LVALUE_TYPE_INT8,
  LVALUE_TYPE_UINT8,
  LVALUE_TYPE_INT16,
  LVALUE_TYPE_UINT16,
  LVALUE_TYPE_INT32,
  LVALUE_TYPE_UINT32,
  LVALUE_TYPE_INT64,
  LVALUE_TYPE_UINT64,
  LVALUE_TYPE_FLOAT,
  LVALUE_TYPE_DOUBLE,
  LVALUE_TYPE_POINTER,
  LVALUE_TYPE_BINARY,
  LVALUE_TYPE_ID,
  LVALUE_TYPE_STRING,
  LVALUE_TYPE_OBJECT,
  LVALUE_TYPE_FUNCTION,
  LVALUE_TYPE_SIZED_STRING,
} lvalue_type_t;

typedef struct _luser_data_t {
  void* data;
  ldestroy_t destroy;
} luser_data_t;

typedef struct _lvalue_t {
#ifdef NDEBUG
  uint8_t type;
#else
  lvalue_type_t type;
#endif /*NDEBUG*/
  bool_t free_handle;
  union {
    bool_t vbool;
    int8_t vint8;
    uint8_t vuint8;
    int16_t vint16;
    uint16_t vuint16;
    int32_t vint32;
    uint32_t vuint32;
    int64_t vint64;
    uint64_t vuint64;
    float vfloat;
    double vdouble;
    char* vstring;
    sized_str_t vid;
    lobject_t* vobject;
    luser_data_t vpointer;
    lfunction_t vfunction;
    sized_str_t vsized_string;
    binary_data_t vbinary_data;
  } v;
} lvalue_t;

lvalue_t* lvalue_set_nil(lvalue_t* v);

double lvalue_double(const lvalue_t* v);
lvalue_t* lvalue_set_double(lvalue_t* v, double vdouble);

float lvalue_float(const lvalue_t* v);
lvalue_t* lvalue_set_float(lvalue_t* v, float vfloat);

void* lvalue_pointer(const lvalue_t* v);
lvalue_t* lvalue_set_pointer(lvalue_t* v, void* vpointer);
lvalue_t* lvalue_set_pointer_ex(lvalue_t* v, void* vpointer, ldestroy_t destroy);

uint64_t lvalue_uint64(const lvalue_t* v);
lvalue_t* lvalue_set_uint64(lvalue_t* v, uint64_t vuint64);

int64_t lvalue_int64(const lvalue_t* v);
lvalue_t* lvalue_set_int64(lvalue_t* v, int64_t vint64);

uint32_t lvalue_uint32(const lvalue_t* v);
lvalue_t* lvalue_set_uint32(lvalue_t* v, uint32_t vuint32);

int32_t lvalue_int32(const lvalue_t* v);
lvalue_t* lvalue_set_int32(lvalue_t* v, int32_t vint32);

uint16_t lvalue_uint16(const lvalue_t* v);
lvalue_t* lvalue_set_uint16(lvalue_t* v, uint16_t vuint16);

int16_t lvalue_int16(const lvalue_t* v);
lvalue_t* lvalue_set_int16(lvalue_t* v, int16_t vint16);

uint8_t lvalue_uint8(const lvalue_t* v);
lvalue_t* lvalue_set_uint8(lvalue_t* v, uint8_t vuint8);

int8_t lvalue_int8(const lvalue_t* v);
lvalue_t* lvalue_set_int8(lvalue_t* v, int8_t vint8);

lobject_t* lvalue_object(const lvalue_t* v);
lvalue_t* lvalue_set_object(lvalue_t* v, lobject_t* vobject);
lvalue_t* lvalue_ref_object(lvalue_t* v, lobject_t* vobject);

bool_t lvalue_bool(const lvalue_t* v);
lvalue_t* lvalue_set_bool(lvalue_t* v, bool_t vbool);

const sized_str_t* lvalue_sized_string(const lvalue_t* v);
lvalue_t* lvalue_set_sized_string(lvalue_t* v, const char* str, uint32_t size);

const sized_str_t* lvalue_id(const lvalue_t* v);
lvalue_t* lvalue_set_id(lvalue_t* v, const char* str, uint32_t size);

const char* lvalue_string(const lvalue_t* v);
lvalue_t* lvalue_set_string(lvalue_t* v, const char* vstring);
lvalue_t* lvalue_dup_string(lvalue_t* v, const char* vstring);
lvalue_t* lvalue_dup_nstring(lvalue_t* v, const char* vstring, uint32_t size);

int32_t lvalue_to_int(const lvalue_t* v);
int32_t lvalue_to_int32(const lvalue_t* v);
int64_t lvalue_to_int64(const lvalue_t* v);
bool_t lvalue_to_bool(const lvalue_t* v);

double lvalue_to_double(const lvalue_t* v);
const char* lvalue_to_string(const lvalue_t* v, char* buff, uint32_t size);

ret_t lvalue_reset(lvalue_t* v);
lvalue_t* lvalue_copy(lvalue_t* d, const lvalue_t* s);
lvalue_t* lvalue_deep_copy(lvalue_t* d, const lvalue_t* s);

int32_t lvalue_compare_with_str(const lvalue_t* v, const char* str);
uint32_t lvalue_type_size(lvalue_type_t type);

END_C_DECLS

#endif /*LOX_VALUE_H*/
