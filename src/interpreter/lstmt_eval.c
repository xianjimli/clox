#include "interpreter/lexpr_eval.h"
#include "interpreter/lstmt_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lcallable_func.h"
#include "interpreter/lcallable_class.h"

ret_t lstmts_eval(lstmts_t* stmts, linterpreter_t* interpreter, lvalue_t* result) {
  uint16_t i = 0;
  if (stmts != NULL) {
    for (i = 0; i < stmts->size; i++) {
      lstmt_eval(stmts->elms[i], interpreter, result);
    }
  }
  return RET_OK;
}

#if 0
static ret_t lstmt_func_eval(lstmt_t* stmt, linterpreter_t* interpreter, lvalue_t* result) {
  uint32_t i = 0;
  lparams_t* params = stmt->data.stmt_func.params;
  lstr_append(interpreter, stmt->data.stmt_func.name.interpreter, stmt->data.stmt_func.name.size);
  lstr_append(interpreter, "(", 1);
  for (i = 0; i < params->size; i++) {
    lparam_t* iter = params->elms + i;
    if (i > 0) {
      lstr_append(interpreter, ",", 1);
    }
    lstr_append(interpreter, iter->interpreter, iter->size);
  }
  lstr_append(interpreter, ")", 1);
  lstmt_eval(stmt->data.stmt_func.stmt_body, interpreter, result);
  return RET_OK;
}
#endif

ret_t lstmt_eval(lstmt_t* stmt, linterpreter_t* interpreter, lvalue_t* result) {
  lvalue_set_bool(result, FALSE);
  return_value_if_fail(stmt != NULL && interpreter != NULL, RET_BAD_PARAMS);
  if (interpreter->meet_break || interpreter->meet_continue || interpreter->meet_return) {
    return RET_OK;
  }

  switch (stmt->type) {
    case LTOKEN_BLOCK: {
      linterpreter_enter_block(interpreter);
      lstmts_eval(stmt->data.stmt_block.stmts, interpreter, result);
      linterpreter_leave_block(interpreter);
      break;
    }
    case LTOKEN_EXPR: {
      lexpr_eval(stmt->data.stmt_expr.expr, interpreter, result);
      break;
    }
    case LTOKEN_BREAK: {
      interpreter->meet_break = TRUE;
      break;
    }
    case LTOKEN_CONTINUE: {
      interpreter->meet_continue = TRUE;
      break;
    }
    case LTOKEN_RETURN: {
      lexpr_eval(stmt->data.stmt_return.expr, interpreter, result);
      interpreter->meet_return = TRUE;
      break;
    }
    case LTOKEN_VAR: {
      const sized_str_t* name = &(stmt->data.stmt_var.name);
      lexpr_eval(stmt->data.stmt_var.initializer, interpreter, result);
      linterpreter_define_var(interpreter, name, result);
      lvalue_reset(result);
      break;
    }
    case LTOKEN_IF: {
      lvalue_t condition;
      lexpr_eval(stmt->data.stmt_if.condition, interpreter, &condition);

      if (lvalue_to_bool(&condition)) {
        lstmt_eval(stmt->data.stmt_if.stmt_then, interpreter, result);
      } else if (stmt->data.stmt_if.stmt_else != NULL) {
        lstmt_eval(stmt->data.stmt_if.stmt_else, interpreter, result);
      }
      break;
    }
    case LTOKEN_WHILE: {
      lvalue_t condition;
      lexpr_eval(stmt->data.stmt_while.condition, interpreter, &condition);

      while (lvalue_to_bool(&condition)) {
        interpreter->meet_continue = FALSE;
        lstmt_eval(stmt->data.stmt_while.stmt_body, interpreter, result);
        if (interpreter->meet_return || interpreter->meet_break) {
          break;
        }
        lexpr_eval(stmt->data.stmt_while.condition, interpreter, &condition);
      }

      interpreter->meet_break = FALSE;
      interpreter->meet_continue = FALSE;
      break;
    }
    case LTOKEN_FOR: {
      for (lstmt_eval(stmt->data.stmt_for.initializer, interpreter, result);
           lexpr_eval(stmt->data.stmt_for.condition, interpreter, result) == RET_OK &&
           lvalue_to_bool(result);
           lexpr_eval(stmt->data.stmt_for.increment, interpreter, result)) {
        interpreter->meet_continue = FALSE;
        lstmt_eval(stmt->data.stmt_for.stmt_body, interpreter, result);
        if (interpreter->meet_return || interpreter->meet_break) {
          break;
        }
      }
      interpreter->meet_break = FALSE;
      interpreter->meet_continue = FALSE;
      break;
    }
    case LTOKEN_FUN: {
      lvalue_t v;
      const sized_str_t* name = &(stmt->data.stmt_func.name);
      lcallable_t* callable = lcallable_func_create(NULL, stmt);
      return_value_if_fail(callable != NULL, RET_OOM);
      linterpreter_define_var(interpreter, name, lvalue_set_object(&v, LOBJECT(callable)));
      LOBJECT_UNREF(callable);
      break;
    }
    case LTOKEN_CLASS: {
      lvalue_t v;
      const sized_str_t* name = &(stmt->data.stmt_class.name);
      lcallable_t* callable = lcallable_clazz_create(stmt);
      return_value_if_fail(callable != NULL, RET_OOM);
      linterpreter_define_var(interpreter, name, lvalue_set_object(&v, LOBJECT(callable)));
      LOBJECT_UNREF(callable);
      break;
#if 0
      lstr_append(interpreter, stmt->data.stmt_class.name.interpreter, stmt->data.stmt_class.name.size);
      if (stmt->data.stmt_class.super != NULL) {
        lstr_append(interpreter, "<", 1);
        lexpr_eval(stmt->data.stmt_class.super, interpreter, result);
      }
      lstmts_eval(stmt->data.stmt_class.funcs, interpreter, result);
    }
#endif
    }
    default:
      break;
  }

  return RET_OK;
}
