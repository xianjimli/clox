#include "interpreter/lobject_lox.h"
#include "interpreter/lcallable.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lcallable_func.h"
#include "interpreter/lcallable_class.h"

static ret_t lobject_lox_unset(lobject_t* o, const char* name) {
  lobject_lox_t* lox = LOBJECT_LOX(o);
  return_value_if_fail(lox != NULL, RET_BAD_PARAMS);

  return lmap_remove(lox->props, name);
}

static ret_t lobject_lox_get(lobject_t* o, const char* name, lvalue_t* v) {
  lobject_lox_t* lox = LOBJECT_LOX(o);
  return_value_if_fail(lox != NULL, RET_BAD_PARAMS);

  if (lmap_get(lox->props, name, v) == RET_OK) {
    return RET_OK;
  } else {
    lcallable_t* callable = NULL;
    lstmt_t* func = lstmt_find_func(lox->clazz, name);
    if (func != NULL) {
      callable = lcallable_func_create(o, func);
      lvalue_set_object(v, LOBJECT(callable));
      lmap_set(lox->props, name, v);
      LOBJECT_UNREF(callable);

      return RET_OK;
    }
  }

  if (lox->super != NULL) {
    return lobject_get(lox->super, name, v);
  }

  return RET_NOT_FOUND;
}

static ret_t lobject_lox_set(lobject_t* o, const char* name, const lvalue_t* v) {
  lobject_lox_t* lox = LOBJECT_LOX(o);
  return_value_if_fail(lox != NULL, RET_BAD_PARAMS);
  /*FIXME:*/
  return lmap_set(lox->props, name, v);
}

static ret_t lobject_lox_on_destroy(lobject_t* o) {
  lobject_lox_t* lox = LOBJECT_LOX(o);
  return_value_if_fail(lox != NULL, RET_BAD_PARAMS);

  LOBJECT_UNREF(lox->super);
  LOBJECT_UNREF(lox->props);

  return RET_OK;
}

static lobject_vtable_t s_object_lox_vtable = {.etype = 0,
                                               .size = sizeof(lobject_lox_t),
                                               .type = "object_lox",
                                               .desc = "object_lox",
                                               .get = lobject_lox_get,
                                               .set = lobject_lox_set,
                                               .unset = lobject_lox_unset,
                                               .on_destroy = lobject_lox_on_destroy};

lobject_lox_t* lobject_lox_cast(lobject_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_lox_vtable, NULL);

  return (lobject_lox_t*)obj;
}

static ret_t lobject_lox_call_init(lobject_lox_t* lox, linterpreter_t* interpreter, lvalue_t* argv,
                                   uint32_t argc) {
  lcallable_t* callable = NULL;
  lstmt_t* func = lstmt_find_func(lox->clazz, LSTR_TOKEN_INIT);

  if (func != NULL) {
    callable = lcallable_func_create(LOBJECT(lox), func);
    if (callable != NULL) {
      lvalue_t v;
      lcallable_call(callable, interpreter, argv, argc, &v);
      LOBJECT_UNREF(callable);
    }
  }

  return RET_OK;
}

lobject_t* lobject_lox_create_only(lstmt_t* clazz) {
  lobject_t* o = NULL;
  lobject_lox_t* obj = NULL;
  return_value_if_fail(clazz != NULL && clazz->type == LTOKEN_CLASS, NULL);
  o = lobject_create(&s_object_lox_vtable);
  obj = LOBJECT_LOX(o);
  return_value_if_fail(obj != NULL, NULL);

  obj->clazz = clazz;
  obj->props = lmap_create();
  if (obj->props == NULL) {
    LOBJECT_UNREF(o);
    return NULL;
  }

  return o;
}

lobject_t* lobject_lox_create(lstmt_t* clazz, linterpreter_t* interpreter, lvalue_t* argv,
                              uint32_t argc) {
  lobject_t* o = lobject_lox_create_only(clazz);
  lobject_lox_t* obj = LOBJECT_LOX(o);
  return_value_if_fail(clazz != NULL && obj != NULL, NULL);

  if (clazz->data.stmt_class.super != NULL) {
    lvalue_t v;
    lcallable_clazz_t* super_clazz = NULL;
    linterpreter_get_var(interpreter, clazz->data.stmt_class.super, &v);
    super_clazz = LCALLABLE_CLASS(lvalue_object(&v));
    obj->super = lobject_lox_create_only(super_clazz->clazz);
  }

  lobject_lox_call_init(obj, interpreter, argv, argc);

  return o;
}
