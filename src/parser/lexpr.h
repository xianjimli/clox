#ifndef LOX_EXPR_H
#define LOX_EXPR_H

#include "base/lmem.h"
#include "base/lstr.h"
#include "parser/ltoken.h"
#include "parser/llexer.h"
#include "parser/lexprs.h"

BEGIN_C_DECLS

struct _lexpr_t {
  union {
    lvalue_t value;
    lexprs_t* args;
  } data;
#ifdef NDEBUG
  uint32_t type : 8;
#else
  ltoken_type_t type;
#endif /*NDEBUG*/
  uint32_t has_args : 1;
  uint32_t has_value : 1;
  uint32_t line : 16;
  uint32_t scope_level : 6;
};

lexpr_t* lexpr_create_id(ltoken_t* token);
lexpr_t* lexpr_create_number(ltoken_t* token);
lexpr_t* lexpr_create_bool(ltoken_t* token);
lexpr_t* lexpr_create_nil(void);
lexpr_t* lexpr_create_string(ltoken_t* token);
lexpr_t* lexpr_create_binary(ltoken_type_t type, lexpr_t* left, lexpr_t* right);
lexpr_t* lexpr_create_assign(lexpr_t* name, lexpr_t* right);
lexpr_t* lexpr_create_unary(ltoken_type_t type, lexpr_t* right);
lexpr_t* lexpr_create_call(ltoken_type_t type, lexprs_t* args);
lexpr_t* lexpr_create_get(lexpr_t* obj, lexpr_t* name);
lexpr_t* lexpr_create_set(lexpr_t* obj, lexpr_t* name, lexpr_t* value);
lexpr_t* lexpr_create_set(lexpr_t* obj, lexpr_t* name, lexpr_t* value);
lexpr_t* lexpr_create_group(lexpr_t* expr);
lexpr_t* lexpr_create_this(void);
lexpr_t* lexpr_create_super(void);
lexpr_t* lexpr_create_var(ltoken_t* name);

lvalue_t* lexpr_get_value(lexpr_t* expr);
const sized_str_t* lexpr_get_value_id(lexpr_t* expr);
const sized_str_t* lexpr_get_value_string(lexpr_t* expr);
lexpr_t* lexpr_get_arg(lexpr_t* expr, uint16_t index);

ret_t lexpr_to_string(lexpr_t* expr, lstr_t* str);

ret_t lexpr_destroy(lexpr_t* expr);

END_C_DECLS

#endif /*LOX_EXPR_H*/
