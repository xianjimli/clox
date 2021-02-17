#include "parser/lexpr.h"
#include "gtest/gtest.h"
#include "parser/lexpr_to_string.h"

TEST(LExpr, basic1) {
  llexer_t lexer;
  lstr_t* str = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "123 + 345") != NULL, true);
  lexpr_t* a = lexpr_create_number(llexer_get(&lexer));
  ltoken_t o = *llexer_get(&lexer);
  lexpr_t* b = lexpr_create_number(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_binary(o.type, a, b);

  ASSERT_EQ(a->type, LTOKEN_NUMBER);
  ASSERT_EQ(b->type, LTOKEN_NUMBER);
  ASSERT_EQ(expr->type, LTOKEN_PLUS);
  ASSERT_EQ(lexpr_get_arg(expr, 0), a);
  ASSERT_EQ(lexpr_get_arg(expr, 1), b);
  ASSERT_EQ(lvalue_uint32(lexpr_get_value(a)), 123u);
  ASSERT_EQ(lvalue_uint32(lexpr_get_value(b)), 345u);

  lstr_clear(str);
  ASSERT_EQ(lexpr_to_string(a, str), RET_OK);
  ASSERT_STREQ(str->str, "123");

  lstr_clear(str);
  ASSERT_EQ(lexpr_to_string(b, str), RET_OK);
  ASSERT_STREQ(str->str, "345");

  lstr_clear(str);
  ASSERT_EQ(lexpr_to_string(expr, str), RET_OK);
  ASSERT_STREQ(str->str, "123+345");

  LOBJECT_UNREF(str);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, string) {
  llexer_t lexer;
  const sized_str_t* str = NULL;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "'abc'") != NULL, true);
  lexpr_t* a = lexpr_create_string(llexer_get(&lexer));

  ASSERT_EQ(a->type, LTOKEN_STRING);
  str = lexpr_get_value_string(a);

  ASSERT_EQ(str->size, 3u);
  ASSERT_EQ(strncmp(str->str, "abc", 3), 0);

  ASSERT_EQ(lexpr_to_string(a, s), RET_OK);
  ASSERT_STREQ(s->str, "\"abc\"");
  LOBJECT_UNREF(s);
  lexpr_destroy(a);
  llexer_deinit(&lexer);
}

TEST(LExpr, id) {
  llexer_t lexer;
  const sized_str_t* str = NULL;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "abc") != NULL, true);
  lexpr_t* a = lexpr_create_id(llexer_get(&lexer));

  ASSERT_EQ(a->type, LTOKEN_IDENTIFIER);
  str = lexpr_get_value_id(a);

  ASSERT_EQ(str->size, 3u);
  ASSERT_EQ(strncmp(str->str, "abc", 3), 0);

  ASSERT_EQ(lexpr_to_string(a, s), RET_OK);
  ASSERT_STREQ(s->str, "abc");
  LOBJECT_UNREF(s);
  lexpr_destroy(a);
  llexer_deinit(&lexer);
}

TEST(LExpr, assign) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "a = 345") != NULL, true);
  lexpr_t* a = lexpr_create_id(llexer_get(&lexer));
  llexer_get(&lexer);
  lexpr_t* b = lexpr_create_number(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_assign(a, b);

  ASSERT_EQ(a->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(b->type, LTOKEN_NUMBER);
  ASSERT_EQ(expr->type, LTOKEN_EQUAL);
  ASSERT_EQ(lexpr_get_arg(expr, 0), a);
  ASSERT_EQ(lexpr_get_arg(expr, 1), b);
  ASSERT_EQ(lvalue_uint32(lexpr_get_value(b)), 345u);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "a=345");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, unary) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "-123") != NULL, true);
  ltoken_type_t minus = llexer_get(&lexer)->type;
  ASSERT_EQ(minus, LTOKEN_MINUS);

  lexpr_t* a = lexpr_create_number(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_unary(minus, a);

  ASSERT_EQ(expr->type, LTOKEN_MINUS);
  ASSERT_EQ(a->type, LTOKEN_NUMBER);
  ASSERT_EQ(lexpr_get_arg(expr, 0), a);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "-123");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, call_func) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  lexprs_t* args = lexprs_create(3);
  ASSERT_EQ(llexer_init(&lexer, "test(1,2)") != NULL, true);

  lexprs_push(args, lexpr_create_id(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_LEFT_PAREN), TRUE);
  lexprs_push(args, lexpr_create_number(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_COMMA), TRUE);
  lexprs_push(args, lexpr_create_number(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_RIGHT_PAREN), TRUE);

  lexpr_t* expr = lexpr_create_call(LTOKEN_LEFT_PAREN, args);
  ASSERT_EQ(expr->type, LTOKEN_CALL_FUNC);
  ASSERT_EQ(lexpr_get_arg(expr, 0)->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(lexpr_get_arg(expr, 1)->type, LTOKEN_NUMBER);
  ASSERT_EQ(lexpr_get_arg(expr, 2)->type, LTOKEN_NUMBER);
  ASSERT_EQ(lvalue_uint32(lexpr_get_value(args->elms[1])), 1u);
  ASSERT_EQ(lvalue_uint32(lexpr_get_value(args->elms[2])), 2u);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "test(1,2)");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, call_construct) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  lexprs_t* args = lexprs_create(3);
  ASSERT_EQ(llexer_init(&lexer, "test{1,2}") != NULL, true);

  lexprs_push(args, lexpr_create_id(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_LEFT_BRACE), TRUE);
  lexprs_push(args, lexpr_create_number(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_COMMA), TRUE);
  lexprs_push(args, lexpr_create_number(llexer_get(&lexer)));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_RIGHT_BRACE), TRUE);

  lexpr_t* expr = lexpr_create_call(LTOKEN_LEFT_BRACE, args);
  ASSERT_EQ(expr->type, LTOKEN_CALL_CONSTRUCTOR);
  ASSERT_EQ(lexpr_get_arg(expr, 0)->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(lexpr_get_arg(expr, 1)->type, LTOKEN_NUMBER);
  ASSERT_EQ(lexpr_get_arg(expr, 2)->type, LTOKEN_NUMBER);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "test{1,2}");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, get) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "a.name") != NULL, true);

  lexpr_t* obj = lexpr_create_id(llexer_get(&lexer));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_DOT), TRUE);
  lexpr_t* name = lexpr_create_id(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_get(obj, name);

  ASSERT_EQ(expr->type, LTOKEN_GET);
  ASSERT_EQ(lexpr_get_arg(expr, 0)->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(lexpr_get_arg(expr, 1)->type, LTOKEN_IDENTIFIER);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "a.name");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, set) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "a.name = 123") != NULL, true);

  lexpr_t* obj = lexpr_create_id(llexer_get(&lexer));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_DOT), TRUE);
  lexpr_t* name = lexpr_create_id(llexer_get(&lexer));
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_EQUAL), TRUE);
  lexpr_t* value = lexpr_create_number(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_set(obj, name, value);

  ASSERT_EQ(expr->type, LTOKEN_SET);
  ASSERT_EQ(lexpr_get_arg(expr, 0)->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(lexpr_get_arg(expr, 1)->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(lexpr_get_arg(expr, 2)->type, LTOKEN_NUMBER);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "a.name=123");

  LOBJECT_UNREF(s);

  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, this) {
  lstr_t* s = lstr_create(100);
  lexpr_t* expr = lexpr_create_this();
  ASSERT_EQ(expr->type, LTOKEN_THIS);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "this");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
}

TEST(LExpr, super) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "super.name") != NULL, true);
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_SUPER), TRUE);
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_DOT), TRUE);
  lexpr_t* expr = lexpr_create_super();
  ASSERT_EQ(expr->type, LTOKEN_SUPER);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "super");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, var) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "var name") != NULL, true);
  ASSERT_EQ(llexer_match(&lexer, LTOKEN_VAR), TRUE);
  lexpr_t* expr = lexpr_create_var(llexer_get(&lexer));
  ASSERT_EQ(expr->type, LTOKEN_VAR);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "var name");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, true) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "true") != NULL, true);
  lexpr_t* expr = lexpr_create_bool(llexer_get(&lexer));
  ASSERT_EQ(expr->type, LTOKEN_TRUE);
  ASSERT_EQ(lvalue_bool(lexpr_get_value(expr)), TRUE);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "true");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, false) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, "false") != NULL, true);
  lexpr_t* expr = lexpr_create_bool(llexer_get(&lexer));
  ASSERT_EQ(expr->type, LTOKEN_FALSE);
  ASSERT_EQ(lvalue_bool(lexpr_get_value(expr)), FALSE);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "false");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, nil) {
  lstr_t* s = lstr_create(100);
  lexpr_t* expr = lexpr_create_nil();
  ASSERT_EQ(expr->type, LTOKEN_NIL);
  ASSERT_EQ(lvalue_pointer(lexpr_get_value(expr)) == NULL, true);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, "nil");

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
}

static void test_binary(const char* text) {
  llexer_t lexer;
  lstr_t* s = lstr_create(100);
  ASSERT_EQ(llexer_init(&lexer, text) != NULL, true);
  lexpr_t* a = lexpr_create_number(llexer_get(&lexer));
  ltoken_t o = *llexer_get(&lexer);
  lexpr_t* b = lexpr_create_number(llexer_get(&lexer));
  lexpr_t* expr = lexpr_create_binary(o.type, a, b);
  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, text);
  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  llexer_deinit(&lexer);
}

TEST(LExpr, binary) {
  test_binary("1-2");
  test_binary("1+2");
  test_binary("1|2");
  test_binary("1&2");
  test_binary("1^2");
  test_binary("1&&2");
  test_binary("1||2");
  test_binary("1*2");
  test_binary("1/2");
  test_binary("1%2");
}
