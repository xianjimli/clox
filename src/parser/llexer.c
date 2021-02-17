#include "parser/llexer.h"
#include "base/lstr_utils.h"

llexer_t* llexer_init(llexer_t* lexer, const char* input) {
  return_value_if_fail(lexer != NULL && input != NULL, NULL);
  memset(lexer, 0x00, sizeof(llexer_t));

  lexer->input = lstrdup(input);
  lexer->cursor = lexer->input;

  return lexer;
}

static char llexer_get_char(llexer_t* lexer) {
  char c = 0;
  if (lexer->unget_char) {
    c = lexer->unget_char;
    lexer->unget_char = '\0';
  } else if (*lexer->cursor) {
    c = *lexer->cursor++;
    if (c == '\n') {
      lexer->line++;
    }
  } else {
    return '\0';
  }

  return c;
}

static ret_t llexer_unget_char(llexer_t* lexer, char c) {
  lexer->unget_char = c;
  return RET_OK;
}

static char llexer_skip_space_and_get_char(llexer_t* lexer) {
  char c = llexer_get_char(lexer);

  if (!c) {
    return c;
  }

  while (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
    c = llexer_get_char(lexer);
  }
  return c;
}

static bool_t llexer_match_chars(llexer_t* lexer, const char* expected) {
  char c = llexer_get_char(lexer);
  if (c && strchr(expected, c) != NULL) {
    return TRUE;
  } else {
    llexer_unget_char(lexer, c);
    return FALSE;
  }
}

static bool_t llexer_match_char(llexer_t* lexer, char expected) {
  char c = llexer_get_char(lexer);
  if (c == expected) {
    return TRUE;
  } else {
    llexer_unget_char(lexer, c);
    return FALSE;
  }
}

static ltoken_t* llexer_init_token(llexer_t* lexer, ltoken_type_t type) {
  return ltoken_init(&(lexer->token), type, lexer->line);
}

static ltoken_t* llexer_parse_line_comment(llexer_t* lexer) {
  char c = llexer_get_char(lexer);
  while (c && c != '\n') {
    c = llexer_get_char(lexer);
  }

  return llexer_init_token(lexer, LTOKEN_COMMENT);
}

static ltoken_t* llexer_parse_block_comment(llexer_t* lexer) {
  char c = llexer_get_char(lexer);
  while (c) {
    c = llexer_get_char(lexer);
    if (c == '*' && llexer_match_char(lexer, '/')) {
      break;
    }
  }
  return llexer_init_token(lexer, LTOKEN_COMMENT);
}

static ltoken_t* llexer_parse_string(llexer_t* lexer, char quot, bool_t enable_escape) {
  ltoken_t* t = NULL;
  const char* start = lexer->cursor;
  char c = llexer_get_char(lexer);

  while (c && c != quot) {
    c = llexer_get_char(lexer);
  }

  t = llexer_init_token(lexer, LTOKEN_STRING);
  t->lexeme = start;
  t->lexeme_len = lexer->cursor - start - 1;

  if (enable_escape) {
    t->lexeme_len = lstr_escape((char*)(t->lexeme), t->lexeme_len);
  }

  return t;
}

static ltoken_t* llexer_to_number(llexer_t* lexer, const char* str, uint32_t base) {
  uint64_t v = 0;
  ltoken_t* t = llexer_init_token(lexer, LTOKEN_NUMBER);

  if (base == 10 && strchr(str, '.') != NULL) {
    lvalue_set_double(&(t->literal), atof(str));
    return t;
  }

  v = strtoll(str, NULL, base);
  if (v >= UINT32_MAX) {
    lvalue_set_uint64(&(t->literal), v);
  } else {
    lvalue_set_uint32(&(t->literal), v);
  }

  return t;
}

static ltoken_t* llexer_parse_number(llexer_t* lexer) {
  const char* start = lexer->cursor - 1;
  char c = llexer_get_char(lexer);
  if (llexer_match_chars(lexer, "xX")) {
    start = lexer->cursor;
    c = llexer_get_char(lexer);
    while (c && isxdigit(c)) {
      c = llexer_get_char(lexer);
    }
    llexer_unget_char(lexer, c);

    return llexer_to_number(lexer, start, 16);
  } else if (llexer_match_chars(lexer, "bB")) {
    start = lexer->cursor;
    c = llexer_get_char(lexer);
    while (c && (c == '0' || c == '1')) {
      c = llexer_get_char(lexer);
    }
    llexer_unget_char(lexer, c);

    return llexer_to_number(lexer, start, 2);
  } else {
    while (c && (c == '.' || isdigit(c))) {
      c = llexer_get_char(lexer);
    }
    llexer_unget_char(lexer, c);

    return llexer_to_number(lexer, start, 10);
  }
}

#define MATCH(KEY, TYPE)                                                     \
  if (*p == *KEY && len == (sizeof(KEY) - 1) && strncmp(p, KEY, len) == 0) { \
    t->type = TYPE;
#define ELSE_MATCH(KEY, TYPE)                                                     \
  }                                                                               \
  else if (*p == *KEY && len == (sizeof(KEY) - 1) && strncmp(p, KEY, len) == 0) { \
    t->type = TYPE;
#define END_MATCH() \
  }                 \
  ;
static ltoken_t* llexer_to_keyword(llexer_t* lexer, ltoken_t* t) {
  const char* p = t->lexeme;
  uint32_t len = t->lexeme_len;

  MATCH("class", LTOKEN_CLASS);
  ELSE_MATCH("else", LTOKEN_ELSE);
  ELSE_MATCH("if", LTOKEN_IF);
  ELSE_MATCH("while", LTOKEN_WHILE);
  ELSE_MATCH("true", LTOKEN_TRUE);
  ELSE_MATCH("false", LTOKEN_FALSE);
  ELSE_MATCH("nil", LTOKEN_NIL);
  ELSE_MATCH("return", LTOKEN_RETURN);
  ELSE_MATCH("continue", LTOKEN_CONTINUE);
  ELSE_MATCH("break", LTOKEN_BREAK);
  ELSE_MATCH(LSTR_TOKEN_THIS, LTOKEN_THIS);
  ELSE_MATCH(LSTR_TOKEN_SUPER, LTOKEN_SUPER);
  ELSE_MATCH("var", LTOKEN_VAR);
  ELSE_MATCH("fun", LTOKEN_FUN);
  ELSE_MATCH("for", LTOKEN_FOR);
  END_MATCH();

  return t;
}

static ltoken_t* llexer_parse_id(llexer_t* lexer) {
  ltoken_t* t = NULL;
  const char* start = lexer->cursor - 1;
  char c = llexer_get_char(lexer);

  while (c && (isalpha(c) || isdigit(c) || c == '_')) {
    c = llexer_get_char(lexer);
  }
  llexer_unget_char(lexer, c);

  t = llexer_init_token(lexer, LTOKEN_IDENTIFIER);
  t->lexeme = start;
  if (c) {
    t->lexeme_len = lexer->cursor - start - 1;
  } else {
    t->lexeme_len = lexer->cursor - start;
  }

  return llexer_to_keyword(lexer, t);
}

static ret_t llexer_unexpected_char(llexer_t* lexer, char c) {
  log_debug("unexpected char %c at %u\n", c, lexer->line);
  return RET_OK;
}

static ltoken_t* llexer_next(llexer_t* lexer) {
  char c = llexer_skip_space_and_get_char(lexer);
  switch (c) {
    case '\0':
      return llexer_init_token(lexer, LTOKEN_EOF);
    case '(':
      return llexer_init_token(lexer, LTOKEN_LEFT_PAREN);
    case ')':
      return llexer_init_token(lexer, LTOKEN_RIGHT_PAREN);
    case '{':
      return llexer_init_token(lexer, LTOKEN_LEFT_BRACE);
    case '}':
      return llexer_init_token(lexer, LTOKEN_RIGHT_BRACE);
    case ',':
      return llexer_init_token(lexer, LTOKEN_COMMA);
    case '.':
      return llexer_init_token(lexer, LTOKEN_DOT);
    case '-':
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_MINUS_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_MINUS);
      }
    case '+':
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_PLUS_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_PLUS);
      }
    case ';':
      return llexer_init_token(lexer, LTOKEN_SEMICOLON);
    case '*':
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_STAR_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_STAR);
      }
    case '%':
      return llexer_init_token(lexer, LTOKEN_PERCENT);
    case '!': {
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_NOT_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_NOT);
      }
      break;
    }
    case '^':
      return llexer_init_token(lexer, LTOKEN_BIT_XOR);
    case '~':
      return llexer_init_token(lexer, LTOKEN_BIT_NOT);
    case '|': {
      if (llexer_match_char(lexer, '|')) {
        return llexer_init_token(lexer, LTOKEN_OR);
      } else {
        return llexer_init_token(lexer, LTOKEN_BIT_OR);
      }
      break;
    }
    case '&': {
      if (llexer_match_char(lexer, '&')) {
        return llexer_init_token(lexer, LTOKEN_AND);
      } else {
        return llexer_init_token(lexer, LTOKEN_BIT_AND);
      }
      break;
    }
    case '=': {
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_EQUAL_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_EQUAL);
      }
      break;
    }
    case '<': {
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_LESS_EQUAL);
      } else if (llexer_match_char(lexer, '<')) {
        return llexer_init_token(lexer, LTOKEN_LESS_LESS);
      } else {
        return llexer_init_token(lexer, LTOKEN_LESS);
      }
      break;
    }
    case '>': {
      if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_GREATER_EQUAL);
      } else if (llexer_match_char(lexer, '>')) {
        return llexer_init_token(lexer, LTOKEN_GREATER_GREATER);
      } else {
        return llexer_init_token(lexer, LTOKEN_GREATER);
      }
      break;
    }
    case '/':
      if (llexer_match_char(lexer, '/')) {
        return llexer_parse_line_comment(lexer);
      } else if (llexer_match_char(lexer, '*')) {
        return llexer_parse_block_comment(lexer);
      } else if (llexer_match_char(lexer, '=')) {
        return llexer_init_token(lexer, LTOKEN_SLASH_EQUAL);
      } else {
        return llexer_init_token(lexer, LTOKEN_SLASH);
      }
    case '\'':
      return llexer_parse_string(lexer, c, TRUE);
    case '"':
      return llexer_parse_string(lexer, c, TRUE);
    case '`':
      return llexer_parse_string(lexer, c, FALSE);
    default:
      if (isdigit(c)) {
        llexer_unget_char(lexer, c);
        return llexer_parse_number(lexer);
      } else if (isalpha(c)) {
        llexer_unget_char(lexer, c);
        return llexer_parse_id(lexer);
      } else {
        llexer_unexpected_char(lexer, c);
        return NULL;
      }
      break;
  }

  return NULL;
}

ltoken_t* llexer_get(llexer_t* lexer) {
  return_value_if_fail(lexer != NULL, NULL);
  if (lexer->unget_token) {
    lexer->unget_token = FALSE;
    return &(lexer->token);
  }
#if 0
  do {
   t = llexer_next(lexer);
  }while(t->type == LTOKEN_COMMENT);
#endif

  return llexer_next(lexer);
}

ret_t llexer_unget(llexer_t* lexer) {
  return_value_if_fail(lexer != NULL, RET_BAD_PARAMS);
  lexer->unget_token = TRUE;

  return RET_OK;
}

bool_t llexer_match(llexer_t* lexer, ltoken_type_t type) {
  ltoken_t* t = NULL;
  return_value_if_fail(lexer != NULL, FALSE);

  t = llexer_get(lexer);
  if (t->type == type) {
    return TRUE;
  } else {
    llexer_unget(lexer);
    return FALSE;
  }
}

ret_t llexer_deinit(llexer_t* lexer) {
  return_value_if_fail(lexer != NULL, RET_BAD_PARAMS);
  LMEM_FREE(lexer->input);

  return RET_OK;
}
