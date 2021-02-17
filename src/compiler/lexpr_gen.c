#include "parser/lvalue_ops.h"
#include "compiler/lexpr_gen.h"
#include "compiler/lcompiler.h"

static ret_t lexpr_call_gen(lexpr_t* expr, lcompiler_t* compiler) {
  uint32_t i = 0;
  uint32_t nr = expr->data.args->size - 1;
  lexpr_t* callee = lexpr_get_arg(expr, 0);
  lvalue_t* args = LMEM_NEW_ARRAY(lvalue_t, expr->data.args->size);
  goto_error_if_fail(args != NULL);

  memset(args, 0x00, sizeof(lvalue_t) * nr);
  for (i = 0; i < nr; i++) {
    lexpr_t* iter = lexpr_get_arg(expr, i + 1);
    lexpr_gen(iter, compiler);
  }

  for (i = 0; i < nr; i++) {
    lvalue_reset(args + i);
  }
  LMEM_FREE(args);

  return RET_OK;
error:
  LMEM_FREE(args);
  return RET_OK;
}

ret_t lexpr_gen(lexpr_t* expr, lcompiler_t* compiler) {
  return_value_if_fail(expr != NULL && compiler != NULL, RET_BAD_PARAMS);

  if (expr->has_value) {
    lvalue_t* v = NULL;
    if (expr->type == LTOKEN_NUMBER) {
      v = lexpr_get_value(expr);
      lobject_func_emit_const(compiler->func, v);
    } else if (expr->type == LTOKEN_STRING) {
      v = lexpr_get_value(expr);
      lobject_func_emit_const(compiler->func, v);
    } else if (expr->type == LTOKEN_IDENTIFIER) {
    }
    return RET_OK;
  } else if (expr->has_args) {
    lvalue_t left;
    lvalue_t right;
    ret_t ret = RET_OK;
    switch (expr->type) {
      case LTOKEN_GET: {
        lvalue_t name;
        lobject_t* obj = NULL;
        const sized_str_t* prop = lexpr_get_value_id(lexpr_get_arg(expr, 1));

        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        obj = lvalue_object(&name);
        return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

        break;
      }
      case LTOKEN_SET: {
        lvalue_t name;
        lvalue_t value;
        lobject_t* obj = NULL;
        const sized_str_t* prop = lexpr_get_value_id(lexpr_get_arg(expr, 1));

        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        obj = lvalue_object(&name);
        return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
        lexpr_gen(lexpr_get_arg(expr, 2), compiler);

        break;
      }
      case LTOKEN_GROUP: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        break;
      }
      case LTOKEN_PLUS:
      case LTOKEN_PLUS_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        lobject_func_emit_byte(compiler->func, OP_ADD);
        break;
      }
      case LTOKEN_SUB:
      case LTOKEN_MINUS_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_EQUAL: {
        lexpr_t* left = lexpr_get_arg(expr, 0);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_STAR:
      case LTOKEN_STAR_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_SLASH:
      case LTOKEN_SLASH_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_PERCENT: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_NOT_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_EQUAL_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_GREATER: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_GREATER_GREATER: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_GREATER_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_LESS: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_LESS_LESS: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_LESS_EQUAL: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_AND: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_OR: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_BIT_AND: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_BIT_OR: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_BIT_XOR: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        lexpr_gen(lexpr_get_arg(expr, 1), compiler);
        break;
      }
      case LTOKEN_MINUS: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        break;
      }
      case LTOKEN_NOT: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        break;
      }
      case LTOKEN_BIT_NOT: {
        lexpr_gen(lexpr_get_arg(expr, 0), compiler);
        break;
      }
      case LTOKEN_CALL_FUNC:
      case LTOKEN_CALL_CONSTRUCTOR: {
        lexpr_call_gen(expr, compiler);
        break;
      }
      default:
        break;
    }
    return ret;
  } else if (expr->type == LTOKEN_THIS) {
    return RET_OK;
  } else if (expr->type == LTOKEN_SUPER) {
    return RET_OK;
  }

  return RET_OK;
}
