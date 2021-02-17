#include "base/lmem.h"
#include "base/lvalue.h"
#include "base/lobject.h"

lvalue_t* lvalue_set_nil(lvalue_t* v) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_NIL;

  return v;
}

lvalue_t* lvalue_set_double(lvalue_t* v, double vdouble) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_DOUBLE;
  v->free_handle = FALSE;
  v->v.vdouble = vdouble;

  return v;
}

double lvalue_double(const lvalue_t* v) {
  return_value_if_fail(v != NULL && (v->type == LVALUE_TYPE_DOUBLE || v->type == LVALUE_TYPE_FLOAT),
                       0);
  if (v->type == LVALUE_TYPE_DOUBLE) {
    return v->v.vdouble;
  } else if (v->type == LVALUE_TYPE_FLOAT) {
    return v->v.vfloat;
  } else {
    return 0;
  }
}

lvalue_t* lvalue_set_float(lvalue_t* v, float vfloat) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_FLOAT;
  v->free_handle = FALSE;
  v->v.vfloat = vfloat;

  return v;
}

float lvalue_float(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_FLOAT, 0);
  return v->v.vfloat;
}

lvalue_t* lvalue_set_pointer(lvalue_t* v, void* vpointer) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_POINTER;
  v->free_handle = FALSE;
  v->v.vpointer.data = vpointer;

  return v;
}

lvalue_t* lvalue_set_pointer_ex(lvalue_t* v, void* vpointer, ldestroy_t destroy) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_POINTER;
  v->free_handle = TRUE;
  v->v.vpointer.data = vpointer;
  v->v.vpointer.destroy = destroy;

  return v;
}

void* lvalue_pointer(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_POINTER, 0);
  return v->v.vpointer.data;
}

lvalue_t* lvalue_set_uint64(lvalue_t* v, uint64_t vuint64) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_UINT64;
  v->free_handle = FALSE;
  v->v.vuint64 = vuint64;

  return v;
}

uint64_t lvalue_uint64(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_UINT64, 0);
  return v->v.vuint64;
}

lvalue_t* lvalue_set_int64(lvalue_t* v, int64_t vint64) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_INT64;
  v->free_handle = FALSE;
  v->v.vint64 = vint64;

  return v;
}

int64_t lvalue_int64(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_INT64, 0);
  return v->v.vint64;
}

lvalue_t* lvalue_set_uint32(lvalue_t* v, uint32_t vuint32) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_UINT32;
  v->free_handle = FALSE;
  v->v.vuint32 = vuint32;

  return v;
}

uint32_t lvalue_uint32(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_UINT32, 0);
  return v->v.vuint32;
}

lvalue_t* lvalue_set_int32(lvalue_t* v, int32_t vint32) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_INT32;
  v->free_handle = FALSE;
  v->v.vint32 = vint32;

  return v;
}

int32_t lvalue_int32(const lvalue_t* v) {
  return_value_if_fail(v != NULL, 0);
  if (v->type == LVALUE_TYPE_INT32) {
    return v->v.vint32;
  } else {
    return lvalue_to_int32(v);
  }
}

lvalue_t* lvalue_set_uint16(lvalue_t* v, uint16_t vuint16) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_UINT16;
  v->free_handle = FALSE;
  v->v.vuint16 = vuint16;

  return v;
}

uint16_t lvalue_uint16(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_UINT16, 0);
  return v->v.vuint16;
}

lvalue_t* lvalue_set_int16(lvalue_t* v, int16_t vint16) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_INT16;
  v->free_handle = FALSE;
  v->v.vint16 = vint16;

  return v;
}

int16_t lvalue_int16(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_INT16, 0);
  return v->v.vint16;
}

lvalue_t* lvalue_set_uint8(lvalue_t* v, uint8_t vuint8) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_UINT8;
  v->free_handle = FALSE;
  v->v.vuint8 = vuint8;

  return v;
}

uint8_t lvalue_uint8(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_UINT8, 0);
  return v->v.vuint8;
}

lvalue_t* lvalue_set_int8(lvalue_t* v, int8_t vint8) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_INT8;
  v->free_handle = FALSE;
  v->v.vint8 = vint8;

  return v;
}

int8_t lvalue_int8(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_INT8, 0);
  return v->v.vint8;
}

lvalue_t* lvalue_set_object(lvalue_t* v, lobject_t* vobject) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_OBJECT;
  v->free_handle = FALSE;
  v->v.vobject = vobject;

  return v;
}

lvalue_t* lvalue_ref_object(lvalue_t* v, lobject_t* vobject) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_OBJECT;
  v->free_handle = TRUE;
  v->v.vobject = vobject;
  LOBJECT_REF(vobject);

  return v;
}

lobject_t* lvalue_object(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_OBJECT, 0);
  return v->v.vobject;
}

lvalue_t* lvalue_set_bool(lvalue_t* v, bool_t vbool) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_BOOL;
  v->free_handle = FALSE;
  v->v.vbool = vbool;

  return v;
}

bool_t lvalue_bool(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_BOOL, 0);
  return v->v.vbool;
}

const sized_str_t* lvalue_sized_string(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_SIZED_STRING, NULL);

  return &(v->v.vsized_string);
}

lvalue_t* lvalue_set_sized_string(lvalue_t* v, const char* str, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);
  v->v.vsized_string.str = str;
  v->v.vsized_string.size = size;
  v->type = LVALUE_TYPE_SIZED_STRING;

  return v;
}

const sized_str_t* lvalue_id(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_ID, NULL);

  return &(v->v.vid);
}

lvalue_t* lvalue_set_id(lvalue_t* v, const char* str, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);
  v->v.vid.str = str;
  v->v.vid.size = size;
  v->type = LVALUE_TYPE_ID;

  return v;
}

int32_t lvalue_to_int32(const lvalue_t* v) {
  return lvalue_to_int(v);
}

bool_t lvalue_to_bool(const lvalue_t* v) {
  return lvalue_to_int(v);
}

int32_t lvalue_to_int(const lvalue_t* v) {
  return_value_if_fail(v != NULL, 0);
  switch (v->type) {
    case LVALUE_TYPE_NIL: {
      return 0;
    }
    case LVALUE_TYPE_BOOL: {
      return (int32_t)(v->v.vbool);
    }
    case LVALUE_TYPE_INT8: {
      return (int32_t)(v->v.vint8);
    }
    case LVALUE_TYPE_UINT8: {
      return (int32_t)(v->v.vuint8);
    }
    case LVALUE_TYPE_INT16: {
      return (int32_t)(v->v.vint16);
    }
    case LVALUE_TYPE_UINT16: {
      return (int32_t)(v->v.vuint16);
    }
    case LVALUE_TYPE_INT32: {
      return (int32_t)(v->v.vint32);
    }
    case LVALUE_TYPE_UINT32: {
      return (int32_t)(v->v.vuint32);
    }
    case LVALUE_TYPE_INT64: {
      return (int32_t)(v->v.vint64);
    }
    case LVALUE_TYPE_UINT64: {
      return (int32_t)(v->v.vuint64);
    }
    case LVALUE_TYPE_FLOAT: {
      return (int32_t)(v->v.vfloat);
    }
    case LVALUE_TYPE_DOUBLE: {
      return (int32_t)(v->v.vdouble);
    }
    case LVALUE_TYPE_STRING: {
      return atoi(v->v.vstring);
    }
    case LVALUE_TYPE_SIZED_STRING: {
      return atoi(v->v.vsized_string.str);
    }
    default: {
      assert(!"not support type to int");
      return 0;
    }
  }
}

int64_t lvalue_to_int64(const lvalue_t* v) {
  return_value_if_fail(v != NULL, 0);
  switch (v->type) {
    case LVALUE_TYPE_NIL: {
      return 0;
    }
    case LVALUE_TYPE_BOOL: {
      return (int64_t)(v->v.vbool);
    }
    case LVALUE_TYPE_INT8: {
      return (int64_t)(v->v.vint8);
    }
    case LVALUE_TYPE_UINT8: {
      return (int64_t)(v->v.vuint8);
    }
    case LVALUE_TYPE_INT16: {
      return (int64_t)(v->v.vint16);
    }
    case LVALUE_TYPE_UINT16: {
      return (int64_t)(v->v.vuint16);
    }
    case LVALUE_TYPE_INT32: {
      return (int64_t)(v->v.vint32);
    }
    case LVALUE_TYPE_UINT32: {
      return (int64_t)(v->v.vuint32);
    }
    case LVALUE_TYPE_INT64: {
      return (int64_t)(v->v.vint64);
    }
    case LVALUE_TYPE_UINT64: {
      return (int64_t)(v->v.vuint64);
    }
    case LVALUE_TYPE_FLOAT: {
      return (int64_t)(v->v.vfloat);
    }
    case LVALUE_TYPE_DOUBLE: {
      return (int64_t)(v->v.vdouble);
    }
    case LVALUE_TYPE_STRING: {
      return atoi(v->v.vstring);
    }
    case LVALUE_TYPE_SIZED_STRING: {
      return atoi(v->v.vsized_string.str);
    }
    default: {
      assert(!"not support type to int");
      return 0;
    }
  }
}

double lvalue_to_double(const lvalue_t* v) {
  return_value_if_fail(v != NULL, 0);
  switch (v->type) {
    case LVALUE_TYPE_NIL: {
      return 0;
    }
    case LVALUE_TYPE_BOOL: {
      return (double)(v->v.vbool);
    }
    case LVALUE_TYPE_INT8: {
      return (double)(v->v.vint8);
    }
    case LVALUE_TYPE_UINT8: {
      return (double)(v->v.vuint8);
    }
    case LVALUE_TYPE_INT16: {
      return (double)(v->v.vint16);
    }
    case LVALUE_TYPE_UINT16: {
      return (double)(v->v.vuint16);
    }
    case LVALUE_TYPE_INT32: {
      return (double)(v->v.vint32);
    }
    case LVALUE_TYPE_UINT32: {
      return (double)(v->v.vuint32);
    }
    case LVALUE_TYPE_INT64: {
      return (double)(v->v.vint64);
    }
    case LVALUE_TYPE_UINT64: {
      return (double)(v->v.vuint64);
    }
    case LVALUE_TYPE_FLOAT: {
      return (double)(v->v.vfloat);
    }
    case LVALUE_TYPE_DOUBLE: {
      return (double)(v->v.vdouble);
    }
    case LVALUE_TYPE_STRING: {
      return atof(v->v.vstring);
    }
    case LVALUE_TYPE_SIZED_STRING: {
      return atof(v->v.vsized_string.str);
    }
    default: {
      assert(!"not support type to double");
      return 0;
    }
  }
}

static const char* sized_str_to_str(const sized_str_t* str, char* buff, uint32_t size) {
  memset(buff, 0x00, size);
  size--;
  size = size < str->size ? size : str->size;
  memcpy(buff, str->str, size);

  return buff;
}

const char* lvalue_to_string(const lvalue_t* v, char* buff, uint32_t size) {
  return_value_if_fail(v != NULL && buff != NULL && size > 1, 0);
  switch (v->type) {
    case LVALUE_TYPE_NIL: {
      return "nil";
    }
    case LVALUE_TYPE_BOOL: {
      return (v->v.vbool) ? "true" : "false";
    }
    case LVALUE_TYPE_INT8: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint8));
      return buff;
    }
    case LVALUE_TYPE_UINT8: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint8));
      return buff;
    }
    case LVALUE_TYPE_INT16: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint16));
      return buff;
    }
    case LVALUE_TYPE_UINT16: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint16));
      return buff;
    }
    case LVALUE_TYPE_INT32: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint32));
      return buff;
    }
    case LVALUE_TYPE_UINT32: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint32));
      return buff;
    }
    case LVALUE_TYPE_INT64: {
      snprintf(buff, size, "%" PRId64, (int64_t)(v->v.vint64));
      return buff;
    }
    case LVALUE_TYPE_UINT64: {
      snprintf(buff, size, "%" PRIu64, (uint64_t)(v->v.vuint64));
      return buff;
    }
    case LVALUE_TYPE_FLOAT: {
      snprintf(buff, size, "%f", (v->v.vfloat));
      return buff;
    }
    case LVALUE_TYPE_DOUBLE: {
      snprintf(buff, size, "%lf", (v->v.vdouble));
      return buff;
    }
    case LVALUE_TYPE_STRING: {
      return v->v.vstring;
    }
    case LVALUE_TYPE_SIZED_STRING: {
      const sized_str_t* str = &(v->v.vsized_string);
      return sized_str_to_str(str, buff, size);
    }
    case LVALUE_TYPE_ID: {
      const sized_str_t* str = &(v->v.vid);
      return sized_str_to_str(str, buff, size);
    }
    default: {
      assert(!"not support type to double");
      return NULL;
    }
  }
}

ret_t lvalue_reset(lvalue_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);
  if (v->free_handle) {
    switch (v->type) {
      case LVALUE_TYPE_OBJECT: {
        LOBJECT_UNREF(v->v.vobject);
        break;
      }
      case LVALUE_TYPE_STRING: {
        LMEM_FREE(v->v.vstring);
        break;
      }
      case LVALUE_TYPE_POINTER: {
        if (v->v.vpointer.destroy != NULL) {
          v->v.vpointer.destroy(v->v.vpointer.data);
        }
        break;
      }
      default:
        break;
    }
  }
  memset(v, 0x00, sizeof(lvalue_t));

  return RET_OK;
}

lvalue_t* lvalue_copy(lvalue_t* v, const lvalue_t* s) {
  return_value_if_fail(v != NULL && s != NULL, NULL);
  memcpy(v, s, sizeof(lvalue_t));
  v->free_handle = FALSE;

  return v;
}

lvalue_t* lvalue_deep_copy(lvalue_t* v, const lvalue_t* s) {
  return_value_if_fail(lvalue_copy(v, s) != NULL, NULL);
  switch (v->type) {
    case LVALUE_TYPE_STRING: {
      v->v.vstring = lstrdup(v->v.vstring);
      v->free_handle = TRUE;
      break;
    }
    case LVALUE_TYPE_OBJECT: {
      LOBJECT_REF(v->v.vobject);
      v->free_handle = TRUE;
      break;
    }
    case LVALUE_TYPE_POINTER: {
      if (v->v.vpointer.destroy != NULL) {
        v->free_handle = TRUE;
        ((lvalue_t*)s)->free_handle = FALSE;
      }
      break;
    }
    default:
      break;
  }

  return v;
}

lvalue_t* lvalue_set_string(lvalue_t* v, const char* vstring) {
  return_value_if_fail(v != NULL, NULL);
  v->type = LVALUE_TYPE_STRING;
  v->free_handle = FALSE;
  v->v.vstring = (char*)vstring;

  return v;
}

const char* lvalue_string(const lvalue_t* v) {
  return_value_if_fail(v != NULL && v->type == LVALUE_TYPE_STRING, 0);
  return v->v.vstring;
}

lvalue_t* lvalue_dup_string(lvalue_t* v, const char* vstring) {
  return_value_if_fail(v != NULL && vstring != NULL, NULL);

  return lvalue_dup_nstring(v, vstring, strlen(vstring));
}

lvalue_t* lvalue_dup_nstring(lvalue_t* v, const char* vstring, uint32_t size) {
  return_value_if_fail(v != NULL && vstring != NULL, NULL);

  v->type = LVALUE_TYPE_STRING;
  v->free_handle = TRUE;
  v->v.vstring = lstrndup(vstring, size);

  return v;
}

int32_t lvalue_compare_with_str(const lvalue_t* v, const char* str) {
  return lstrcmp(lvalue_string(v), str);
}

uint32_t lvalue_type_size(lvalue_type_t type) {
  switch (type) {
    case LVALUE_TYPE_INT8: {
      return sizeof(int8_t);
    }
    case LVALUE_TYPE_UINT8: {
      return sizeof(uint8_t);
    }
    case LVALUE_TYPE_INT16: {
      return sizeof(int16_t);
    }
    case LVALUE_TYPE_UINT16: {
      return sizeof(uint16_t);
    }
    case LVALUE_TYPE_INT32: {
      return sizeof(int32_t);
    }
    case LVALUE_TYPE_UINT32: {
      return sizeof(uint32_t);
    }
    case LVALUE_TYPE_INT64: {
      return sizeof(int64_t);
    }
    case LVALUE_TYPE_UINT64: {
      return sizeof(uint64_t);
    }
    case LVALUE_TYPE_FLOAT: {
      return sizeof(float);
    }
    case LVALUE_TYPE_DOUBLE: {
      return sizeof(double);
    }
    case LVALUE_TYPE_OBJECT:
    case LVALUE_TYPE_STRING:
    case LVALUE_TYPE_POINTER: {
      return sizeof(luser_data_t);
    }
    case LVALUE_TYPE_BINARY: {
      return sizeof(binary_data_t);
    }
    case LVALUE_TYPE_ID:
    case LVALUE_TYPE_SIZED_STRING: {
      return sizeof(sized_str_t);
    }
    default: {
      return sizeof(void*);
    }
  }
}
