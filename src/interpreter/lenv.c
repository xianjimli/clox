#include "base/lmem.h"
#include "interpreter/lenv.h"

lenv_t* lenv_create(lenv_t* enclosing) {
  lenv_t* env = LMEM_NEW(lenv_t);
  return_value_if_fail(env != NULL, NULL);

  env->enclosing = enclosing;
  env->values = lmap_create();
  if (enclosing != NULL) {
    env->level = enclosing->level + 1;
  }

  return env;
}

ret_t lenv_get(lenv_t* env, const char* name, lvalue_t* v) {
  return_value_if_fail(env != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (lmap_get(env->values, name, v) == RET_OK) {
    return RET_OK;
  } else if (env->enclosing != NULL) {
    return lenv_get(env->enclosing, name, v);
  }

  return RET_NOT_FOUND;
}

ret_t lenv_set(lenv_t* env, const char* name, const lvalue_t* v) {
  lvalue_t vv;
  return_value_if_fail(env != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (lmap_get(env->values, name, &vv) == RET_OK) {
    return lmap_set(env->values, name, v);
  } else if (env->enclosing != NULL) {
    return lenv_set(env->enclosing, name, v);
  }

  return RET_NOT_FOUND;
}

lenv_t* lenv_get_ancestor(lenv_t* env, uint32_t distance) {
  lenv_t* iter = env;
  return_value_if_fail(env != NULL, NULL);
  while (distance > 0 && iter != NULL) {
    iter = iter->enclosing;
    distance--;
  }

  return iter;
}

ret_t lenv_get_at(lenv_t* env, uint32_t distance, const char* name, lvalue_t* v) {
  lenv_t* ancestor = lenv_get_ancestor(env, distance);
  return_value_if_fail(ancestor != NULL, RET_BAD_PARAMS);

  return lenv_get(ancestor, name, v);
}

ret_t lenv_set_at(lenv_t* env, uint32_t distance, const char* name, const lvalue_t* v) {
  lenv_t* ancestor = lenv_get_ancestor(env, distance);
  return_value_if_fail(ancestor != NULL, RET_BAD_PARAMS);

  return lenv_set(ancestor, name, v);
}

ret_t lenv_define(lenv_t* env, const char* name, const lvalue_t* v) {
  return lmap_set(env->values, name, v);
}

ret_t lenv_destroy(lenv_t* env) {
  return_value_if_fail(env != NULL, RET_BAD_PARAMS);
  LOBJECT_UNREF(env->values);
  LMEM_FREE(env);

  return RET_OK;
}
