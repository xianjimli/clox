#include "parser/lvalue_ops.h"
#include "interpreter/lexpr_eval.h"
#include "interpreter/linterpreter.h"
#include "interpreter/lcallable.h"

static ret_t lexpr_call_eval(lexpr_t* expr, linterpreter_t* interpreter, lvalue_t* result) {
  lvalue_t v;
  uint32_t i = 0;
  lcallable_t* callable = NULL;
  uint32_t nr = expr->data.args->size - 1;
  lexpr_t* callee = lexpr_get_arg(expr, 0);
  lvalue_t* args = LMEM_NEW_ARRAY(lvalue_t, expr->data.args->size);
  goto_error_if_fail(args != NULL);
  goto_error_if_fail(lexpr_eval(callee, interpreter, &v) == RET_OK) callable =
      LCALLABLE(lvalue_object(&v));
  goto_error_if_fail(callable != NULL);

  memset(args, 0x00, sizeof(lvalue_t) * nr);
  for (i = 0; i < nr; i++) {
    lexpr_t* iter = lexpr_get_arg(expr, i + 1);
    lexpr_eval(iter, interpreter, args + i);
  }

  lcallable_call(callable, interpreter, args, nr, result);

  for (i = 0; i < nr; i++) {
    lvalue_reset(args + i);
  }
  LMEM_FREE(args);

  return RET_OK;
error:
  LMEM_FREE(args);
  return RET_OK;
}

ret_t lexpr_eval(lexpr_t* expr, linterpreter_t* interpreter, lvalue_t* result) {
  lvalue_set_bool(result, FALSE);
  return_value_if_fail(expr != NULL && interpreter != NULL, RET_BAD_PARAMS);

  if (expr->has_value) {
    if (expr->type == LTOKEN_NUMBER || expr->type == LTOKEN_STRING) {
      lvalue_copy(result, lexpr_get_value(expr));

      return RET_OK;
    } else if (expr->type == LTOKEN_IDENTIFIER) {
      return linterpreter_get_var(interpreter, expr, result);
    }
  } else if (expr->has_args) {
    lvalue_t left;
    lvalue_t right;
    ret_t ret = RET_OK;
    switch (expr->type) {
      case LTOKEN_GET: {
        lvalue_t name;
        lobject_t* obj = NULL;
        const sized_str_t* prop = lexpr_get_value_id(lexpr_get_arg(expr, 1));

        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &name);
        obj = lvalue_object(&name);
        return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
        ret = lobject_get(obj, linterpreter_to_name(interpreter, prop), result);

        break;
      }
      case LTOKEN_SET: {
        lvalue_t name;
        lvalue_t value;
        lobject_t* obj = NULL;
        const sized_str_t* prop = lexpr_get_value_id(lexpr_get_arg(expr, 1));

        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &name);
        obj = lvalue_object(&name);
        return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
        lexpr_eval(lexpr_get_arg(expr, 2), interpreter, &value);

        ret = lobject_set(obj, linterpreter_to_name(interpreter, prop), &value);
        break;
      }
      case LTOKEN_GROUP: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, result);
        break;
      }
      case LTOKEN_PLUS:
      case LTOKEN_PLUS_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_add(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_SUB:
      case LTOKEN_MINUS_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_sub(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_EQUAL: {
        lexpr_t* left = lexpr_get_arg(expr, 0);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, result);
        linterpreter_set_var(interpreter, left, result);
        break;
      }
      case LTOKEN_STAR:
      case LTOKEN_STAR_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_mul(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_SLASH:
      case LTOKEN_SLASH_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_div(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_PERCENT: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_mod(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_NOT_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_not_eq(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_EQUAL_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_eq(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_GREATER: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_great(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_GREATER_GREATER: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_rshift(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_GREATER_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_great_eq(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_LESS: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_less(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_LESS_LESS: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_lshift(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_LESS_EQUAL: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_less_eq(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_AND: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_and(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_OR: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_or(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_BIT_AND: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_bit_and(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_BIT_OR: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_bit_or(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_BIT_XOR: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        lexpr_eval(lexpr_get_arg(expr, 1), interpreter, &right);
        ret = lvalue_bit_xor(&left, &right, result);
        lvalue_reset(&left);
        lvalue_reset(&right);
        break;
      }
      case LTOKEN_MINUS: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        ret = lvalue_minus(&left, result);
        lvalue_reset(&left);
        break;
      }
      case LTOKEN_NOT: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        ret = lvalue_not(&left, result);
        lvalue_reset(&left);
        break;
      }
      case LTOKEN_BIT_NOT: {
        lexpr_eval(lexpr_get_arg(expr, 0), interpreter, &left);
        ret = lvalue_bit_not(&left, result);
        lvalue_reset(&left);
        break;
      }
      case LTOKEN_CALL_FUNC:
      case LTOKEN_CALL_CONSTRUCTOR: {
        lexpr_call_eval(expr, interpreter, result);
        break;
      }
      default:
        break;
    }
    return ret;
  } else if (expr->type == LTOKEN_THIS) {
    return linterpreter_get_var(interpreter, expr, result);
  } else if (expr->type == LTOKEN_SUPER) {
    return linterpreter_get_var(interpreter, expr, result);
  }

  return RET_OK;
}
