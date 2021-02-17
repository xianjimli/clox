#ifndef LOX_TOKEN_H
#define LOX_TOKEN_H

#include "base/lvalue.h"

BEGIN_C_DECLS

typedef enum _ltoken_type_t {
  LTOKEN_LEFT_PAREN,
  LTOKEN_RIGHT_PAREN,
  LTOKEN_LEFT_BRACE,
  LTOKEN_RIGHT_BRACE,
  LTOKEN_COMMA,
  LTOKEN_DOT,
  LTOKEN_MINUS,
  LTOKEN_PLUS,
  LTOKEN_SEMICOLON,
  LTOKEN_SLASH,
  LTOKEN_PERCENT,
  LTOKEN_STAR,
  LTOKEN_NOT_EQUAL,
  LTOKEN_EQUAL,
  LTOKEN_EQUAL_EQUAL,
  LTOKEN_GREATER,
  LTOKEN_GREATER_GREATER,
  LTOKEN_GREATER_EQUAL,
  LTOKEN_LESS,
  LTOKEN_LESS_LESS,
  LTOKEN_LESS_EQUAL,
  LTOKEN_IDENTIFIER,
  LTOKEN_STRING,
  LTOKEN_NUMBER,
  LTOKEN_CLASS,
  LTOKEN_ELSE,
  LTOKEN_FALSE,
  LTOKEN_FUN,
  LTOKEN_FOR,
  LTOKEN_IF,
  LTOKEN_NIL,
  LTOKEN_AND,
  LTOKEN_OR,
  LTOKEN_NOT,
  LTOKEN_BIT_AND,
  LTOKEN_BIT_OR,
  LTOKEN_BIT_XOR,
  LTOKEN_BIT_NOT,
  LTOKEN_RETURN,
  LTOKEN_CONTINUE,
  LTOKEN_BREAK,
  LTOKEN_SUPER,
  LTOKEN_THIS,
  LTOKEN_TRUE,
  LTOKEN_VAR,
  LTOKEN_WHILE,
  LTOKEN_COMMENT,
  LTOKEN_PLUS_EQUAL,
  LTOKEN_MINUS_EQUAL,
  LTOKEN_STAR_EQUAL,
  LTOKEN_SLASH_EQUAL,
  LTOKEN_EOF,
  /*not real, for expr only*/
  LTOKEN_SUB,
  LTOKEN_SET,
  LTOKEN_GET,
  LTOKEN_GROUP,
  LTOKEN_CALL_FUNC,
  LTOKEN_CALL_CONSTRUCTOR,
  /*not real, for stmt only*/
  LTOKEN_METHOD,
  LTOKEN_CONSTRUCTOR,
  LTOKEN_EXPR,
  LTOKEN_BLOCK,
} ltoken_type_t;

typedef struct _ltoken_t {
  ltoken_type_t type;
  const char* lexeme;
  lvalue_t literal;
  uint32_t line;
  uint32_t lexeme_len;
} ltoken_t;

ltoken_t* ltoken_init(ltoken_t* token, ltoken_type_t type, uint32_t line);
ltoken_t* ltoken_init_id(ltoken_t* token, const char* lexeme, uint32_t lexeme_len, uint32_t line);
ltoken_t* ltoken_init_str(ltoken_t* token, const char* lexeme, uint32_t lexeme_len, uint32_t line);
ltoken_t* ltoken_init_number(ltoken_t* token, const char* lexeme, uint32_t line);

#define LSTR_TOKEN_THIS "this"
#define LSTR_TOKEN_SUPER "super"
#define LSTR_TOKEN_INIT "init"

END_C_DECLS

#endif /*LOX_TOKEN_H*/
