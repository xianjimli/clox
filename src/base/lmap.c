#include "base/lmem.h"
#include "base/lmap.h"
#include "base/lnamed_value.h"

static ret_t lobject_map_get(lobject_t* o, const char* name, lvalue_t* v) {
  lmap_t* map = LMAP(o);

  if (LSTR_EQ(name, LOBJECT_PROP_SIZE)) {
    lvalue_set_uint32(v, map->array->size);
    return RET_OK;
  } else if (LSTR_EQ(name, LOBJECT_PROP_CAPACITY)) {
    lvalue_set_uint32(v, map->array->capacity);
    return RET_OK;
  } else {
    return lmap_get(map, name, v);
  }
}

static ret_t lobject_map_set(lobject_t* o, const char* name, const lvalue_t* v) {
  lmap_t* map = LMAP(o);

  return lmap_set(map, name, v);
}

static ret_t lobject_map_on_destroy(lobject_t* o) {
  lmap_t* map = LMAP(o);
  LOBJECT_UNREF(map->array);

  return RET_OK;
}

static lobject_vtable_t s_object_map_vtable = {.etype = 0,
                                               .size = sizeof(lmap_t),
                                               .type = "object_map",
                                               .desc = "object_map",
                                               .get = lobject_map_get,
                                               .set = lobject_map_set,
                                               .on_destroy = lobject_map_on_destroy};

static int32_t lmap_value_compare(lvalue_t* v, const char* b) {
  lnamed_value_t* a = LNAMED_VALUE(lvalue_object(v));

  return lnamed_value_compare_by_name(a, b);
}

lmap_t* lmap_create(void) {
  lobject_t* o = lobject_create(&s_object_map_vtable);
  lmap_t* map = LMAP(o);
  return_value_if_fail(map != NULL, NULL);

  map->array = larray_create(5);
  if (map->array == NULL) {
    LOBJECT_UNREF(o);
    return NULL;
  }

  return map;
}

ret_t lmap_clear(lmap_t* map) {
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);

  return larray_clear(map->array);
}

ret_t lmap_set(lmap_t* map, const char* name, const lvalue_t* v) {
  int32_t index = 0;
  return_value_if_fail(map != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  index = larray_find(map->array, (lcompare_t)lmap_value_compare, name);
  if (index < 0) {
    lvalue_t lv;
    lnamed_value_t* nv = lnamed_value_create(name, v);
    return_value_if_fail(nv != NULL, RET_OOM);
    lvalue_set_object(&lv, LOBJECT(nv));
    if (larray_push(map->array, &lv) == RET_OK) {
      LOBJECT_UNREF(nv);
      return RET_OK;
    } else {
      LOBJECT_UNREF(nv);
      return RET_OOM;
    }
  } else {
    lvalue_t vv;
    lnamed_value_t* nv = NULL;
    larray_get(map->array, index, &vv);
    nv = LNAMED_VALUE(lvalue_object(&vv));
    return_value_if_fail(nv != NULL, RET_BAD_PARAMS);

    lvalue_deep_copy(&(nv->value), v);
    return RET_OK;
  }
}

ret_t lmap_get(lmap_t* map, const char* name, lvalue_t* v) {
  int32_t index = 0;
  return_value_if_fail(map != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  index = larray_find(map->array, (lcompare_t)lmap_value_compare, name);
  if (index >= 0) {
    lnamed_value_t* nv = NULL;
    return_value_if_fail(larray_get(map->array, index, v) == RET_OK, RET_BAD_PARAMS);

    nv = LNAMED_VALUE(lvalue_object(v));
    lvalue_copy(v, &(nv->value));
    return RET_OK;
  } else {
    memset(v, 0x00, sizeof(lvalue_t));
    return RET_NOT_FOUND;
  }
}

ret_t lmap_exist(lmap_t* map, const char* name) {
  int32_t index = 0;
  return_value_if_fail(map != NULL && name != NULL, RET_BAD_PARAMS);
  index = larray_find(map->array, (lcompare_t)lmap_value_compare, name);

  return index >= 0;
}

ret_t lmap_remove(lmap_t* map, const char* name) {
  int32_t index = 0;
  return_value_if_fail(map != NULL && name != NULL, RET_BAD_PARAMS);
  index = larray_find(map->array, (lcompare_t)lmap_value_compare, name);
  if (index >= 0) {
    return larray_remove(map->array, index);
  } else {
    return RET_NOT_FOUND;
  }
}

bool_t lmap_get_bool(lmap_t* map, const char* name, bool_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_bool(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_bool(lmap_t* map, const char* name, bool_t value) {
  lvalue_t v;
  lvalue_set_bool(&v, value);

  return lmap_set(map, name, &v);
}

int8_t lmap_get_int8(lmap_t* map, const char* name, int8_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_int8(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_int8(lmap_t* map, const char* name, int8_t value) {
  lvalue_t v;
  lvalue_set_int8(&v, value);

  return lmap_set(map, name, &v);
}

uint8_t lmap_get_uint8(lmap_t* map, const char* name, uint8_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_uint8(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_uint8(lmap_t* map, const char* name, uint8_t value) {
  lvalue_t v;
  lvalue_set_uint8(&v, value);

  return lmap_set(map, name, &v);
}

int16_t lmap_get_int16(lmap_t* map, const char* name, int16_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_int16(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_int16(lmap_t* map, const char* name, int16_t value) {
  lvalue_t v;
  lvalue_set_int16(&v, value);

  return lmap_set(map, name, &v);
}

uint16_t lmap_get_uint16(lmap_t* map, const char* name, uint16_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_uint16(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_uint16(lmap_t* map, const char* name, uint16_t value) {
  lvalue_t v;
  lvalue_set_uint16(&v, value);

  return lmap_set(map, name, &v);
}

int32_t lmap_get_int32(lmap_t* map, const char* name, int32_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_int32(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_int32(lmap_t* map, const char* name, int32_t value) {
  lvalue_t v;
  lvalue_set_int32(&v, value);

  return lmap_set(map, name, &v);
}

uint32_t lmap_get_uint32(lmap_t* map, const char* name, uint32_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_uint32(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_uint32(lmap_t* map, const char* name, uint32_t value) {
  lvalue_t v;
  lvalue_set_uint32(&v, value);

  return lmap_set(map, name, &v);
}

uint32_t lmap_get_int64(lmap_t* map, const char* name, uint32_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_int64(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_int64(lmap_t* map, const char* name, uint32_t value) {
  lvalue_t v;
  lvalue_set_int64(&v, value);

  return lmap_set(map, name, &v);
}

uint64_t lmap_get_uint64(lmap_t* map, const char* name, uint64_t defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_uint64(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_uint64(lmap_t* map, const char* name, uint64_t value) {
  lvalue_t v;
  lvalue_set_uint64(&v, value);

  return lmap_set(map, name, &v);
}

float lmap_get_float(lmap_t* map, const char* name, float defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_float(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_float(lmap_t* map, const char* name, float value) {
  lvalue_t v;
  lvalue_set_float(&v, value);

  return lmap_set(map, name, &v);
}

double lmap_get_double(lmap_t* map, const char* name, double defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_double(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_double(lmap_t* map, const char* name, double value) {
  lvalue_t v;
  lvalue_set_double(&v, value);

  return lmap_set(map, name, &v);
}

const char* lmap_get_string(lmap_t* map, const char* name, const char* defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_string(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_string(lmap_t* map, const char* name, const char* value) {
  lvalue_t v;
  lvalue_set_string(&v, value);

  return lmap_set(map, name, &v);
}

lobject_t* lmap_get_object(lmap_t* map, const char* name, lobject_t* defval) {
  lvalue_t v;
  if (lmap_get(map, name, &v) == RET_OK) {
    return lvalue_object(&v);
  } else {
    return defval;
  }
}

ret_t lmap_set_object(lmap_t* map, const char* name, lobject_t* value) {
  lvalue_t v;
  lvalue_set_object(&v, value);

  return lmap_set(map, name, &v);
}
