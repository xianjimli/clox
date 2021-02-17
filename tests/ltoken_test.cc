#include "gtest/gtest.h"
#include "parser/ltoken.h"

TEST(LToken, token) {
  ltoken_t token;
  ltoken_t* t = &token;

  ASSERT_EQ(ltoken_init(t, LTOKEN_LESS, 10) != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_LESS);
  ASSERT_EQ(t->line, 10u);
}

TEST(LToken, id) {
  ltoken_t token;
  ltoken_t* t = &token;

  ASSERT_EQ(ltoken_init_id(t, "hello", 6, 11) != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_IDENTIFIER);
  ASSERT_EQ(t->line, 11u);
  ASSERT_EQ(t->lexeme_len, 6u);
  ASSERT_STREQ(t->lexeme, "hello");
}

TEST(LToken, str) {
  ltoken_t token;
  ltoken_t* t = &token;

  ASSERT_EQ(ltoken_init_str(t, "hello", 6, 11) != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_STRING);
  ASSERT_EQ(t->line, 11u);
  ASSERT_EQ(t->lexeme_len, 6u);
  ASSERT_STREQ(t->lexeme, "hello");
}

TEST(LToken, number) {
  ltoken_t token;
  ltoken_t* t = &token;

  ASSERT_EQ(ltoken_init_number(t, "123", 12) != NULL, true);
  ASSERT_EQ(t->type, LTOKEN_NUMBER);
  ASSERT_EQ(t->line, 12u);
  ASSERT_EQ(lvalue_double(&(t->literal)), 123);
}
