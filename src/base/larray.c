#include "base/lmem.h"
#include "base/larray.h"
#include "base/lstr_utils.h"

static ret_t lobject_array_get(lobject_t* o, const char* name, lvalue_t* v) {
  larray_t* array = LARRAY(o);

  if (LSTR_EQ(name, LOBJECT_PROP_SIZE)) {
    lvalue_set_uint32(v, array->size);
    return RET_OK;
  } else if (LSTR_EQ(name, LOBJECT_PROP_CAPACITY)) {
    lvalue_set_uint32(v, array->capacity);
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t lobject_array_on_destroy(lobject_t* o) {
  uint32_t i = 0;
  larray_t* array = LARRAY(o);

  return_value_if_fail(array != NULL, RET_BAD_PARAMS);
  for (i = 0; i < array->size; i++) {
    lvalue_reset(array->data + i);
  }
  array->size = 0;
  LMEM_FREE(array->data);

  return RET_OK;
}

static lobject_vtable_t s_object_array_vtable = {.etype = 0,
                                                 .size = sizeof(larray_t),
                                                 .type = "object_array",
                                                 .desc = "object_array",
                                                 .get = lobject_array_get,
                                                 .on_destroy = lobject_array_on_destroy};

static ret_t larray_extend(larray_t* array) {
  ret_t ret = RET_OOM;
  return_value_if_fail(array != NULL, RET_BAD_PARAMS);

  if (array->size < array->capacity) {
    ret = RET_OK;
  } else {
    lvalue_t* data = NULL;
    uint32_t capacity = array->capacity + (array->capacity >> 1) + 1;
    data = (lvalue_t*)LMEM_REALLOC(array->data, capacity * sizeof(lvalue_t));

    if (data != NULL) {
      array->data = data;
      array->capacity = capacity;
      ret = RET_OK;
    }
  }

  return ret;
}

larray_t* larray_create(uint32_t capacity) {
  lobject_t* o = lobject_create(&s_object_array_vtable);
  larray_t* array = LARRAY(o);
  return_value_if_fail(array != NULL, NULL);

  if (capacity > 0) {
    array->data = LMEM_NEW_ARRAY(lvalue_t, capacity);
    array->capacity = capacity;

    if (array->data == NULL) {
      LOBJECT_UNREF(o);
      return NULL;
    }
  }

  return array;
}

ret_t larray_clear(larray_t* array) {
  uint32_t i = 0;
  return_value_if_fail(array != NULL, RET_BAD_PARAMS);

  for (i = 0; i < array->size; i++) {
    lvalue_t* iter = array->data + i;
    lvalue_reset(iter);
  }

  array->size = 0;

  return RET_OK;
}

ret_t larray_set(larray_t* array, uint32_t index, const lvalue_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(array != NULL && index < array->size, RET_BAD_PARAMS);

  lvalue_reset(array->data + index);

  lvalue_deep_copy(array->data + index, v);

  return RET_OK;
}

ret_t larray_get(larray_t* array, uint32_t index, lvalue_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(array != NULL && index < array->size, RET_BAD_PARAMS);

  lvalue_copy(v, array->data + index);

  return RET_OK;
}

ret_t larray_remove(larray_t* array, uint32_t index) {
  uint32_t i = 0;
  uint32_t nr = 0;
  return_value_if_fail(array != NULL && index < array->size, RET_BAD_PARAMS);

  lvalue_reset(array->data + index);
  nr = array->size - 1;
  for (i = index; i < nr; i++) {
    array->data[i] = array->data[i + 1];
  }
  array->size--;
  memset(array->data + nr, 0x00, sizeof(lvalue_t));

  return RET_OK;
}

lvalue_t* larray_top(larray_t* array) {
  return_value_if_fail(array != NULL && array->size > 0, NULL);
  return array->data + array->size - 1;
}

ret_t larray_pop(larray_t* array, lvalue_t* v) {
  lvalue_t* vv = NULL;
  return_value_if_fail(array != NULL && array->size > 0, RET_BAD_PARAMS);
  vv = array->data + array->size - 1;

  lvalue_deep_copy(v, vv);

  return larray_remove(array, array->size - 1);
}

ret_t larray_insert(larray_t* array, uint32_t index, const lvalue_t* v) {
  lvalue_t* s = NULL;
  lvalue_t* d = NULL;
  lvalue_t* p = NULL;
  return_value_if_fail(array != NULL && v != NULL, RET_BAD_PARAMS);
  index = LMIN(index, array->size);
  return_value_if_fail(larray_extend(array) == RET_OK, RET_OOM);

  p = array->data + index;
  d = array->data + array->size;
  s = d - 1;

  while (s >= p) {
    *d-- = *s--;
  }
  lvalue_deep_copy(p, v);
  array->size++;

  return RET_OK;
}

ret_t larray_push(larray_t* array, const lvalue_t* v) {
  return_value_if_fail(array != NULL, RET_BAD_PARAMS);

  return larray_insert(array, array->size, v);
}

int32_t larray_find(larray_t* array, lcompare_t compare, const void* ctx) {
  uint32_t i = 0;
  return_value_if_fail(array != NULL && compare != NULL, -1);

  for (i = 0; i < array->size; i++) {
    if (compare(array->data + i, ctx) == 0) {
      return i;
    }
  }

  return -1;
}

bool_t larray_get_bool(larray_t* array, uint32_t index, bool_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_bool(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_bool(larray_t* array, uint32_t index, bool_t value) {
  lvalue_t v;
  lvalue_set_bool(&v, value);
  return larray_set(array, index, &v);
}

int8_t larray_get_int8(larray_t* array, uint32_t index, int8_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_int8(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_int8(larray_t* array, uint32_t index, int8_t value) {
  lvalue_t v;
  lvalue_set_int8(&v, value);
  return larray_set(array, index, &v);
}

uint8_t larray_get_uint8(larray_t* array, uint32_t index, uint8_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_uint8(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_uint8(larray_t* array, uint32_t index, uint8_t value) {
  lvalue_t v;
  lvalue_set_uint8(&v, value);
  return larray_set(array, index, &v);
}

int16_t larray_get_int16(larray_t* array, uint32_t index, int16_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_int16(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_int16(larray_t* array, uint32_t index, int16_t value) {
  lvalue_t v;
  lvalue_set_int16(&v, value);
  return larray_set(array, index, &v);
}

uint16_t larray_get_uint16(larray_t* array, uint32_t index, uint16_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_uint16(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_uint16(larray_t* array, uint32_t index, uint16_t value) {
  lvalue_t v;
  lvalue_set_uint16(&v, value);
  return larray_set(array, index, &v);
}

int32_t larray_get_int32(larray_t* array, uint32_t index, int32_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_int32(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_int32(larray_t* array, uint32_t index, int32_t value) {
  lvalue_t v;
  lvalue_set_int32(&v, value);
  return larray_set(array, index, &v);
}

uint32_t larray_get_uint32(larray_t* array, uint32_t index, uint32_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_uint32(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_uint32(larray_t* array, uint32_t index, uint32_t value) {
  lvalue_t v;
  lvalue_set_uint32(&v, value);
  return larray_set(array, index, &v);
}

uint32_t larray_get_int64(larray_t* array, uint32_t index, uint32_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_int64(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_int64(larray_t* array, uint32_t index, uint32_t value) {
  lvalue_t v;
  lvalue_set_int64(&v, value);
  return larray_set(array, index, &v);
}

uint64_t larray_get_uint64(larray_t* array, uint32_t index, uint64_t defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_uint64(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_uint64(larray_t* array, uint32_t index, uint64_t value) {
  lvalue_t v;
  lvalue_set_uint64(&v, value);
  return larray_set(array, index, &v);
}

float larray_get_float(larray_t* array, uint32_t index, float defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_float(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_float(larray_t* array, uint32_t index, float value) {
  lvalue_t v;
  lvalue_set_float(&v, value);
  return larray_set(array, index, &v);
}

double larray_get_double(larray_t* array, uint32_t index, double defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_double(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_double(larray_t* array, uint32_t index, double value) {
  lvalue_t v;
  lvalue_set_double(&v, value);
  return larray_set(array, index, &v);
}

const char* larray_get_string(larray_t* array, uint32_t index, const char* defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_string(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_string(larray_t* array, uint32_t index, const char* value) {
  lvalue_t v;
  lvalue_set_string(&v, value);
  return larray_set(array, index, &v);
}

lobject_t* larray_get_object(larray_t* array, uint32_t index, lobject_t* defval) {
  lvalue_t v;
  if (larray_get(array, index, &v) == RET_OK) {
    return lvalue_object(&v);
  } else {
    return defval;
  }
}

ret_t larray_set_object(larray_t* array, uint32_t index, lobject_t* value) {
  lvalue_t v;
  lvalue_set_object(&v, value);
  return larray_set(array, index, &v);
}
