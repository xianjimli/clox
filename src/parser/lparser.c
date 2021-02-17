
#include "parser/lparser.h"

#define GET_TOKEN() llexer_get(&(parser->lexer));
#define MATCH(type) llexer_match(&(parser->lexer), type)
#define UNGET_TOKEN() llexer_unget(&(parser->lexer));
#define SET_LINE(astmt) astmt->line = parser->lexer.line

static bool_t ltoken_is_id(ltoken_t* t) {
  return (t->type == LTOKEN_IDENTIFIER || t->type == LTOKEN_THIS || t->type == LTOKEN_SUPER);
}

lparser_t* lparser_init(lparser_t* parser, const char* input) {
  return_value_if_fail(parser != NULL && input != NULL, NULL);
  return_value_if_fail(llexer_init(&(parser->lexer), input) != NULL, NULL);

  return parser;
}

static ret_t lparser_error(lparser_t* parser, const char* message) {
  parser->errors++;
  log_debug("%d: %s\n", parser->lexer.line, message);
  return RET_OK;
}

static ltoken_t* lparser_expect(lparser_t* parser, ltoken_type_t type, const char* message) {
  ltoken_t* t = GET_TOKEN();
  if (t->type == type) {
    return t;
  } else {
    lparser_error(parser, message);
    UNGET_TOKEN();
    return NULL;
  }
}

lexpr_t* lparser_parse_expr(lparser_t* parser) {
  return_value_if_fail(parser != NULL, NULL);

  return lparser_parse_assignment(parser);
}

static lstmt_t* lparser_parse_stmt_expr(lparser_t* parser) {
  lstmt_t* stmt = NULL;
  lexpr_t* expr = lparser_parse_expr(parser);
  return_value_if_fail(expr != NULL, NULL);
  stmt = lstmt_create_expr(expr);
  if (stmt != NULL) {
    SET_LINE(stmt);
    return stmt;
  } else {
    lexpr_destroy(expr);
    return NULL;
  }
}

lexpr_t* lparser_parse_primary(lparser_t* parser) {
  ltoken_t* t = GET_TOKEN();
  switch (t->type) {
    case LTOKEN_TRUE:
    case LTOKEN_FALSE: {
      return lexpr_create_bool(t);
    }
    case LTOKEN_NIL: {
      return lexpr_create_nil();
    }
    case LTOKEN_NUMBER: {
      return lexpr_create_number(t);
    }
    case LTOKEN_IDENTIFIER: {
      return lexpr_create_id(t);
    }
    case LTOKEN_STRING: {
      return lexpr_create_string(t);
    }
    case LTOKEN_SUPER: {
      return lexpr_create_super();
    }
    case LTOKEN_THIS: {
      return lexpr_create_this();
    }
    case LTOKEN_LEFT_PAREN: {
      lexpr_t* expr = lparser_parse_expr(parser);
      return_value_if_fail(expr != NULL, NULL);
      t = lparser_expect(parser, LTOKEN_RIGHT_PAREN, "expected ')'");
      return lexpr_create_group(expr);
    }
    default: {
      UNGET_TOKEN();
      return NULL;
    }
  }
}

lexpr_t* lparser_parse_call_args(lparser_t* parser, lexpr_t* callee) {
  lexpr_t* expr = NULL;
  lexpr_t* arg = NULL;
  ltoken_t* t = GET_TOKEN();
  lexprs_t* args = lexprs_create(3);
  ltoken_type_t type = t->type;
  return_value_if_fail(args != NULL, NULL);
  goto_error_if_fail(lexprs_push(args, callee) == RET_OK);

  if (!MATCH(LTOKEN_RIGHT_PAREN)) {
    do {
      arg = lparser_parse_expr(parser);
      goto_error_if_fail(arg != NULL);
      goto_error_if_fail(lexprs_push(args, arg) == RET_OK);
    } while (MATCH(LTOKEN_COMMA));
    lparser_expect(parser, LTOKEN_RIGHT_PAREN, "expected ')'");
  }

  expr = lexpr_create_call(type, args);
  goto_error_if_fail(expr != NULL);

  return expr;
error:
  args->elms[0] = NULL; /*clear callee*/
  lexprs_destroy(args);

  return NULL;
}

lexpr_t* lparser_parse_call(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* texpr = NULL;
  lexpr_t* expr = lparser_parse_primary(parser);
  return_value_if_fail(expr != NULL, NULL);

  SET_LINE(expr);
  while (TRUE) {
    if (MATCH(LTOKEN_LEFT_PAREN)) {
      UNGET_TOKEN();
      texpr = lparser_parse_call_args(parser, expr);
      goto_error_if_fail(texpr != NULL);
      expr = texpr;
    } else if (MATCH(LTOKEN_DOT)) {
      t = GET_TOKEN();
      goto_error_if_fail(t != NULL);
      goto_error_if_fail(ltoken_is_id(t));
      texpr = lexpr_create_id(t);
      goto_error_if_fail(texpr != NULL);
      SET_LINE(texpr);

      texpr = lexpr_create_get(expr, texpr);
      goto_error_if_fail(texpr != NULL);
      SET_LINE(texpr);

      expr = texpr;
    } else {
      break;
    }
  }

  return expr;
error:
  lexpr_destroy(expr);
  return NULL;
}

lexpr_t* lparser_parse_unary(lparser_t* parser) {
  ltoken_t* t = GET_TOKEN();
  if (t->type == LTOKEN_NOT || t->type == LTOKEN_BIT_NOT || t->type == LTOKEN_MINUS) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_unary(parser);
    return_value_if_fail(right != NULL, NULL);
    SET_LINE(right);
    return lexpr_create_unary(type, right);
  } else {
    UNGET_TOKEN();
    return lparser_parse_call(parser);
  }
}

lexpr_t* lparser_parse_factor(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_unary(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_SLASH || t->type == LTOKEN_PERCENT || t->type == LTOKEN_STAR) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_unary(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_term(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_factor(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_PLUS || t->type == LTOKEN_MINUS) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_factor(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_compare(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_term(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_LESS || t->type == LTOKEN_LESS_EQUAL || t->type == LTOKEN_GREATER ||
         t->type == LTOKEN_GREATER_EQUAL) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_term(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_equality(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_compare(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_EQUAL_EQUAL || t->type == LTOKEN_NOT_EQUAL) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_compare(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_and(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_equality(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_AND) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_equality(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_or(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_and(parser);
  return_value_if_fail(expr != NULL, NULL);

  t = GET_TOKEN();
  while (t->type == LTOKEN_OR) {
    ltoken_type_t type = t->type;
    lexpr_t* right = lparser_parse_and(parser);
    goto_error_if_fail(right != NULL);

    expr = lexpr_create_binary(type, expr, right);
    goto_error_if_fail(expr != NULL);
    SET_LINE(expr);

    t = GET_TOKEN();
  }
  UNGET_TOKEN();

  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lexpr_t* lparser_parse_assignment(lparser_t* parser) {
  ltoken_t* t = NULL;
  lexpr_t* expr = lparser_parse_or(parser);
  return_value_if_fail(expr != NULL, NULL);
  t = GET_TOKEN();
  if (t->type == LTOKEN_EQUAL || t->type == LTOKEN_STAR_EQUAL || t->type == LTOKEN_SLASH_EQUAL ||
      t->type == LTOKEN_PLUS_EQUAL || t->type == LTOKEN_MINUS_EQUAL) {
    ltoken_type_t op_type = t->type;
    lexpr_t* value = lparser_parse_assignment(parser);
    goto_error_if_fail(value != NULL);

    if (op_type != LTOKEN_EQUAL) {
      value = lexpr_create_binary(op_type, expr, value);
      goto_error_if_fail(value != NULL);
    }

    if (expr->type == LTOKEN_IDENTIFIER) {
      expr = lexpr_create_assign(expr, value);
      goto_error_if_fail(expr != NULL);
    } else if (expr->type == LTOKEN_GET) {
      lexpr_t* obj = lexpr_get_arg(expr, 0);
      lexpr_t* name = lexpr_get_arg(expr, 1);
      lexpr_t* texpr = lexpr_create_set(obj, name, value);
      goto_error_if_fail(texpr != NULL);
      expr->data.args->size = 0;
      lexpr_destroy(expr);
      expr = texpr;
    } else {
      lparser_error(parser, "invalid assignment");
    }
  } else {
    UNGET_TOKEN();
  }

  SET_LINE(expr);
  return expr;
error:
  lexpr_destroy(expr);

  return NULL;
}

lstmt_t* lparser_parse_declaration(lparser_t* parser) {
  while (MATCH(LTOKEN_SEMICOLON))
    ;

  if (MATCH(LTOKEN_VAR)) {
    return lparser_parse_var(parser);
  } else if (MATCH(LTOKEN_FUN)) {
    return lparser_parse_func(parser);
  } else if (MATCH(LTOKEN_CLASS)) {
    return lparser_parse_class(parser);
  }

  return lparser_parse_stmt(parser);
}

lstmt_t* lparser_parse_block(lparser_t* parser) {
  lstmt_t* block = NULL;
  lstmts_t* stmts = lstmts_create(5);
  return_value_if_fail(stmts != NULL, NULL);

  while (!MATCH(LTOKEN_RIGHT_BRACE) && !MATCH(LTOKEN_EOF)) {
    lstmt_t* stmt = lparser_parse_declaration(parser);
    if (stmt == NULL) {
      break;
    }
    goto_error_if_fail(lstmts_push(stmts, stmt) == RET_OK);
    while (MATCH(LTOKEN_SEMICOLON))
      ;
  }
  block = lstmt_create_block(stmts);
  goto_error_if_fail(block != NULL);
  SET_LINE(block);
  return block;
error:
  lstmts_destroy(stmts);
  return NULL;
}

lstmt_t* lparser_parse_if(lparser_t* parser) {
  ltoken_t* t = NULL;
  lstmt_t* stmt_if = NULL;
  lexpr_t* condition = NULL;
  lstmt_t* stmt_then = NULL;
  lstmt_t* stmt_else = NULL;
  t = lparser_expect(parser, LTOKEN_LEFT_PAREN, "missing '('");
  return_value_if_fail(t != NULL, NULL);
  condition = lparser_parse_expr(parser);
  return_value_if_fail(condition != NULL, NULL);
  t = lparser_expect(parser, LTOKEN_RIGHT_PAREN, "missing ')'");
  goto_error_if_fail(t != NULL);

  t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "missing '{'");
  goto_error_if_fail(t != NULL);
  stmt_then = lparser_parse_block(parser);
  goto_error_if_fail(stmt_then != NULL);
  if (MATCH(LTOKEN_ELSE)) {
    t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "missing '}'");
    goto_error_if_fail(t != NULL);
    stmt_else = lparser_parse_block(parser);
    goto_error_if_fail(stmt_else != NULL);
  }
  stmt_if = lstmt_create_if(condition, stmt_then, stmt_else);
  goto_error_if_fail(stmt_if != NULL);
  SET_LINE(stmt_if);
  return stmt_if;
error:
  lexpr_destroy(condition);
  lstmt_destroy(stmt_then);
  lstmt_destroy(stmt_else);

  return NULL;
}

lstmt_t* lparser_parse_while(lparser_t* parser) {
  ltoken_t* t = NULL;
  lstmt_t* stmt_while = NULL;
  lexpr_t* condition = NULL;
  lstmt_t* stmt_body = NULL;
  t = lparser_expect(parser, LTOKEN_LEFT_PAREN, "missing '('");
  return_value_if_fail(t != NULL, NULL);
  condition = lparser_parse_expr(parser);
  return_value_if_fail(condition != NULL, NULL);
  t = lparser_expect(parser, LTOKEN_RIGHT_PAREN, "missing ')'");
  goto_error_if_fail(t != NULL);

  t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "missing '{'");
  goto_error_if_fail(t != NULL);
  stmt_body = lparser_parse_block(parser);
  goto_error_if_fail(stmt_body != NULL);
  stmt_while = lstmt_create_while(condition, stmt_body);
  goto_error_if_fail(stmt_while != NULL);

  SET_LINE(stmt_while);
  return stmt_while;
error:
  lexpr_destroy(condition);
  lstmt_destroy(stmt_body);

  return NULL;
}

lstmt_t* lparser_parse_for(lparser_t* parser) {
  ltoken_t* t = NULL;
  lstmt_t* stmt_for = NULL;
  lstmt_t* initializer = NULL;
  lexpr_t* condition = NULL;
  lexpr_t* increment = NULL;
  lstmt_t* stmt_body = NULL;
  t = lparser_expect(parser, LTOKEN_LEFT_PAREN, "missing '('");
  return_value_if_fail(t != NULL, NULL);

  if (!MATCH(LTOKEN_SEMICOLON)) {
    initializer = lparser_parse_stmt_expr(parser);
    goto_error_if_fail(initializer != NULL);

    t = lparser_expect(parser, LTOKEN_SEMICOLON, "missing ';'");
    goto_error_if_fail(t != NULL);
  } else if (MATCH(LTOKEN_VAR)) {
    initializer = lparser_parse_var(parser);
    goto_error_if_fail(initializer != NULL);

    t = lparser_expect(parser, LTOKEN_SEMICOLON, "missing ';'");
    goto_error_if_fail(t != NULL);
  }

  if (!MATCH(LTOKEN_SEMICOLON)) {
    condition = lparser_parse_expr(parser);
    goto_error_if_fail(condition != NULL);

    t = lparser_expect(parser, LTOKEN_SEMICOLON, "missing ';'");
    goto_error_if_fail(t != NULL);
  }

  if (!MATCH(LTOKEN_RIGHT_PAREN)) {
    increment = lparser_parse_expr(parser);
    goto_error_if_fail(increment != NULL);

    t = lparser_expect(parser, LTOKEN_RIGHT_PAREN, "missing ')'");
    goto_error_if_fail(t != NULL);
  }

  t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "missing '{'");
  goto_error_if_fail(t != NULL);
  stmt_body = lparser_parse_block(parser);
  goto_error_if_fail(stmt_body != NULL);
  stmt_for = lstmt_create_for(initializer, condition, increment, stmt_body);
  goto_error_if_fail(stmt_for != NULL);
  SET_LINE(stmt_for);

  return stmt_for;
error:
  lstmt_destroy(initializer);
  lexpr_destroy(condition);
  lexpr_destroy(increment);
  lstmt_destroy(stmt_body);

  return NULL;
}

lstmt_t* lparser_parse_stmt(lparser_t* parser) {
  while (MATCH(LTOKEN_SEMICOLON))
    ;

  if (MATCH(LTOKEN_IF)) {
    return lparser_parse_if(parser);
  } else if (MATCH(LTOKEN_WHILE)) {
    return lparser_parse_while(parser);
  } else if (MATCH(LTOKEN_RETURN)) {
    return lparser_parse_return(parser);
  } else if (MATCH(LTOKEN_BREAK)) {
    return lstmt_create_break();
  } else if (MATCH(LTOKEN_CONTINUE)) {
    return lstmt_create_continue();
  } else if (MATCH(LTOKEN_FOR)) {
    return lparser_parse_for(parser);
  } else if (MATCH(LTOKEN_LEFT_BRACE)) {
    return lparser_parse_block(parser);
  }

  return lparser_parse_stmt_expr(parser);
}

lstmt_t* lparser_parse_var(lparser_t* parser) {
  ltoken_t* t = GET_TOKEN();
  lexpr_t* expr = NULL;
  lstmt_t* stmt = NULL;
  if (t->type == LTOKEN_IDENTIFIER) {
    ltoken_t name = *t;
    if (MATCH(LTOKEN_EQUAL)) {
      expr = lparser_parse_expr(parser);
      goto_error_if_fail(expr != NULL);
      stmt = lstmt_create_var(&name, expr);
      goto_error_if_fail(stmt != NULL);
      SET_LINE(stmt);
      return stmt;
    } else {
      lparser_error(parser, "missing '='");
      return NULL;
    }
  } else {
    lparser_error(parser, "missing var name");
    return NULL;
  }
error:
  lexpr_destroy(expr);
  return NULL;
}

lstmt_t* lparser_parse_return(lparser_t* parser) {
  lstmt_t* stmt = NULL;
  lexpr_t* expr = NULL;
  ltoken_t* t = GET_TOKEN();
  if (t->type == LTOKEN_EOF || t->type == LTOKEN_SEMICOLON || t->type == LTOKEN_RIGHT_BRACE) {
    UNGET_TOKEN();
  } else {
    UNGET_TOKEN();
    expr = lparser_parse_expr(parser);
    return_value_if_fail(expr != NULL, NULL);
  }

  stmt = lstmt_create_return(expr);
  if (stmt != NULL) {
    SET_LINE(stmt);
    return stmt;
  } else {
    lexpr_destroy(expr);
    return NULL;
  }
}

lstmt_t* lparser_parse_func(lparser_t* parser) {
  ltoken_t* t = GET_TOKEN();
  lparams_t* params = NULL;
  lstmt_t* func = NULL;
  lstmt_t* body = NULL;

  if (t->type == LTOKEN_IDENTIFIER) {
    ltoken_t name = *t;
    if (MATCH(LTOKEN_LEFT_PAREN)) {
      params = lparams_create(4);
      return_value_if_fail(params != NULL, NULL);

      while (!MATCH(LTOKEN_RIGHT_PAREN)) {
        t = lparser_expect(parser, LTOKEN_IDENTIFIER, "expected an id.");
        goto_error_if_fail(t != NULL);
        goto_error_if_fail(lparams_push(params, t->lexeme, t->lexeme_len) == RET_OK);
        MATCH(LTOKEN_COMMA);
      }
      t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "expected an id.");
      goto_error_if_fail(t != NULL);
      body = lparser_parse_block(parser);
      goto_error_if_fail(body != NULL);
      func = lstmt_create_func(&name, params, body);
      goto_error_if_fail(func != NULL);
      SET_LINE(func);
      return func;
    } else {
      lparser_error(parser, "missing '='");
      return NULL;
    }
  } else {
    lparser_error(parser, "missing func name");
    return NULL;
  }

error:
  lstmt_destroy(body);
  lparams_destroy(params);

  return NULL;
}

lstmt_t* lparser_parse_class(lparser_t* parser) {
  ltoken_t* t = GET_TOKEN();
  lstmt_t* stmt = NULL;
  lstmts_t* funcs = NULL;

  if (t->type == LTOKEN_IDENTIFIER) {
    ltoken_t name = *t;
    ltoken_t save_super_name;
    ltoken_t* super_name = NULL;

    if (MATCH(LTOKEN_LESS)) {
      super_name = lparser_expect(parser, LTOKEN_IDENTIFIER, "expected '<'");
      goto_error_if_fail(super_name != NULL);
      save_super_name = *super_name;
      super_name = &save_super_name;
    }

    t = lparser_expect(parser, LTOKEN_LEFT_BRACE, "expected an id.");
    goto_error_if_fail(t != NULL);

    funcs = lstmts_create(5);
    goto_error_if_fail(funcs != NULL);
    while (!MATCH(LTOKEN_RIGHT_BRACE)) {
      lstmt_t* func = lparser_parse_func(parser);
      goto_error_if_fail(func != NULL);
      assert(func->type == LTOKEN_FUN);
      func->type = LTOKEN_METHOD;
      goto_error_if_fail(lstmts_push(funcs, func) == RET_OK);
      MATCH(LTOKEN_COMMA);
    }

    stmt = lstmt_create_class(&name, super_name, funcs);
    goto_error_if_fail(stmt != NULL);
    SET_LINE(stmt);
    return stmt;
  } else {
    lparser_error(parser, "missing class name");
    return NULL;
  }

error:
  lstmts_destroy(funcs);
  return NULL;
}

lstmt_t* lparser_parse(lparser_t* parser) {
  return lparser_parse_block(parser);
}

ret_t lparser_deinit(lparser_t* parser) {
  llexer_deinit(&(parser->lexer));
  return RET_OK;
}
