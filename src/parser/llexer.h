#ifndef LOX_LEXER_H
#define LOX_LEXER_H

#include "parser/ltoken.h"

BEGIN_C_DECLS

typedef struct _llexer_t {
  char* input;
  char* cursor;
  uint32_t line;

  char unget_char;
  bool_t unget_token;

  ltoken_t token;
} llexer_t;

llexer_t* llexer_init(llexer_t* lexer, const char* input);
bool_t llexer_match(llexer_t* lexer, ltoken_type_t type);
ltoken_t* llexer_get(llexer_t* lexer);
ret_t llexer_unget(llexer_t* lexer);
ret_t llexer_deinit(llexer_t* lexer);

END_C_DECLS

#endif /*LOX_LEXER_H*/
