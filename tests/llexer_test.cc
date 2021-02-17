#include "gtest/gtest.h"
#include "parser/llexer.h"

static void test_lexer(const char* str, ltoken_type_t* types, uint32_t nr) {
  uint32_t i = 0;
  llexer_t lexer;
  ltoken_t* t = NULL;
  llexer_t* l = &lexer;

  ASSERT_EQ(llexer_init(l, str) != NULL, true);
  for (i = 0; i < nr; i++) {
    t = llexer_get(l);
    ASSERT_EQ(t != NULL, true);
    ASSERT_EQ(t->type, types[i]);
  }
  llexer_deinit(l);

  ASSERT_EQ(llexer_init(l, str) != NULL, true);
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(llexer_match(l, types[i]), TRUE);
  }
  llexer_deinit(l);

  return;
}

static void test_lexer_sring(const char* str, const char* expected) {
  llexer_t lexer;
  ltoken_t* t = NULL;
  llexer_t* l = &lexer;

  ASSERT_EQ(llexer_init(l, str) != NULL, true);
  t = llexer_get(l);
  ASSERT_EQ(t != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_STRING);
  ASSERT_EQ(strncmp(t->lexeme, expected, t->lexeme_len), 0);
  llexer_deinit(l);

  return;
}

static void test_lexer_id(const char* str, ltoken_type_t type, const char* expected) {
  llexer_t lexer;
  ltoken_t* t = NULL;
  llexer_t* l = &lexer;

  ASSERT_EQ(llexer_init(l, str) != NULL, true);
  t = llexer_get(l);
  ASSERT_EQ(t != NULL, true);
  if (expected != NULL) {
    ASSERT_EQ(t->type, LTOKEN_IDENTIFIER);
    ASSERT_EQ(strncmp(t->lexeme, expected, t->lexeme_len), 0);
  } else {
    ASSERT_EQ(t->type, type);
  }
  llexer_deinit(l);

  return;
}

static void test_lexer_number(const char* str, lvalue_t* expected) {
  llexer_t lexer;
  ltoken_t* t = NULL;
  llexer_t* l = &lexer;

  ASSERT_EQ(llexer_init(l, str) != NULL, true);
  t = llexer_get(l);
  ASSERT_EQ(t != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_NUMBER);
  switch (expected->type) {
    case LVALUE_TYPE_UINT32: {
      ASSERT_EQ(lvalue_uint32(&(t->literal)), lvalue_uint32(expected));
      break;
    }
    case LVALUE_TYPE_UINT64: {
      ASSERT_EQ(lvalue_uint64(&(t->literal)), lvalue_uint64(expected));
      break;
    }
    case LVALUE_TYPE_DOUBLE: {
      ASSERT_EQ(lvalue_double(&(t->literal)), lvalue_double(expected));
      break;
    }
    default:
      break;
  }
  llexer_deinit(l);

  return;
}

TEST(LLexer, LTOKEN_LEFT_PAREN) {
  ltoken_type_t types[] = {LTOKEN_LEFT_PAREN, LTOKEN_EOF};
  test_lexer("(", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_RIGHT_PAREN) {
  ltoken_type_t types[] = {LTOKEN_RIGHT_PAREN, LTOKEN_EOF};
  test_lexer(")", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_LEFT_BRACE) {
  ltoken_type_t types[] = {LTOKEN_LEFT_BRACE, LTOKEN_EOF};
  test_lexer("{", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_RIGHT_BRACE) {
  ltoken_type_t types[] = {LTOKEN_RIGHT_BRACE, LTOKEN_EOF};
  test_lexer("}", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_COMMA) {
  ltoken_type_t types[] = {LTOKEN_COMMA, LTOKEN_EOF};
  test_lexer(",", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_DOT) {
  ltoken_type_t types[] = {LTOKEN_DOT, LTOKEN_EOF};
  test_lexer(".", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_MINUS) {
  ltoken_type_t types[] = {LTOKEN_MINUS, LTOKEN_EOF};
  test_lexer("-", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_MINUS_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_MINUS_EQUAL, LTOKEN_EOF};
  test_lexer("-=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_PLUS) {
  ltoken_type_t types[] = {LTOKEN_PLUS, LTOKEN_EOF};
  test_lexer("+", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_PLUS_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_PLUS_EQUAL, LTOKEN_EOF};
  test_lexer("+=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_SEMICOLON) {
  ltoken_type_t types[] = {LTOKEN_SEMICOLON, LTOKEN_EOF};
  test_lexer(";", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_STAR) {
  ltoken_type_t types[] = {LTOKEN_STAR, LTOKEN_EOF};
  test_lexer("*", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_STAR_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_STAR_EQUAL, LTOKEN_EOF};
  test_lexer("*=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_NOT) {
  ltoken_type_t types[] = {LTOKEN_NOT, LTOKEN_EOF};
  test_lexer("!", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_NOT_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_NOT_EQUAL, LTOKEN_EOF};
  test_lexer("!=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_EQUAL, LTOKEN_EOF};
  test_lexer("=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_EQUAL_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_EQUAL_EQUAL, LTOKEN_EOF};
  test_lexer("==", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_LESS) {
  ltoken_type_t types[] = {LTOKEN_LESS, LTOKEN_EOF};
  test_lexer("<", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_LESS_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_LESS_EQUAL, LTOKEN_EOF};
  test_lexer("<=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_GREATER) {
  ltoken_type_t types[] = {LTOKEN_GREATER, LTOKEN_EOF};
  test_lexer(">", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_GREATER_EQUAL) {
  ltoken_type_t types[] = {LTOKEN_GREATER_EQUAL, LTOKEN_EOF};
  test_lexer(">=", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_SLASH) {
  ltoken_type_t types[] = {LTOKEN_SLASH, LTOKEN_EOF};
  test_lexer("/", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_BIT_AND) {
  ltoken_type_t types[] = {LTOKEN_BIT_AND, LTOKEN_EOF};
  test_lexer("&", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_BIT_OR) {
  ltoken_type_t types[] = {LTOKEN_BIT_OR, LTOKEN_EOF};
  test_lexer("|", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_BIT_NOT) {
  ltoken_type_t types[] = {LTOKEN_BIT_NOT, LTOKEN_EOF};
  test_lexer("~", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_BIT_XOR) {
  ltoken_type_t types[] = {LTOKEN_BIT_XOR, LTOKEN_EOF};
  test_lexer("^", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_AND) {
  ltoken_type_t types[] = {LTOKEN_AND, LTOKEN_EOF};
  test_lexer("&&", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_OR) {
  ltoken_type_t types[] = {LTOKEN_OR, LTOKEN_EOF};
  test_lexer("||", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_COMMENT) {
  ltoken_type_t types[] = {LTOKEN_COMMENT, LTOKEN_EOF};
  test_lexer("//123", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_COMMENT1) {
  ltoken_type_t types[] = {LTOKEN_COMMENT, LTOKEN_EOF};
  test_lexer("/*abc*/", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic1) {
  ltoken_type_t types[] = {LTOKEN_NUMBER, LTOKEN_PLUS,   LTOKEN_NUMBER,
                           LTOKEN_STAR,   LTOKEN_NUMBER, LTOKEN_EOF};
  test_lexer("1+2*4", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic2) {
  ltoken_type_t types[] = {LTOKEN_NUMBER, LTOKEN_PLUS,   LTOKEN_NUMBER,
                           LTOKEN_STAR,   LTOKEN_NUMBER, LTOKEN_EOF};
  test_lexer("\t1 + 2 * 4 ", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic3) {
  ltoken_type_t types[] = {LTOKEN_IDENTIFIER, LTOKEN_LEFT_PAREN,  LTOKEN_NUMBER, LTOKEN_COMMA,
                           LTOKEN_NUMBER,     LTOKEN_RIGHT_PAREN, LTOKEN_EOF};
  test_lexer("aa(1,2)", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic4) {
  ltoken_type_t types[] = {LTOKEN_IDENTIFIER, LTOKEN_LEFT_PAREN,  LTOKEN_NUMBER, LTOKEN_COMMA,
                           LTOKEN_NUMBER,     LTOKEN_RIGHT_PAREN, LTOKEN_EOF};
  test_lexer("aa (1, \n2) ", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic5) {
  ltoken_type_t types[] = {LTOKEN_IF,         LTOKEN_LEFT_PAREN,  LTOKEN_NUMBER,
                           LTOKEN_LESS,       LTOKEN_NUMBER,      LTOKEN_RIGHT_PAREN,
                           LTOKEN_LEFT_BRACE, LTOKEN_RIGHT_BRACE, LTOKEN_EOF};
  test_lexer("if(1<2) {}", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic6) {
  ltoken_type_t types[] = {LTOKEN_IF,         LTOKEN_LEFT_PAREN,  LTOKEN_NUMBER,
                           LTOKEN_LESS_EQUAL, LTOKEN_NUMBER,      LTOKEN_RIGHT_PAREN,
                           LTOKEN_LEFT_BRACE, LTOKEN_RIGHT_BRACE, LTOKEN_EOF};
  test_lexer("if(1<=2) \n{\r\n}", types, ARRAY_SIZE(types));
}

TEST(LLexer, basic7) {
  ltoken_type_t types[] = {LTOKEN_IF,         LTOKEN_LEFT_PAREN,  LTOKEN_NUMBER,
                           LTOKEN_LESS_EQUAL, LTOKEN_NUMBER,      LTOKEN_RIGHT_PAREN,
                           LTOKEN_LEFT_BRACE, LTOKEN_RIGHT_BRACE, LTOKEN_EOF};
  test_lexer("if (1123 <= 0x245) \n{\r\n}", types, ARRAY_SIZE(types));
}

TEST(LLexer, LTOKEN_STRING) {
  test_lexer_sring("\"abc\"", "abc");
  test_lexer_sring("\"a\"", "a");
  test_lexer_sring("\"\"", "");
  test_lexer_sring("\"a'a\"", "a'a");
  test_lexer_sring("\"'\"", "'");

  test_lexer_sring("'abc'", "abc");
  test_lexer_sring("'a'", "a");
  test_lexer_sring("''", "");
  test_lexer_sring("'\"'", "\"");

  test_lexer_sring("`abc`", "abc");
  test_lexer_sring("`\"`", "\"");
  test_lexer_sring("`'`", "'");
}

TEST(LLexer, LTOKEN_NUMBER) {
  lvalue_t v;
  test_lexer_number("1", lvalue_set_uint32(&v, 1));
  test_lexer_number("0x11", lvalue_set_uint32(&v, 0x11));
  test_lexer_number("0X1122", lvalue_set_uint32(&v, 0x1122));
  test_lexer_number("0b11", lvalue_set_uint32(&v, 0b11));
  test_lexer_number("0B1011", lvalue_set_uint32(&v, 0B1011));
  test_lexer_number("1.5", lvalue_set_double(&v, 1.5));
  test_lexer_number("0X1122334455", lvalue_set_uint64(&v, 0X1122334455));
}

TEST(LLexer, LTOKEN_ID) {
  test_lexer_id("class", LTOKEN_CLASS, NULL);
  test_lexer_id("else", LTOKEN_ELSE, NULL);
  test_lexer_id("if", LTOKEN_IF, NULL);
  test_lexer_id("while", LTOKEN_WHILE, NULL);
  test_lexer_id("for", LTOKEN_FOR, NULL);
  test_lexer_id("var", LTOKEN_VAR, NULL);
  test_lexer_id("fun", LTOKEN_FUN, NULL);
  test_lexer_id("true", LTOKEN_TRUE, NULL);
  test_lexer_id("false", LTOKEN_FALSE, NULL);
  test_lexer_id("return", LTOKEN_RETURN, NULL);
  test_lexer_id("super", LTOKEN_SUPER, NULL);
  test_lexer_id("this", LTOKEN_THIS, NULL);
  test_lexer_id("nil", LTOKEN_NIL, NULL);

  test_lexer_id("class1", LTOKEN_IDENTIFIER, "class1");
  test_lexer_id("else1", LTOKEN_IDENTIFIER, "else1");
  test_lexer_id("if1", LTOKEN_IDENTIFIER, "if1");
  test_lexer_id("while1", LTOKEN_IDENTIFIER, "while1");
  test_lexer_id("for1", LTOKEN_IDENTIFIER, "for1");
  test_lexer_id("var1", LTOKEN_IDENTIFIER, "var1");
  test_lexer_id("fun1", LTOKEN_IDENTIFIER, "fun1");
  test_lexer_id("true1", LTOKEN_IDENTIFIER, "true1");
  test_lexer_id("false1", LTOKEN_IDENTIFIER, "false1");
  test_lexer_id("return1", LTOKEN_IDENTIFIER, "return1");
  test_lexer_id("super1", LTOKEN_IDENTIFIER, "super1");
  test_lexer_id("this_a", LTOKEN_IDENTIFIER, "this_a");
  test_lexer_id("nil_123", LTOKEN_IDENTIFIER, "nil_123");
  test_lexer_id("print_aaa", LTOKEN_IDENTIFIER, "print_aaa");
}
