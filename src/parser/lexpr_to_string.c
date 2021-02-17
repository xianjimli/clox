#include "parser/lexpr_to_string.h"

static ret_t lexpr_binary_to_string(lexpr_t* expr, lstr_t* str, const char* operator) {
  lexpr_to_string(lexpr_get_arg(expr, 0), str);
  lstr_append(str, operator, - 1);
  lexpr_to_string(lexpr_get_arg(expr, 1), str);

  return RET_OK;
}

static ret_t lexpr_unary_to_string(lexpr_t* expr, lstr_t* str, const char* operator) {
  lstr_append(str, operator, - 1);
  lexpr_to_string(lexpr_get_arg(expr, 0), str);

  return RET_OK;
}

static ret_t lexpr_call_to_string(lexpr_t* expr, lstr_t* str) {
  uint32_t i = 0;
  lexpr_to_string(lexpr_get_arg(expr, 0), str);
  lstr_append(str, expr->type == LTOKEN_CALL_FUNC ? "(" : "{", 1);
  for (i = 1; i < expr->data.args->size; i++) {
    lexpr_t* iter = lexpr_get_arg(expr, i);
    if (i > 1) {
      lstr_append(str, ",", 1);
    }
    lexpr_to_string(iter, str);
  }
  lstr_append(str, expr->type == LTOKEN_CALL_FUNC ? ")" : "}", 1);

  return RET_OK;
}

ret_t lexpr_to_string(lexpr_t* expr, lstr_t* str) {
  char buff[64];
  return_value_if_fail(expr != NULL && str != NULL, RET_BAD_PARAMS);

  if (expr->has_value) {
    if (expr->type == LTOKEN_VAR) {
      lexpr_unary_to_string(expr, str, "var ");
    }

    if (expr->type == LTOKEN_STRING) {
      lstr_append(str, "\"", 1);
      /*TODO: escape*/
      lstr_append(str, lvalue_to_string(lexpr_get_value(expr), buff, sizeof(buff)), -1);
      lstr_append(str, "\"", 1);
    } else {
      lstr_append(str, lvalue_to_string(lexpr_get_value(expr), buff, sizeof(buff)), -1);
    }
  } else if (expr->has_args) {
    switch (expr->type) {
      case LTOKEN_PLUS:
      case LTOKEN_PLUS_EQUAL: {
        lexpr_binary_to_string(expr, str, "+");
        break;
      }
      case LTOKEN_SUB:
      case LTOKEN_MINUS_EQUAL: {
        lexpr_binary_to_string(expr, str, "-");
        break;
      }
      case LTOKEN_GROUP: {
        lstr_append(str, "(", 1);
        lexpr_to_string(lexpr_get_arg(expr, 0), str);
        lstr_append(str, ")", 1);
        break;
      }
      case LTOKEN_EQUAL: {
        lexpr_binary_to_string(expr, str, "=");
        break;
      }
      case LTOKEN_GET: {
        lexpr_binary_to_string(expr, str, ".");
        break;
      }
      case LTOKEN_STAR:
      case LTOKEN_STAR_EQUAL: {
        lexpr_binary_to_string(expr, str, "*");
        break;
      }
      case LTOKEN_SLASH:
      case LTOKEN_SLASH_EQUAL: {
        lexpr_binary_to_string(expr, str, "/");
        break;
      }
      case LTOKEN_PERCENT: {
        lexpr_binary_to_string(expr, str, "%");
        break;
      }
      case LTOKEN_NOT_EQUAL: {
        lexpr_binary_to_string(expr, str, "!=");
        break;
      }
      case LTOKEN_EQUAL_EQUAL: {
        lexpr_binary_to_string(expr, str, "==");
        break;
      }
      case LTOKEN_GREATER: {
        lexpr_binary_to_string(expr, str, ">");
        break;
      }
      case LTOKEN_GREATER_GREATER: {
        lexpr_binary_to_string(expr, str, ">>");
        break;
      }
      case LTOKEN_GREATER_EQUAL: {
        lexpr_binary_to_string(expr, str, ">=");
        break;
      }
      case LTOKEN_LESS: {
        lexpr_binary_to_string(expr, str, "<");
        break;
      }
      case LTOKEN_LESS_LESS: {
        lexpr_binary_to_string(expr, str, "<<");
        break;
      }
      case LTOKEN_LESS_EQUAL: {
        lexpr_binary_to_string(expr, str, "<=");
        break;
      }
      case LTOKEN_AND: {
        lexpr_binary_to_string(expr, str, "&&");
        break;
      }
      case LTOKEN_OR: {
        lexpr_binary_to_string(expr, str, "||");
        break;
      }
      case LTOKEN_BIT_AND: {
        lexpr_binary_to_string(expr, str, "&");
        break;
      }
      case LTOKEN_BIT_OR: {
        lexpr_binary_to_string(expr, str, "|");
        break;
      }
      case LTOKEN_BIT_XOR: {
        lexpr_binary_to_string(expr, str, "^");
        break;
      }
      case LTOKEN_SET: {
        lexpr_to_string(lexpr_get_arg(expr, 0), str);
        lstr_append(str, ".", 1);
        lexpr_to_string(lexpr_get_arg(expr, 1), str);
        lstr_append(str, "=", 1);
        lexpr_to_string(lexpr_get_arg(expr, 2), str);
        break;
      }
      case LTOKEN_MINUS: {
        lexpr_unary_to_string(expr, str, "-");
        break;
      }
      case LTOKEN_NOT: {
        lexpr_unary_to_string(expr, str, "!");
        break;
      }
      case LTOKEN_BIT_NOT: {
        lexpr_unary_to_string(expr, str, "~");
        break;
      }
      case LTOKEN_CALL_FUNC:
      case LTOKEN_CALL_CONSTRUCTOR: {
        lexpr_call_to_string(expr, str);
        break;
      }
      default:
        break;
    }
  } else if (expr->type == LTOKEN_THIS) {
    lstr_append(str, LSTR_TOKEN_THIS, -1);
  } else if (expr->type == LTOKEN_SUPER) {
    lstr_append(str, LSTR_TOKEN_SUPER, -1);
  }

  return RET_OK;
}
