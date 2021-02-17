#include "parser/ltoken.h"

ltoken_t* ltoken_init(ltoken_t* token, ltoken_type_t type, uint32_t line) {
  return_value_if_fail(token != NULL, NULL);
  memset(token, 0x00, sizeof(ltoken_t));
  token->type = type;
  token->line = line;

  return token;
}

ltoken_t* ltoken_init_id(ltoken_t* token, const char* lexeme, uint32_t lexeme_len, uint32_t line) {
  return_value_if_fail(token != NULL && lexeme != NULL, NULL);
  memset(token, 0x00, sizeof(ltoken_t));
  token->type = LTOKEN_IDENTIFIER;
  token->line = line;
  token->lexeme = lexeme;
  token->lexeme_len = lexeme_len;

  return token;
}

ltoken_t* ltoken_init_str(ltoken_t* token, const char* lexeme, uint32_t lexeme_len, uint32_t line) {
  return_value_if_fail(token != NULL && lexeme != NULL, NULL);
  memset(token, 0x00, sizeof(ltoken_t));
  token->type = LTOKEN_STRING;
  token->line = line;
  token->lexeme = lexeme;
  token->lexeme_len = lexeme_len;

  return token;
}

ltoken_t* ltoken_init_number(ltoken_t* token, const char* lexeme, uint32_t line) {
  return_value_if_fail(token != NULL && lexeme != NULL, NULL);
  memset(token, 0x00, sizeof(ltoken_t));
  token->type = LTOKEN_NUMBER;
  token->line = line;
  lvalue_set_double(&(token->literal), atof(lexeme));

  return token;
}
