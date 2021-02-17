#include "base/lmem.h"
#include "interpreter/lresolver.h"

static ret_t lstmt_resolve(lstmt_t* stmt, lresolver_t* resolver);
static ret_t lexpr_resolve(lexpr_t* expr, lresolver_t* resolver);
static lmap_t* lresolver_get_scope(lresolver_t* resolver, int32_t index);
static ret_t lresolver_put_name(lresolver_t* resolver, const char* name, bool_t value);

static ret_t lresolver_error(lresolver_t* resolver, const char* fmt, const char* arg) {
  return_value_if_fail(resolver != NULL && fmt != NULL, RET_BAD_PARAMS);

  resolver->error++;
  log_debug(fmt, arg);

  return RET_OK;
}

static const char* lresolver_to_str(lresolver_t* resolver, sized_str_t* name) {
  return lstrncpy(resolver->tname, sizeof(resolver->tname), name->str, name->size);
}

static ret_t lexpr_binary_resolve(lexpr_t* expr, lresolver_t* resolver) {
  lexpr_resolve(lexpr_get_arg(expr, 0), resolver);
  lexpr_resolve(lexpr_get_arg(expr, 1), resolver);

  return RET_OK;
}

static ret_t lexpr_unary_resolve(lexpr_t* expr, lresolver_t* resolver) {
  lexpr_resolve(lexpr_get_arg(expr, 0), resolver);

  return RET_OK;
}

static ret_t lexpr_call_resolve(lexpr_t* expr, lresolver_t* resolver) {
  uint32_t i = 0;
  for (i = 0; i < expr->data.args->size; i++) {
    lexpr_t* iter = lexpr_get_arg(expr, i);
    lexpr_resolve(iter, resolver);
  }

  return RET_OK;
}

static ret_t lresolver_resolve_local(lresolver_t* resolver, lexpr_t* expr, const char* str,
                                     int32_t size) {
  lvalue_t v;
  uint32_t i = 0;
  uint32_t nr = resolver->scopes->size;
  sized_str_t s = {size >= 0 ? size : strlen(str), str};
  const char* name = lresolver_to_str(resolver, &s);

  for (i = 0; i < nr; i++) {
    lmap_t* scope = lresolver_get_scope(resolver, i);
    if (lmap_get(scope, name, &v) == RET_OK) {
      resolver->on_result(resolver->on_result_ctx, expr, name, i);
      return RET_OK;
    }
  }
  /*external var*/
  resolver->on_result(resolver->on_result_ctx, expr, name, -1);

  return RET_FAIL;
}

static ret_t lexpr_resolve(lexpr_t* expr, lresolver_t* resolver) {
  return_value_if_fail(expr != NULL && resolver != NULL, RET_BAD_PARAMS);
  if (expr->has_value) {
    if (expr->type == LTOKEN_IDENTIFIER) {
      const sized_str_t* str = lvalue_id(lexpr_get_value(expr));
      lresolver_resolve_local(resolver, expr, str->str, str->size);
    }
  } else if (expr->has_args) {
    switch (expr->type) {
      case LTOKEN_GROUP: {
        lexpr_resolve(lexpr_get_arg(expr, 0), resolver);
        break;
      }
      case LTOKEN_PLUS:
      case LTOKEN_SUB:
      case LTOKEN_EQUAL:
      case LTOKEN_GET:
      case LTOKEN_STAR:
      case LTOKEN_SLASH:
      case LTOKEN_PERCENT:
      case LTOKEN_NOT_EQUAL:
      case LTOKEN_EQUAL_EQUAL:
      case LTOKEN_GREATER:
      case LTOKEN_GREATER_GREATER:
      case LTOKEN_GREATER_EQUAL:
      case LTOKEN_LESS:
      case LTOKEN_LESS_LESS:
      case LTOKEN_LESS_EQUAL:
      case LTOKEN_AND:
      case LTOKEN_OR:
      case LTOKEN_BIT_AND:
      case LTOKEN_BIT_OR:
      case LTOKEN_BIT_XOR: {
        lexpr_binary_resolve(expr, resolver);
        break;
      }
      case LTOKEN_SET: {
        lexpr_resolve(lexpr_get_arg(expr, 0), resolver);
        lexpr_resolve(lexpr_get_arg(expr, 2), resolver);
        break;
      }
      case LTOKEN_MINUS:
      case LTOKEN_NOT:
      case LTOKEN_BIT_NOT: {
        lexpr_unary_resolve(expr, resolver);
        break;
      }
      case LTOKEN_CALL_FUNC:
      case LTOKEN_CALL_CONSTRUCTOR: {
        lexpr_call_resolve(expr, resolver);
        break;
      }
      default:
        break;
    }
  } else if (expr->type == LTOKEN_THIS) {
    lresolver_resolve_local(resolver, expr, LSTR_TOKEN_THIS, -1);
  } else if (expr->type == LTOKEN_SUPER) {
    if (resolver->current_class == NULL) {
      lresolver_error(resolver, "%s", "not in class\n");
    } else if (resolver->current_class->data.stmt_class.super == NULL) {
      lresolver_error(resolver, "%s", "class has no super\n");
    } else {
      lresolver_resolve_local(resolver, expr, LSTR_TOKEN_SUPER, -1);
    }
  }

  return RET_OK;
}

#if 1
static ret_t lstmts_resolve(lstmts_t* stmts, lresolver_t* resolver) {
  uint16_t i = 0;
  if (stmts != NULL) {
    for (i = 0; i < stmts->size; i++) {
      lstmt_resolve(stmts->elms[i], resolver);
    }
  }
  return RET_OK;
}

static ret_t lstmt_resolve(lstmt_t* stmt, lresolver_t* resolver) {
  uint16_t i = 0;
  return_value_if_fail(stmt != NULL && resolver != NULL, RET_BAD_PARAMS);

  switch (stmt->type) {
    case LTOKEN_BLOCK: {
      lresolver_begin_scope(resolver);
      lstmts_resolve(stmt->data.stmt_block.stmts, resolver);
      lresolver_end_scope(resolver);
      break;
    }
    case LTOKEN_EXPR: {
      lexpr_resolve(stmt->data.stmt_expr.expr, resolver);
      break;
    }
    case LTOKEN_BREAK: {
      if (resolver->in_loop == 0) {
        lresolver_error(resolver, "%s", "break must in loop");
      }
      break;
    }
    case LTOKEN_CONTINUE: {
      if (resolver->in_loop == 0) {
        lresolver_error(resolver, "%s", "continue must in loop");
      }
      break;
    }
    case LTOKEN_RETURN: {
      if (resolver->current_func == NULL) {
        lresolver_error(resolver, "%s", "return must in function");
      }
      lexpr_resolve(stmt->data.stmt_return.expr, resolver);
      break;
    }
    case LTOKEN_VAR: {
      lresolver_declare(resolver, &(stmt->data.stmt_var.name));
      lexpr_resolve(stmt->data.stmt_var.initializer, resolver);
      lresolver_define(resolver, &(stmt->data.stmt_var.name));
      break;
    }
    case LTOKEN_IF: {
      lexpr_resolve(stmt->data.stmt_if.condition, resolver);
      lstmt_resolve(stmt->data.stmt_if.stmt_then, resolver);
      if (stmt->data.stmt_if.stmt_else != NULL) {
        lstmt_resolve(stmt->data.stmt_if.stmt_else, resolver);
      }
      break;
    }
    case LTOKEN_WHILE: {
      resolver->in_loop++;
      lexpr_resolve(stmt->data.stmt_while.condition, resolver);
      lstmt_resolve(stmt->data.stmt_while.stmt_body, resolver);
      resolver->in_loop--;
      break;
    }
    case LTOKEN_FOR: {
      resolver->in_loop++;
      if (stmt->data.stmt_for.initializer != NULL) {
        lstmt_resolve(stmt->data.stmt_for.initializer, resolver);
      }
      if (stmt->data.stmt_for.condition != NULL) {
        lexpr_resolve(stmt->data.stmt_for.condition, resolver);
      }
      if (stmt->data.stmt_for.increment != NULL) {
        lexpr_resolve(stmt->data.stmt_for.increment, resolver);
      }
      lstmt_resolve(stmt->data.stmt_for.stmt_body, resolver);
      resolver->in_loop--;
      break;
    }
    case LTOKEN_METHOD:
    case LTOKEN_FUN: {
      lstmt_t* enclosing_func = resolver->current_func;
      lparams_t* params = stmt->data.stmt_func.params;
      sized_str_t* name = &(stmt->data.stmt_func.name);

      lresolver_declare(resolver, name);
      lresolver_define(resolver, name);
      resolver->current_func = stmt;

      lresolver_begin_scope(resolver);
      for (i = 0; i < params->size; i++) {
        lparam_t* iter = params->elms + i;
        lresolver_declare(resolver, iter);
        lresolver_define(resolver, iter);
      }
      lstmt_resolve(stmt->data.stmt_func.stmt_body, resolver);
      lresolver_end_scope(resolver);

      resolver->current_func = enclosing_func;
      break;
    }
    case LTOKEN_CLASS: {
      lstmt_t* enclosing_class = resolver->current_class;
      sized_str_t* name = &(stmt->data.stmt_class.name);
      lexpr_t* super = stmt->data.stmt_class.super;
      const sized_str_t* super_name = super != NULL ? lvalue_id(lexpr_get_value(super)) : NULL;

      if (super != NULL && super_name->size == name->size &&
          lstrncmp(super_name->str, name->str, name->size) == 0) {
        lresolver_error(resolver, "%s", "can not inherit self\n");
      }

      lresolver_declare(resolver, name);
      lresolver_define(resolver, name);
      resolver->current_class = stmt;

      if (super != NULL) {
        lexpr_resolve(super, resolver);
        lresolver_begin_scope(resolver);
        lresolver_put_name(resolver, "super", TRUE);
      }

      lresolver_begin_scope(resolver);
      lresolver_put_name(resolver, "this", TRUE);

      lstmts_resolve(stmt->data.stmt_class.funcs, resolver);
      lresolver_end_scope(resolver);

      if (super != NULL) {
        lresolver_end_scope(resolver);
      }
      resolver->current_class = enclosing_class;
    }
    default:
      break;
  }

  return RET_OK;
}

#endif

lresolver_t* lresolver_create(lresolver_on_result_t on_result, void* on_result_ctx) {
  lresolver_t* resolver = NULL;
  return_value_if_fail(on_result != NULL, NULL);
  resolver = LMEM_NEW(lresolver_t);
  return_value_if_fail(resolver != NULL, NULL);

  if (lresolver_init(resolver) != NULL) {
    resolver->on_result = on_result;
    resolver->on_result_ctx = on_result_ctx;
    return resolver;
  } else {
    LMEM_FREE(resolver);
    return NULL;
  }
}

lresolver_t* lresolver_init(lresolver_t* resolver) {
  return_value_if_fail(resolver != NULL, NULL);

  resolver->scopes = larray_create(20);

  return resolver;
}

ret_t lresolver_begin_scope(lresolver_t* resolver) {
  lvalue_t v;
  lmap_t* scope = lmap_create();
  return_value_if_fail(scope != NULL, RET_BAD_PARAMS);
  return_value_if_fail(resolver != NULL, RET_BAD_PARAMS);

  lvalue_set_object(&v, LOBJECT(scope));
  if (larray_push(resolver->scopes, &v) != RET_OK) {
    LOBJECT_UNREF(scope);
    return RET_OOM;
  } else {
    LOBJECT_UNREF(scope);
    return RET_OK;
  }
}

ret_t lresolver_end_scope(lresolver_t* resolver) {
  lvalue_t v;
  return_value_if_fail(resolver != NULL, RET_BAD_PARAMS);
  larray_pop(resolver->scopes, &v);
  lvalue_reset(&v);

  return RET_OK;
}

ret_t lresolver_declare(lresolver_t* resolver, sized_str_t* name) {
  lvalue_t v;
  return_value_if_fail(resolver != NULL && name != NULL, RET_BAD_PARAMS);
  if (lresolver_get(resolver, name, 0, &v) == RET_OK) {
    lresolver_error(resolver, "%s is duplicated\n", lresolver_to_str(resolver, name));
  }

  return lresolver_put(resolver, name, FALSE);
}

ret_t lresolver_define(lresolver_t* resolver, sized_str_t* name) {
  return_value_if_fail(resolver != NULL && name != NULL, RET_BAD_PARAMS);

  return lresolver_put(resolver, name, TRUE);
}

static lmap_t* lresolver_get_scope(lresolver_t* resolver, int32_t index) {
  lvalue_t v;
  lmap_t* scope = NULL;
  return_value_if_fail(resolver != NULL, NULL);
  index = resolver->scopes->size - index - 1;
  return_value_if_fail(index >= 0, NULL);

  return_value_if_fail(larray_get(resolver->scopes, index, &v) == RET_OK, NULL);
  scope = LMAP(lvalue_object(&v));

  return scope;
}

static ret_t lresolver_put_name(lresolver_t* resolver, const char* name, bool_t value) {
  lmap_t* scope = lresolver_get_scope(resolver, 0);
  return_value_if_fail(scope != NULL, RET_BAD_PARAMS);

  return lmap_set_bool(scope, name, value);
}

ret_t lresolver_put(lresolver_t* resolver, sized_str_t* name, bool_t value) {
  lmap_t* scope = lresolver_get_scope(resolver, 0);
  return_value_if_fail(scope != NULL, RET_BAD_PARAMS);

  return lmap_set_bool(scope, lresolver_to_str(resolver, name), value);
}

ret_t lresolver_get(lresolver_t* resolver, sized_str_t* name, uint16_t scope_index,
                    lvalue_t* value) {
  lmap_t* scope = lresolver_get_scope(resolver, scope_index);
  return_value_if_fail(scope != NULL, RET_BAD_PARAMS);

  return lmap_get(scope, lresolver_to_str(resolver, name), value);
}

ret_t lresolver_resolve(lresolver_t* resolver, lstmt_t* stmt) {
  return lstmt_resolve(stmt, resolver);
}

ret_t lresolver_deinit(lresolver_t* resolver) {
  return_value_if_fail(resolver != NULL, RET_BAD_PARAMS);
  LOBJECT_UNREF(resolver->scopes);

  return RET_OK;
}

ret_t lresolver_destroy(lresolver_t* resolver) {
  return_value_if_fail(resolver != NULL, RET_BAD_PARAMS);
  lresolver_deinit(resolver);
  LMEM_FREE(resolver);

  return RET_OK;
}
