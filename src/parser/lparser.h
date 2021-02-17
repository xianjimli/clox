
#include "parser/llexer.h"
#include "parser/lexpr.h"
#include "parser/lstmt.h"

#ifndef LOX_PARSER_H
#define LOX_PARSER_H

BEGIN_C_DECLS

typedef struct _lparser_t {
  llexer_t lexer;
  uint32_t errors;
} lparser_t;

lparser_t* lparser_init(lparser_t* parser, const char* input);
lstmt_t* lparser_parse(lparser_t* parser);
ret_t lparser_deinit(lparser_t* parser);

/*public for tests*/

lexpr_t* lparser_parse_expr(lparser_t* parser);
lexpr_t* lparser_parse_primary(lparser_t* parser);
lexpr_t* lparser_parse_call(lparser_t* parser);
lexpr_t* lparser_parse_unary(lparser_t* parser);
lexpr_t* lparser_parse_factor(lparser_t* parser);
lexpr_t* lparser_parse_term(lparser_t* parser);
lexpr_t* lparser_parse_compare(lparser_t* parser);
lexpr_t* lparser_parse_equality(lparser_t* parser);
lexpr_t* lparser_parse_or(lparser_t* parser);
lexpr_t* lparser_parse_and(lparser_t* parser);
lexpr_t* lparser_parse_assignment(lparser_t* parser);

lstmt_t* lparser_parse_declaration(lparser_t* parser);
lstmt_t* lparser_parse_var(lparser_t* parser);
lstmt_t* lparser_parse_block(lparser_t* parser);
lstmt_t* lparser_parse_if(lparser_t* parser);
lstmt_t* lparser_parse_stmt(lparser_t* parser);
lstmt_t* lparser_parse_while(lparser_t* parser);
lstmt_t* lparser_parse_for(lparser_t* parser);
lstmt_t* lparser_parse_return(lparser_t* parser);
lstmt_t* lparser_parse_func(lparser_t* parser);
lstmt_t* lparser_parse_class(lparser_t* parser);

END_C_DECLS

#endif /*LOX_PARSER_H*/
