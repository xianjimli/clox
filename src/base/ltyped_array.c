#include "base/lmem.h"
#include "base/lvalue.h"
#include "base/ltyped_array.h"

static bool_t ltyped_array_is_type_supported(lvalue_type_t type) {
  switch (type) {
    case LVALUE_TYPE_INT8:
    case LVALUE_TYPE_UINT8:
    case LVALUE_TYPE_INT16:
    case LVALUE_TYPE_UINT16:
    case LVALUE_TYPE_INT32:
    case LVALUE_TYPE_UINT32:
    case LVALUE_TYPE_INT64:
    case LVALUE_TYPE_UINT64:
    case LVALUE_TYPE_FLOAT:
    case LVALUE_TYPE_DOUBLE: {
      return TRUE;
    }
    default: {
      return FALSE;
    }
  }
}

ltyped_array_t* ltyped_array_create(lvalue_type_t type, uint32_t capacity) {
  ltyped_array_t* ltyped_array = NULL;
  uint32_t element_size = lvalue_type_size(type);
  return_value_if_fail(ltyped_array_is_type_supported(type), NULL);
  ltyped_array = LMEM_NEW(ltyped_array_t);
  return_value_if_fail(ltyped_array != NULL, NULL);

  ltyped_array->type = type;
  ltyped_array->element_size = element_size;
  if (capacity > 0) {
    if (ltyped_array_extend(ltyped_array, capacity) != RET_OK) {
      LMEM_FREE(ltyped_array);
      return NULL;
    }
  }

  return ltyped_array;
}

ret_t ltyped_array_get(ltyped_array_t* ltyped_array, uint32_t index, lvalue_t* v) {
  uint8_t* p = NULL;
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ltyped_array->data != NULL && index < ltyped_array->size, RET_BAD_PARAMS);
  p = ltyped_array->data + index * ltyped_array->element_size;

  switch (ltyped_array->type) {
    case LVALUE_TYPE_INT8: {
      lvalue_set_int8(v, *(int8_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT8: {
      lvalue_set_uint8(v, *(uint8_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_INT16: {
      lvalue_set_int16(v, *(int16_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT16: {
      lvalue_set_uint16(v, *(uint16_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_INT32: {
      lvalue_set_int32(v, *(int32_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT32: {
      lvalue_set_uint32(v, *(uint32_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_INT64: {
      lvalue_set_int64(v, *(int64_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT64: {
      lvalue_set_uint64(v, *(uint64_t*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_FLOAT: {
      lvalue_set_float(v, *(float*)p);
      return RET_OK;
    }
    case LVALUE_TYPE_DOUBLE: {
      lvalue_set_double(v, *(double*)p);
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_NOT_IMPL;
    }
  }
}

ret_t ltyped_array_set(ltyped_array_t* ltyped_array, uint32_t index, const lvalue_t* v) {
  uint8_t* p = NULL;
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ltyped_array->data != NULL && index < ltyped_array->size, RET_BAD_PARAMS);
  p = ltyped_array->data + index * ltyped_array->element_size;

  switch (ltyped_array->type) {
    case LVALUE_TYPE_INT8: {
      *(int8_t*)p = lvalue_int8(v);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT8: {
      *(uint8_t*)p = lvalue_uint8(v);
      return RET_OK;
    }
    case LVALUE_TYPE_INT16: {
      *(int16_t*)p = lvalue_int16(v);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT16: {
      *(uint16_t*)p = lvalue_uint16(v);
      return RET_OK;
    }
    case LVALUE_TYPE_INT32: {
      *(int32_t*)p = lvalue_int32(v);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT32: {
      *(uint32_t*)p = lvalue_uint32(v);
      return RET_OK;
    }
    case LVALUE_TYPE_INT64: {
      *(int64_t*)p = lvalue_int64(v);
      return RET_OK;
    }
    case LVALUE_TYPE_UINT64: {
      *(uint64_t*)p = lvalue_uint64(v);
      return RET_OK;
    }
    case LVALUE_TYPE_FLOAT: {
      *(float*)p = lvalue_float(v);
      return RET_OK;
    }
    case LVALUE_TYPE_DOUBLE: {
      *(double*)p = lvalue_double(v);
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_NOT_IMPL;
    }
  }
}

ret_t ltyped_array_extend(ltyped_array_t* ltyped_array, uint32_t capacity) {
  return_value_if_fail(ltyped_array != NULL, RET_BAD_PARAMS);
  if (capacity > ltyped_array->capacity) {
    uint8_t* p = NULL;
    uint32_t mem_size = (capacity + 1) * ltyped_array->element_size;
    uint32_t data_size = ltyped_array->element_size * ltyped_array->size;
    uint8_t* data = (uint8_t*)LMEM_REALLOC(ltyped_array->data, mem_size);
    return_value_if_fail(data != NULL, RET_OOM);
    ENSURE(((uint64_t)data % 8) == 0);

    ltyped_array->data = data;
    ltyped_array->capacity = capacity;
    p = data + data_size;
    memset(p, 0x00, mem_size - data_size);
  }

  return RET_OK;
}

static ret_t ltyped_array_extend_delta(ltyped_array_t* ltyped_array, uint32_t delta) {
  return_value_if_fail(ltyped_array != NULL, RET_BAD_PARAMS);

  if ((ltyped_array->size + delta) > ltyped_array->capacity) {
    uint32_t capacity = (ltyped_array->size + delta) + 1.2F;
    return ltyped_array_extend(ltyped_array, capacity);
  }

  return RET_OK;
}

ret_t ltyped_array_insert(ltyped_array_t* ltyped_array, uint32_t index, const lvalue_t* v) {
  uint8_t* p = NULL;
  uint8_t* s = NULL;
  uint8_t* d = NULL;
  uint32_t element_size = 0;
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);
  index = LMIN(index, ltyped_array->size);
  return_value_if_fail(ltyped_array_extend_delta(ltyped_array, 1) == RET_OK, RET_OOM);

  element_size = ltyped_array->element_size;
  p = ltyped_array->data + index * element_size;
  d = ltyped_array->data + element_size * ltyped_array->size;
  s = d - element_size;

  while (s >= p) {
    memcpy(d, s, element_size);
    s -= element_size;
    d -= element_size;
  }
  ltyped_array->size++;

  return ltyped_array_set(ltyped_array, index, v);
}

ret_t ltyped_array_remove(ltyped_array_t* ltyped_array, uint32_t index) {
  uint8_t* p = NULL;
  uint8_t* s = NULL;
  uint8_t* d = NULL;
  uint32_t element_size = 0;
  return_value_if_fail(ltyped_array != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ltyped_array->data != NULL && index < ltyped_array->size, RET_BAD_PARAMS);

  element_size = ltyped_array->element_size;
  d = ltyped_array->data + index * element_size;
  s = d + element_size;
  p = ltyped_array->data + element_size * ltyped_array->size;

  while (s < p) {
    memcpy(d, s, element_size);
    s += element_size;
    d += element_size;
  }
  ltyped_array->size--;

  return RET_OK;
}

ret_t ltyped_array_pop(ltyped_array_t* ltyped_array, lvalue_t* v) {
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ltyped_array->size > 0, RET_BAD_PARAMS);

  ltyped_array_get(ltyped_array, ltyped_array->size - 1, v);
  ltyped_array->size--;

  return RET_OK;
}

ret_t ltyped_array_tail(ltyped_array_t* ltyped_array, lvalue_t* v) {
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ltyped_array->size > 0, RET_BAD_PARAMS);

  return ltyped_array_get(ltyped_array, ltyped_array->size - 1, v);
}

ret_t ltyped_array_push(ltyped_array_t* ltyped_array, const lvalue_t* v) {
  return_value_if_fail(ltyped_array != NULL && v != NULL, RET_BAD_PARAMS);

  return ltyped_array_insert(ltyped_array, ltyped_array->size, v);
}

ret_t ltyped_array_clear(ltyped_array_t* ltyped_array) {
  return_value_if_fail(ltyped_array != NULL, RET_BAD_PARAMS);
  ltyped_array->size = 0;

  return RET_OK;
}

ret_t ltyped_array_destroy(ltyped_array_t* ltyped_array) {
  return_value_if_fail(ltyped_array != NULL, RET_BAD_PARAMS);
  ltyped_array_clear(ltyped_array);
  LMEM_FREE(ltyped_array->data);
  memset(ltyped_array, 0x00, sizeof(ltyped_array_t));
  LMEM_FREE(ltyped_array);

  return RET_OK;
}
