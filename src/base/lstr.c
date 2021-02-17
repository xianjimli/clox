#include "base/lstr.h"
#include "base/lmem.h"

static ret_t lobject_str_get(lobject_t* o, const char* name, lvalue_t* v) {
  lstr_t* str = LSTR(o);

  if (LSTR_EQ(name, LOBJECT_PROP_SIZE)) {
    lvalue_set_uint32(v, str->size);
    return RET_OK;
  } else if (LSTR_EQ(name, LOBJECT_PROP_CAPACITY)) {
    lvalue_set_uint32(v, str->capacity);
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t lobject_str_on_destroy(lobject_t* o) {
  lstr_t* str = LSTR(o);

  LMEM_FREE(str->str);

  return RET_OK;
}

static lobject_vtable_t s_object_str_vtable = {.etype = 0,
                                               .size = sizeof(lstr_t),
                                               .type = "object_str",
                                               .desc = "object_str",
                                               .get = lobject_str_get,
                                               .on_destroy = lobject_str_on_destroy};
static ret_t lstr_extend(lstr_t* str, int32_t capacity) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (str->capacity <= capacity) {
    char* data = LMEM_REALLOC(str->str, capacity + 1);
    return_value_if_fail(data != NULL, RET_OOM);
    str->str = data;
    str->capacity = capacity;
  }

  return RET_OK;
}

lstr_t* lstr_create(uint32_t capacity) {
  lobject_t* o = lobject_create(&s_object_str_vtable);
  lstr_t* str = LSTR(o);
  return_value_if_fail(str != NULL, NULL);

  lstr_extend(str, capacity);

  return str;
}

ret_t lstr_clear(lstr_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  str->size = 0;
  str->str[0] = '\0';

  return RET_OK;
}

ret_t lstr_set(lstr_t* str, const char* data, int32_t size) {
  return_value_if_fail(str != NULL && data != NULL, RET_BAD_PARAMS);
  size = size < 0 ? strlen(data) : size;
  return_value_if_fail(lstr_extend(str, size + 1) == RET_OK, RET_OOM);

  if (size > 0) {
    memcpy(str->str, data, size);
    str->str[size] = '\0';
    str->size = size;
  }

  return RET_OK;
}

ret_t lstr_append(lstr_t* str, const char* data, int32_t size) {
  return_value_if_fail(str != NULL && data != NULL, RET_BAD_PARAMS);
  size = size < 0 ? strlen(data) : size;
  return_value_if_fail(lstr_extend(str, str->size + size + 1) == RET_OK, RET_OOM);

  if (size > 0) {
    memcpy(str->str + str->size, data, size);
    str->size += size;
    str->str[str->size] = '\0';
  }

  return RET_OK;
}

ret_t lstr_append_value(lstr_t* str, const lvalue_t* v) {
  char buff[64];
  const char* p = buff;
  uint32_t size = sizeof(buff);
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case LVALUE_TYPE_NIL: {
      p = "nil";
      break;
    }
    case LVALUE_TYPE_BOOL: {
      p = (v->v.vbool) ? "true" : "false";
      break;
    }
    case LVALUE_TYPE_INT8: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint8));
      break;
    }
    case LVALUE_TYPE_UINT8: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint8));
      break;
    }
    case LVALUE_TYPE_INT16: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint16));
      break;
    }
    case LVALUE_TYPE_UINT16: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint16));
      break;
    }
    case LVALUE_TYPE_INT32: {
      snprintf(buff, size, "%d", (int32_t)(v->v.vint32));
      break;
    }
    case LVALUE_TYPE_UINT32: {
      snprintf(buff, size, "%u", (uint32_t)(v->v.vuint32));
      break;
    }
    case LVALUE_TYPE_INT64: {
      snprintf(buff, size, "%" PRId64, (int64_t)(v->v.vint64));
      break;
    }
    case LVALUE_TYPE_UINT64: {
      snprintf(buff, size, "%" PRIu64, (uint64_t)(v->v.vuint64));
      break;
    }
    case LVALUE_TYPE_FLOAT: {
      snprintf(buff, size, "%f", (v->v.vfloat));
      break;
    }
    case LVALUE_TYPE_DOUBLE: {
      snprintf(buff, size, "%lf", (v->v.vdouble));
      break;
    }
    case LVALUE_TYPE_STRING: {
      p = lvalue_string(v);
      break;
    }
    case LVALUE_TYPE_SIZED_STRING: {
      return lstr_append(str, v->v.vsized_string.str, v->v.vsized_string.size);
    }
    case LVALUE_TYPE_ID: {
      return lstr_append(str, v->v.vid.str, v->v.vid.size);
    }
    default: {
      return RET_BAD_PARAMS;
    }
  }

  if (p != NULL) {
    lstr_append(str, p, -1);
  }

  return RET_OK;
}

ret_t lstr_append_int(lstr_t* str, const char* format, int32_t value) {
  char buff[64];
  snprintf(buff, sizeof(buff) - 1, format != NULL ? format : "%d", value);

  return lstr_append(str, buff, -1);
}

ret_t lstr_append_float(lstr_t* str, const char* format, double value) {
  char buff[64];
  snprintf(buff, sizeof(buff) - 1, format != NULL ? format : "%lf", value);

  return lstr_append(str, buff, -1);
}

ret_t lstr_append_format_str(lstr_t* str, const char* format, const char* value) {
  char buff[128];
  snprintf(buff, sizeof(buff) - 1, format != NULL ? format : "%s", value);

  return lstr_append(str, buff, -1);
}
