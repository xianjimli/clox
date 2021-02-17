#include "parser/lexpr_to_string.h"
#include "parser/lstmt_to_string.h"

ret_t lstmts_to_string(lstmts_t* stmts, lstr_t* str) {
  uint16_t i = 0;
  lstr_append(str, "{\n", -1);
  if (stmts != NULL) {
    for (i = 0; i < stmts->size; i++) {
      lstmt_to_string(stmts->elms[i], str);
      lstr_append(str, "\n", 1);
    }
  }
  lstr_append(str, "}\n", -1);
  return RET_OK;
}

static ret_t lstmt_func_to_string(lstmt_t* stmt, lstr_t* str) {
  uint32_t i = 0;
  lparams_t* params = stmt->data.stmt_func.params;
  lstr_append(str, stmt->data.stmt_func.name.str, stmt->data.stmt_func.name.size);
  lstr_append(str, "(", 1);
  for (i = 0; i < params->size; i++) {
    lparam_t* iter = params->elms + i;
    if (i > 0) {
      lstr_append(str, ",", 1);
    }
    lstr_append(str, iter->str, iter->size);
  }
  lstr_append(str, ")", 1);
  lstmt_to_string(stmt->data.stmt_func.stmt_body, str);
  return RET_OK;
}

ret_t lstmt_to_string(lstmt_t* stmt, lstr_t* str) {
  return_value_if_fail(stmt != NULL && str != NULL, RET_BAD_PARAMS);

  switch (stmt->type) {
    case LTOKEN_BLOCK: {
      lstmts_to_string(stmt->data.stmt_block.stmts, str);
      break;
    }
    case LTOKEN_EXPR: {
      lexpr_to_string(stmt->data.stmt_expr.expr, str);
      break;
    }
    case LTOKEN_BREAK: {
      lstr_append(str, "break", -1);
      break;
    }
    case LTOKEN_CONTINUE: {
      lstr_append(str, "continue", -1);
      break;
    }
    case LTOKEN_RETURN: {
      lstr_append(str, "return ", -1);
      lexpr_to_string(stmt->data.stmt_return.expr, str);
      break;
    }
    case LTOKEN_VAR: {
      lstr_append(str, "var ", -1);
      lstr_append(str, stmt->data.stmt_var.name.str, stmt->data.stmt_var.name.size);
      lstr_append(str, "=", -1);
      lexpr_to_string(stmt->data.stmt_var.initializer, str);
      break;
    }
    case LTOKEN_IF: {
      lstr_append(str, "if(", -1);
      lexpr_to_string(stmt->data.stmt_if.condition, str);
      lstr_append(str, ") ", -1);
      lstmt_to_string(stmt->data.stmt_if.stmt_then, str);
      if (stmt->data.stmt_if.stmt_else != NULL) {
        lstr_append(str, " else ", -1);
        lstmt_to_string(stmt->data.stmt_if.stmt_else, str);
      }
      break;
    }
    case LTOKEN_WHILE: {
      lstr_append(str, "while(", -1);
      lexpr_to_string(stmt->data.stmt_while.condition, str);
      lstr_append(str, ") ", -1);
      lstmt_to_string(stmt->data.stmt_while.stmt_body, str);
      break;
    }
    case LTOKEN_FOR: {
      lstr_append(str, "for(", -1);
      if (stmt->data.stmt_for.initializer != NULL) {
        lstmt_to_string(stmt->data.stmt_for.initializer, str);
      }
      lstr_append(str, ";", -1);
      if (stmt->data.stmt_for.condition != NULL) {
        lexpr_to_string(stmt->data.stmt_for.condition, str);
      }
      lstr_append(str, ";", -1);
      if (stmt->data.stmt_for.increment != NULL) {
        lexpr_to_string(stmt->data.stmt_for.increment, str);
      }
      lstr_append(str, ") ", -1);
      lstmt_to_string(stmt->data.stmt_for.stmt_body, str);
      break;
    }
    case LTOKEN_METHOD: {
      lstmt_func_to_string(stmt, str);
      break;
    }
    case LTOKEN_FUN: {
      lstr_append(str, "fun ", -1);
      lstmt_func_to_string(stmt, str);
      break;
    }
    case LTOKEN_CLASS: {
      lstr_append(str, "class ", -1);
      lstr_append(str, stmt->data.stmt_class.name.str, stmt->data.stmt_class.name.size);
      if (stmt->data.stmt_class.super != NULL) {
        lstr_append(str, "<", 1);
        lexpr_to_string(stmt->data.stmt_class.super, str);
      }
      lstmts_to_string(stmt->data.stmt_class.funcs, str);
    }
    default:
      break;
  }

  return RET_OK;
}
