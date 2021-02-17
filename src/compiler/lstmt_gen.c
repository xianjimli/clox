#include "compiler/lexpr_gen.h"
#include "compiler/lstmt_gen.h"
#include "compiler/lcompiler.h"
#include "compiler/lobject_func.h"

ret_t lstmts_gen(lstmts_t* stmts, lcompiler_t* compiler) {
  uint16_t i = 0;
  if (stmts != NULL) {
    for (i = 0; i < stmts->size; i++) {
      lstmt_gen(stmts->elms[i], compiler);
    }
  }
  return RET_OK;
}

ret_t lstmt_gen(lstmt_t* stmt, lcompiler_t* compiler) {
  return_value_if_fail(stmt != NULL && compiler != NULL, RET_BAD_PARAMS);

  switch (stmt->type) {
    case LTOKEN_BLOCK: {
      lstmts_gen(stmt->data.stmt_block.stmts, compiler);
      break;
    }
    case LTOKEN_EXPR: {
      lexpr_gen(stmt->data.stmt_expr.expr, compiler);
      lobject_func_emit_byte(compiler->func, OP_POP);
      break;
    }
    case LTOKEN_BREAK: {
      break;
    }
    case LTOKEN_CONTINUE: {
      break;
    }
    case LTOKEN_RETURN: {
      break;
    }
    case LTOKEN_VAR: {
      break;
    }
    case LTOKEN_IF: {
      break;
    }
    case LTOKEN_WHILE: {
      break;
    }
    case LTOKEN_FOR: {
      break;
    }
    case LTOKEN_FUN: {
      break;
    }
    case LTOKEN_CLASS: {
      break;
    }
    default:
      break;
  }

  return RET_OK;
}
