#include "base/lmem.h"
#include "base/lstr_utils.h"
#include "base/lnamed_value.h"

static ret_t lobject_named_value_get(lobject_t* o, const char* name, lvalue_t* v) {
  lnamed_value_t* named_value = LNAMED_VALUE(o);

  if (LSTR_EQ(name, LOBJECT_PROP_NAME)) {
    lvalue_set_string(v, named_value->name);
    return RET_OK;
  } else if (LSTR_EQ(name, LOBJECT_PROP_VALUE)) {
    lvalue_copy(v, &(named_value->value));
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t lobject_named_value_on_destroy(lobject_t* o) {
  lnamed_value_t* nv = LNAMED_VALUE(o);

  LMEM_FREE(nv->name);
  lvalue_reset(&(nv->value));

  return RET_OK;
}

static lobject_vtable_t s_object_named_value_vtable = {
    .etype = 0,
    .size = sizeof(lnamed_value_t),
    .type = "object_named_value",
    .desc = "object_named_value",
    .get = lobject_named_value_get,
    .on_destroy = lobject_named_value_on_destroy};

lnamed_value_t* lnamed_value_create(const char* name, const lvalue_t* v) {
  lobject_t* o = NULL;
  lnamed_value_t* nv = NULL;
  return_value_if_fail(name != NULL && v != NULL, NULL);
  o = lobject_create(&s_object_named_value_vtable);
  nv = LNAMED_VALUE(o);
  return_value_if_fail(nv != NULL, NULL);

  nv->name = lstrdup(name);
  lvalue_deep_copy(&(nv->value), v);

  return nv;
}

ret_t lnamed_value_set_name(lnamed_value_t* nv, const char* name) {
  return_value_if_fail(nv != NULL && name != NULL, RET_BAD_PARAMS);
  nv->name = lstrset(nv->name, name);

  return RET_OK;
}

ret_t lnamed_value_set_value(lnamed_value_t* nv, const lvalue_t* v) {
  return_value_if_fail(nv != NULL && v != NULL, RET_BAD_PARAMS);
  lvalue_reset(&(nv->value));
  lvalue_deep_copy(&(nv->value), v);

  return RET_OK;
}

int32_t lnamed_value_compare_by_name(lnamed_value_t* nv, const char* name) {
  return_value_if_fail(nv != NULL && name != NULL, -1);

  return lstrcmp(nv->name, name);
}
