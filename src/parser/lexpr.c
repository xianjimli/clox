#include "parser/lexpr.h"

static lexpr_t* lexpr_create_with_type(ltoken_type_t type) {
  lexpr_t* expr = LMEM_NEW(lexpr_t);
  return_value_if_fail(expr != NULL, NULL);

  expr->type = type;

  return expr;
}

lexpr_t* lexpr_create(ltoken_t* token) {
  return_value_if_fail(token != NULL, NULL);
  return lexpr_create_with_type(token->type);
}

lexpr_t* lexpr_create_id(ltoken_t* token) {
  lexpr_t* expr = lexpr_create(token);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  lvalue_set_id(&(expr->data.value), token->lexeme, token->lexeme_len);

  return expr;
}

lexpr_t* lexpr_create_number(ltoken_t* token) {
  lexpr_t* expr = lexpr_create(token);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  expr->data.value = token->literal;

  return expr;
}

lexpr_t* lexpr_create_bool(ltoken_t* token) {
  lexpr_t* expr = lexpr_create(token);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  lvalue_set_bool(&(expr->data.value), token->type == LTOKEN_TRUE);

  return expr;
}

lexpr_t* lexpr_create_nil(void) {
  lexpr_t* expr = lexpr_create_with_type(LTOKEN_NIL);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  lvalue_set_nil(&(expr->data.value));

  return expr;
}

lexpr_t* lexpr_create_string(ltoken_t* token) {
  lexpr_t* expr = lexpr_create(token);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  lvalue_set_sized_string(&(expr->data.value), token->lexeme, token->lexeme_len);

  return expr;
}

static lexpr_t* lexpr_set_args2(lexpr_t* expr, lexpr_t* left, lexpr_t* right) {
  return_value_if_fail(expr != NULL, NULL);
  if (left == NULL || right == NULL) {
    lexpr_destroy(expr);
    return NULL;
  }

  expr->has_args = TRUE;
  expr->data.args = lexprs_create(2);
  if (expr->data.args != NULL) {
    lexprs_push(expr->data.args, left);
    lexprs_push(expr->data.args, right);
  } else {
    lexpr_destroy(expr);
    expr = NULL;
  }

  return expr;
}

static lexpr_t* lexpr_set_args1(lexpr_t* expr, lexpr_t* right) {
  return_value_if_fail(expr != NULL, NULL);
  if (right == NULL) {
    lexpr_destroy(expr);
    return NULL;
  }

  expr->has_args = TRUE;
  expr->data.args = lexprs_create(1);
  if (expr->data.args != NULL) {
    lexprs_push(expr->data.args, right);
  } else {
    lexpr_destroy(expr);
    expr = NULL;
  }

  return expr;
}

lexpr_t* lexpr_create_binary(ltoken_type_t type, lexpr_t* left, lexpr_t* right) {
  lexpr_t* expr = lexpr_create_with_type(type == LTOKEN_MINUS ? LTOKEN_SUB : type);
  return_value_if_fail(expr != NULL, NULL);

  return lexpr_set_args2(expr, left, right);
}

ret_t lexpr_destroy(lexpr_t* expr) {
  if (expr != NULL && expr->has_args) {
    ltoken_type_t type = expr->type;

    /*a = a + 1: a is shared by both side, to avoid free twice, set it to null*/
    if (type == LTOKEN_STAR_EQUAL || type == LTOKEN_SLASH_EQUAL || type == LTOKEN_MINUS_EQUAL ||
        type == LTOKEN_PLUS_EQUAL) {
      lexprs_t* exprs = expr->data.args;
      exprs->elms[0] = NULL;
    }

    lexprs_destroy(expr->data.args);
  }
  LMEM_FREE(expr);

  return RET_OK;
}

lexpr_t* lexpr_create_assign(lexpr_t* name, lexpr_t* right) {
  lexpr_t* expr = NULL;
  return_value_if_fail(name != NULL && right != NULL, NULL);
  expr = lexpr_create_with_type(LTOKEN_EQUAL);
  return_value_if_fail(expr != NULL, NULL);

  return lexpr_set_args2(expr, name, right);
}

lexpr_t* lexpr_create_unary(ltoken_type_t type, lexpr_t* right) {
  lexpr_t* expr = lexpr_create_with_type(type);
  return_value_if_fail(expr != NULL, NULL);

  return lexpr_set_args1(expr, right);
}

lvalue_t* lexpr_get_value(lexpr_t* expr) {
  return_value_if_fail(expr != NULL && expr->has_value, NULL);

  return &(expr->data.value);
}

const sized_str_t* lexpr_get_value_id(lexpr_t* expr) {
  lvalue_t* v = lexpr_get_value(expr);
  return_value_if_fail(v != NULL, NULL);

  return lvalue_id(v);
}

const sized_str_t* lexpr_get_value_string(lexpr_t* expr) {
  lvalue_t* v = lexpr_get_value(expr);
  return_value_if_fail(v != NULL, NULL);

  return lvalue_sized_string(v);
}

lexpr_t* lexpr_get_arg(lexpr_t* expr, uint16_t index) {
  return_value_if_fail(expr != NULL && expr->has_args, NULL);
  return_value_if_fail(expr->data.args != NULL && index < expr->data.args->size, NULL);

  return expr->data.args->elms[index];
}

lexpr_t* lexpr_create_call(ltoken_type_t type, lexprs_t* args) {
  lexpr_t* expr = NULL;
  return_value_if_fail(args != NULL, NULL);
  return_value_if_fail(type == LTOKEN_LEFT_PAREN || type == LTOKEN_LEFT_BRACE, NULL);
  expr = lexpr_create_with_type(type == LTOKEN_LEFT_PAREN ? LTOKEN_CALL_FUNC
                                                          : LTOKEN_CALL_CONSTRUCTOR);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_args = TRUE;
  expr->data.args = args;

  return expr;
}

lexpr_t* lexpr_create_get(lexpr_t* obj, lexpr_t* name) {
  lexpr_t* expr = lexpr_create_with_type(LTOKEN_GET);
  return_value_if_fail(expr != NULL && name != NULL, NULL);

  return lexpr_set_args2(expr, obj, name);
}

lexpr_t* lexpr_create_set(lexpr_t* obj, lexpr_t* name, lexpr_t* value) {
  lexpr_t* expr = NULL;
  return_value_if_fail(obj != NULL && name != NULL && value != NULL, NULL);
  expr = lexpr_create_with_type(LTOKEN_SET);

  if (expr == NULL) {
    lexpr_destroy(name);
    return NULL;
  }

  expr->has_args = TRUE;
  expr->data.args = lexprs_create(3);
  if (expr->data.args != NULL) {
    lexprs_push(expr->data.args, obj);
    lexprs_push(expr->data.args, name);
    lexprs_push(expr->data.args, value);
  } else {
    lexpr_destroy(expr);
    lexpr_destroy(name);
    expr = NULL;
  }

  return expr;
}

lexpr_t* lexpr_create_group(lexpr_t* expr) {
  lexpr_t* group = NULL;
  return_value_if_fail(expr != NULL, NULL);
  group = lexpr_create_with_type(LTOKEN_GROUP);
  return_value_if_fail(group != NULL, NULL);

  return lexpr_set_args1(group, expr);
}

lexpr_t* lexpr_create_this(void) {
  return lexpr_create_with_type(LTOKEN_THIS);
}

lexpr_t* lexpr_create_super(void) {
  return lexpr_create_with_type(LTOKEN_SUPER);
}

lexpr_t* lexpr_create_var(ltoken_t* name) {
  lexpr_t* expr = NULL;
  return_value_if_fail(name != NULL && name->type == LTOKEN_IDENTIFIER, NULL);
  expr = lexpr_create_with_type(LTOKEN_VAR);
  return_value_if_fail(expr != NULL, NULL);

  expr->has_value = TRUE;
  lvalue_set_id(&(expr->data.value), name->lexeme, name->lexeme_len);

  return expr;
}
