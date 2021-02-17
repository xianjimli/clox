#include "base/lobject.h"

ret_t lobject_unset(lobject_t* o, const char* name) {
  return_value_if_fail(o != NULL && o->vt != NULL && name != NULL, RET_BAD_PARAMS);

  return o->vt->unset(o, name);
}

ret_t lobject_get(lobject_t* o, const char* name, lvalue_t* v) {
  return_value_if_fail(o != NULL && o->vt != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return o->vt->get(o, name, v);
}

ret_t lobject_set(lobject_t* o, const char* name, const lvalue_t* v) {
  return_value_if_fail(o != NULL && o->vt != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return o->vt->set(o, name, v);
}

static ret_t lobject_on_destroy(lobject_t* o) {
  return_value_if_fail(o != NULL && o->vt != NULL, RET_BAD_PARAMS);
  if (o->vt->on_destroy != NULL) {
    o->vt->on_destroy(o);
  }
  memset(o, 0x00, o->vt->size);
  LMEM_FREE(o);

  return RET_OK;
}

lobject_t* lobject_ref(lobject_t* o) {
  return_value_if_fail(o != NULL, NULL);

  o->refcount++;

  return o;
}

ret_t lobject_unref(lobject_t* o) {
  return_value_if_fail(o != NULL && o->vt != NULL && o->refcount > 0, RET_BAD_PARAMS);
  if (o->refcount == 1) {
    lobject_on_destroy(o);
  } else {
    o->refcount--;
  }

  return RET_OK;
}

lobject_t* lobject_create(const lobject_vtable_t* vt) {
  lobject_t* o = NULL;
  return_value_if_fail(vt != NULL, NULL);
  o = LMEM_ALLOC(vt->size);
  return_value_if_fail(o != NULL, NULL);
  memset(o, 0x00, vt->size);

  o->refcount = 1;
  o->vt = vt;

  return o;
}

bool_t lobject_get_bool(lobject_t* o, const char* name, bool_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_bool(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_bool(lobject_t* o, const char* name, bool_t value) {
  lvalue_t v;
  lvalue_set_bool(&v, value);

  return lobject_set(o, name, &v);
}

int8_t lobject_get_int8(lobject_t* o, const char* name, int8_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_int8(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_int8(lobject_t* o, const char* name, int8_t value) {
  lvalue_t v;
  lvalue_set_int8(&v, value);

  return lobject_set(o, name, &v);
}

uint8_t lobject_get_uint8(lobject_t* o, const char* name, uint8_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_uint8(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_uint8(lobject_t* o, const char* name, uint8_t value) {
  lvalue_t v;
  lvalue_set_uint8(&v, value);

  return lobject_set(o, name, &v);
}

int16_t lobject_get_int16(lobject_t* o, const char* name, int16_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_int16(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_int16(lobject_t* o, const char* name, int16_t value) {
  lvalue_t v;
  lvalue_set_int16(&v, value);

  return lobject_set(o, name, &v);
}

uint16_t lobject_get_uint16(lobject_t* o, const char* name, uint16_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_uint16(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_uint16(lobject_t* o, const char* name, uint16_t value) {
  lvalue_t v;
  lvalue_set_uint16(&v, value);

  return lobject_set(o, name, &v);
}

int32_t lobject_get_int32(lobject_t* o, const char* name, int32_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_int32(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_int32(lobject_t* o, const char* name, int32_t value) {
  lvalue_t v;
  lvalue_set_int32(&v, value);

  return lobject_set(o, name, &v);
}

uint32_t lobject_get_uint32(lobject_t* o, const char* name, uint32_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_uint32(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_uint32(lobject_t* o, const char* name, uint32_t value) {
  lvalue_t v;
  lvalue_set_uint32(&v, value);

  return lobject_set(o, name, &v);
}

int64_t lobject_get_int64(lobject_t* o, const char* name, int64_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_int64(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_int64(lobject_t* o, const char* name, int64_t value) {
  lvalue_t v;
  lvalue_set_int64(&v, value);

  return lobject_set(o, name, &v);
}

uint64_t lobject_get_uint64(lobject_t* o, const char* name, uint64_t defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_uint64(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_uint64(lobject_t* o, const char* name, uint64_t value) {
  lvalue_t v;
  lvalue_set_uint64(&v, value);

  return lobject_set(o, name, &v);
}

float lobject_get_float(lobject_t* o, const char* name, float defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_float(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_float(lobject_t* o, const char* name, float value) {
  lvalue_t v;
  lvalue_set_float(&v, value);

  return lobject_set(o, name, &v);
}

double lobject_get_double(lobject_t* o, const char* name, double defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_double(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_double(lobject_t* o, const char* name, double value) {
  lvalue_t v;
  lvalue_set_double(&v, value);

  return lobject_set(o, name, &v);
}

const char* lobject_get_string(lobject_t* o, const char* name, const char* defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_string(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_string(lobject_t* o, const char* name, const char* value) {
  lvalue_t v;
  lvalue_set_string(&v, value);

  return lobject_set(o, name, &v);
}

lobject_t* lobject_get_object(lobject_t* o, const char* name, lobject_t* defval) {
  lvalue_t v;
  if (lobject_get(o, name, &v) == RET_OK) {
    return lvalue_object(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_object(lobject_t* o, const char* name, lobject_t* value) {
  lvalue_t v;
  lvalue_set_object(&v, value);

  return lobject_set(o, name, &v);
}
