#include "parser/lstmt.h"

lstmt_t* lstmt_create(ltoken_type_t type) {
  lstmt_t* stmt = LMEM_NEW(lstmt_t);
  return_value_if_fail(stmt != NULL, NULL);

  stmt->type = type;

  return stmt;
}

lstmt_t* lstmt_create_block(lstmts_t* stmts) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(stmts != NULL, NULL);
  stmt = lstmt_create(LTOKEN_BLOCK);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_block.stmts = stmts;

  return stmt;
}

lstmt_t* lstmt_create_expr(lexpr_t* expr) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(expr != NULL, NULL);
  stmt = lstmt_create(LTOKEN_EXPR);
  return_value_if_fail(expr != NULL, NULL);
  stmt->data.stmt_expr.expr = expr;

  return stmt;
}

lstmt_t* lstmt_create_return(lexpr_t* expr) {
  lstmt_t* stmt = NULL;
  stmt = lstmt_create(LTOKEN_RETURN);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_return.expr = expr;

  return stmt;
}

lstmt_t* lstmt_create_continue(void) {
  return lstmt_create(LTOKEN_CONTINUE);
}

lstmt_t* lstmt_create_break(void) {
  return lstmt_create(LTOKEN_BREAK);
}

lstmt_t* lstmt_create_if(lexpr_t* condition, lstmt_t* stmt_then, lstmt_t* stmt_else) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(condition != NULL && stmt_then != NULL, NULL);
  stmt = lstmt_create(LTOKEN_IF);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_if.condition = condition;
  stmt->data.stmt_if.stmt_then = stmt_then;
  stmt->data.stmt_if.stmt_else = stmt_else;

  return stmt;
}

lstmt_t* lstmt_create_while(lexpr_t* condition, lstmt_t* stmt_body) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(condition != NULL && stmt_body != NULL, NULL);
  stmt = lstmt_create(LTOKEN_WHILE);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_while.condition = condition;
  stmt->data.stmt_while.stmt_body = stmt_body;

  return stmt;
}

lstmt_t* lstmt_create_for(lstmt_t* initializer, lexpr_t* condition, lexpr_t* increment,
                          lstmt_t* stmt_body) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(stmt_body != NULL, NULL);
  stmt = lstmt_create(LTOKEN_FOR);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_for.initializer = initializer;
  stmt->data.stmt_for.condition = condition;
  stmt->data.stmt_for.increment = increment;
  stmt->data.stmt_for.stmt_body = stmt_body;

  return stmt;
}

lstmt_t* lstmt_create_var(ltoken_t* name, lexpr_t* initializer) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(name != NULL && initializer != NULL, NULL);
  stmt = lstmt_create(LTOKEN_VAR);
  return_value_if_fail(stmt != NULL, NULL);
  stmt->data.stmt_var.name.str = name->lexeme;
  stmt->data.stmt_var.name.size = name->lexeme_len;
  stmt->data.stmt_var.initializer = initializer;

  return stmt;
}

lstmt_t* lstmt_create_func(ltoken_t* name, lparams_t* params, lstmt_t* stmt_body) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(name != NULL && params != NULL && stmt_body != NULL, NULL);
  stmt = lstmt_create(LTOKEN_FUN);
  return_value_if_fail(stmt != NULL, NULL);

  stmt->data.stmt_func.name.str = name->lexeme;
  stmt->data.stmt_func.name.size = name->lexeme_len;
  stmt->data.stmt_func.params = params;
  stmt->data.stmt_func.stmt_body = stmt_body;

  return stmt;
}

lstmt_t* lstmt_create_class(ltoken_t* name, ltoken_t* super_name, lstmts_t* funcs) {
  lstmt_t* stmt = NULL;
  return_value_if_fail(name != NULL && funcs != NULL, NULL);
  stmt = lstmt_create(LTOKEN_CLASS);
  return_value_if_fail(stmt != NULL, NULL);

  stmt->data.stmt_class.name.str = name->lexeme;
  stmt->data.stmt_class.name.size = name->lexeme_len;
  if (super_name != NULL) {
    stmt->data.stmt_class.super = lexpr_create_id(super_name);
  }
  stmt->data.stmt_class.funcs = funcs;

  return stmt;
}

ret_t lstmt_destroy(lstmt_t* stmt) {
  return_value_if_fail(stmt != NULL, RET_BAD_PARAMS);
  switch (stmt->type) {
    case LTOKEN_BLOCK: {
      lstmts_destroy(stmt->data.stmt_block.stmts);
      break;
    }
    case LTOKEN_EXPR: {
      lexpr_destroy(stmt->data.stmt_expr.expr);
      break;
    }
    case LTOKEN_RETURN: {
      lexpr_destroy(stmt->data.stmt_return.expr);
      break;
    }
    case LTOKEN_VAR: {
      lexpr_destroy(stmt->data.stmt_var.initializer);
      break;
    }
    case LTOKEN_IF: {
      lexpr_destroy(stmt->data.stmt_if.condition);
      lstmt_destroy(stmt->data.stmt_if.stmt_then);
      lstmt_destroy(stmt->data.stmt_if.stmt_else);
      break;
    }
    case LTOKEN_WHILE: {
      lexpr_destroy(stmt->data.stmt_while.condition);
      lstmt_destroy(stmt->data.stmt_while.stmt_body);
      break;
    }
    case LTOKEN_FOR: {
      lstmt_destroy(stmt->data.stmt_for.initializer);
      lexpr_destroy(stmt->data.stmt_for.condition);
      lexpr_destroy(stmt->data.stmt_for.increment);
      lstmt_destroy(stmt->data.stmt_for.stmt_body);
      break;
    }
    case LTOKEN_FUN:
    case LTOKEN_METHOD: {
      lstmt_destroy(stmt->data.stmt_func.stmt_body);
      lparams_destroy(stmt->data.stmt_func.params);
      break;
    }
    case LTOKEN_CLASS: {
      lexpr_destroy(stmt->data.stmt_class.super);
      lstmts_destroy(stmt->data.stmt_class.funcs);
      break;
    }
    default:
      break;
  }
  LMEM_FREE(stmt);

  return RET_OK;
}

lstmt_t* lstmt_find_func(lstmt_t* clazz, const char* name) {
  uint32_t i = 0;
  lstmts_t* funcs = NULL;
  lstmt_func_t* func = NULL;
  return_value_if_fail(clazz != NULL && name != NULL, NULL);
  return_value_if_fail(clazz->data.stmt_class.funcs != NULL, NULL);
  funcs = clazz->data.stmt_class.funcs;

  for (i = 0; i < funcs->size; i++) {
    lstmt_t* iter = funcs->elms[i];
    assert(iter->type == LTOKEN_METHOD);
    func = &(iter->data.stmt_func);
    if (lstrncmp(name, func->name.str, func->name.size) == 0) {
      return iter;
    }
  }

  return NULL;
}
