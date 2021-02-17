#include "base/lmem.h"
#include "parser/lparser.h"
#include "interpreter/lenv.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/lresolver.h"
#include "interpreter/linterpreter.h"

static int32_t linterpreter_get_distance(linterpreter_t* interpreter, lexpr_t* expr) {
  char addr[32];
  snprintf(addr, sizeof(addr), "%p", expr);

  return lmap_get_int32(interpreter->locals, addr, -1);
}

static ret_t linterpreter_on_resolver_result(void* ctx, lexpr_t* expr, const char* name,
                                             int32_t level) {
  char addr[32];
  linterpreter_t* interpreter = (linterpreter_t*)ctx;
  snprintf(addr, sizeof(addr), "%p", expr);
  lmap_set_int32(interpreter->locals, addr, level);

  return RET_OK;
}

static ret_t linterpreter_resolve(linterpreter_t* interpreter) {
  lresolver_t* r = lresolver_create(linterpreter_on_resolver_result, interpreter);
  lresolver_resolve(r, interpreter->root);
  lresolver_destroy(r);

  return RET_OK;
}

linterpreter_t* linterpreter_create(lobject_t* global, const char* code, uint32_t size) {
  lparser_t* parser = NULL;
  linterpreter_t* interpreter = NULL;
  return_value_if_fail(code != NULL, NULL);

  interpreter = LMEM_NEW(linterpreter_t);
  return_value_if_fail(interpreter != NULL, NULL);

  parser = &(interpreter->parser);
  goto_error_if_fail(lparser_init(parser, code) != NULL);

  interpreter->global = global;
  interpreter->root = lparser_parse(parser);
  goto_error_if_fail(interpreter->root != NULL);
  interpreter->locals = lmap_create();
  goto_error_if_fail(interpreter->locals != NULL);

  interpreter->tname = lstr_create(100);
  goto_error_if_fail(interpreter->tname != NULL);

  if (interpreter->root != NULL) {
    linterpreter_resolve(interpreter);
  }

  return interpreter;
error:
  linterpreter_destroy(interpreter);

  return NULL;
}

ret_t linterpreter_exec(linterpreter_t* interpreter, lvalue_t* result) {
  return_value_if_fail(interpreter != NULL && result != NULL, RET_BAD_PARAMS);

  interpreter->env = lenv_create(NULL);
  lstmt_eval(interpreter->root, interpreter, result);
  lenv_destroy(interpreter->env);
  interpreter->env = NULL;

  return RET_OK;
}

ret_t linterpreter_destroy(linterpreter_t* interpreter) {
  return_value_if_fail(interpreter != NULL, RET_BAD_PARAMS);

  lparser_deinit(&(interpreter->parser));
  lstmt_destroy(interpreter->root);
  LOBJECT_UNREF(interpreter->tname);
  LOBJECT_UNREF(interpreter->locals);
  LMEM_FREE(interpreter);

  return RET_OK;
}

const char* linterpreter_to_name(linterpreter_t* interpreter, const sized_str_t* name) {
  lstr_set(interpreter->tname, name->str, name->size);
  return interpreter->tname->str;
}

ret_t linterpreter_define_var(linterpreter_t* interpreter, const sized_str_t* name,
                              const lvalue_t* value) {
  return_value_if_fail(interpreter != NULL && interpreter->env != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && value != NULL, RET_BAD_PARAMS);

  return lenv_define(interpreter->env, linterpreter_to_name(interpreter, name), value);
}

ret_t linterpreter_define_object(linterpreter_t* interpreter, const char* name, lobject_t* obj) {
  lvalue_t v;
  const sized_str_t aname = {strlen(name), name};
  lvalue_set_object(&v, obj);

  return linterpreter_define_var(interpreter, &aname, &v);
}

ret_t linterpreter_get_var(linterpreter_t* interpreter, lexpr_t* expr, lvalue_t* value) {
  const char* tname = NULL;
  int32_t distance = linterpreter_get_distance(interpreter, expr);
  if (expr->type == LTOKEN_THIS) {
    tname = LSTR_TOKEN_THIS;
  } else if (expr->type == LTOKEN_SUPER) {
    tname = LSTR_TOKEN_SUPER;
  } else {
    const sized_str_t* name = lexpr_get_value_id(expr);
    tname = linterpreter_to_name(interpreter, name);
  }
  return_value_if_fail(interpreter != NULL && interpreter->env != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tname != NULL && value != NULL, RET_BAD_PARAMS);

  if (distance < 0) {
    return lobject_get(interpreter->global, tname, value);
  } else {
    return lenv_get_at(interpreter->env, distance, tname, value);
  }
}

ret_t linterpreter_set_var(linterpreter_t* interpreter, lexpr_t* expr, const lvalue_t* value) {
  const sized_str_t* name = lexpr_get_value_id(expr);
  const char* tname = linterpreter_to_name(interpreter, name);
  int32_t distance = linterpreter_get_distance(interpreter, expr);
  return_value_if_fail(interpreter != NULL && interpreter->env != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && value != NULL, RET_BAD_PARAMS);

  if (distance < 0) {
    return lobject_set(interpreter->global, tname, value);
  } else {
    return lenv_set_at(interpreter->env, distance, tname, value);
  }
}

ret_t linterpreter_enter_block(linterpreter_t* interpreter) {
  return_value_if_fail(interpreter != NULL, RET_BAD_PARAMS);
  interpreter->env = lenv_create(interpreter->env);

  return RET_OK;
}

ret_t linterpreter_leave_block(linterpreter_t* interpreter) {
  lenv_t* env = NULL;
  return_value_if_fail(interpreter != NULL, RET_BAD_PARAMS);

  env = interpreter->env;
  interpreter->env = interpreter->env->enclosing;
  lenv_destroy(env);

  return RET_OK;
}
