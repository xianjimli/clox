#ifndef LOX_STMT_H
#define LOX_STMT_H

#include "base/lmem.h"
#include "base/lstr.h"
#include "parser/ltoken.h"
#include "parser/llexer.h"
#include "parser/lexpr.h"
#include "parser/lparams.h"
#include "parser/lstmts.h"

BEGIN_C_DECLS

struct _lstmt_t;
typedef struct _lstmt_t lstmt_t;

typedef struct _lstmt_block_t {
  lstmts_t* stmts;
} lstmt_block_t;

typedef struct _lstmt_class_t {
  sized_str_t name;
  lexpr_t* super;
  lstmts_t* funcs;
} lstmt_class_t;

typedef struct _lstmt_expr_t {
  lexpr_t* expr;
} lstmt_expr_t;

typedef struct _lstmt_func_t {
  sized_str_t name;
  lparams_t* params;
  lstmt_t* stmt_body;
} lstmt_func_t;

typedef struct _lstmt_if_t {
  lexpr_t* condition;
  lstmt_t* stmt_then;
  lstmt_t* stmt_else;
} lstmt_if_t;

typedef struct _lstmt_while_t {
  lexpr_t* condition;
  lstmt_t* stmt_body;
} lstmt_while_t;

typedef struct _lstmt_for_t {
  lstmt_t* initializer;
  lexpr_t* condition;
  lexpr_t* increment;
  lstmt_t* stmt_body;
} lstmt_for_t;

typedef struct _lstmt_return_t {
  lexpr_t* expr;
} lstmt_return_t;

typedef struct _lstmt_var_t {
  sized_str_t name;
  lexpr_t* initializer;
} lstmt_var_t;

struct _lstmt_t {
  union {
    lstmt_block_t stmt_block;
    lstmt_class_t stmt_class;
    lstmt_expr_t stmt_expr;
    lstmt_func_t stmt_func;
    lstmt_if_t stmt_if;
    lstmt_while_t stmt_while;
    lstmt_for_t stmt_for;
    lstmt_var_t stmt_var;
    lstmt_return_t stmt_return;
  } data;
#ifdef NDEBUG
  uint32_t type : 8;
#else
  ltoken_type_t type;
#endif /*NDEBUG*/
  uint32_t line : 16;
};

lstmt_t* lstmt_create_block(lstmts_t* stmts);
lstmt_t* lstmt_create_expr(lexpr_t* expr);
lstmt_t* lstmt_create_return(lexpr_t* expr);
lstmt_t* lstmt_create_continue(void);
lstmt_t* lstmt_create_break(void);
lstmt_t* lstmt_create_var(ltoken_t* name, lexpr_t* initializer);
lstmt_t* lstmt_create_while(lexpr_t* condition, lstmt_t* stmt_body);
lstmt_t* lstmt_create_for(lstmt_t* initializer, lexpr_t* condition, lexpr_t* increment,
                          lstmt_t* stmt_body);
lstmt_t* lstmt_create_if(lexpr_t* condition, lstmt_t* stmt_then, lstmt_t* stmt_else);
lstmt_t* lstmt_create_func(ltoken_t* name, lparams_t* params, lstmt_t* stmt_body);
lstmt_t* lstmt_create_class(ltoken_t* name, ltoken_t* super_name, lstmts_t* funcs);

lstmt_t* lstmt_find_func(lstmt_t* clazz, const char* name);

ret_t lstmt_destroy(lstmt_t* stmt);

END_C_DECLS

#endif /*LOX_STMT_H*/
