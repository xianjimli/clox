#ifndef LOX_INTERPRETER_H
#define LOX_INTERPRETER_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "interpreter/lenv.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

struct _linterpreter_t {
  lobject_t* global;
  lparser_t parser;
  lstmt_t* root;
  lenv_t* env;
  lmap_t* locals;
  lstr_t* tname;

  uint32_t error;
  bool_t meet_break;
  bool_t meet_return;
  bool_t meet_continue;
};

linterpreter_t* linterpreter_create(lobject_t* global, const char* code, uint32_t size);
ret_t linterpreter_define_var(linterpreter_t* interpreter, const sized_str_t* name,
                              const lvalue_t* value);
ret_t linterpreter_define_object(linterpreter_t* interpreter, const char* name, lobject_t* obj);
ret_t linterpreter_get_var(linterpreter_t* interpreter, lexpr_t* expr, lvalue_t* value);
ret_t linterpreter_set_var(linterpreter_t* interpreter, lexpr_t* expr, const lvalue_t* value);

ret_t linterpreter_exec(linterpreter_t* interpreter, lvalue_t* result);
ret_t linterpreter_enter_block(linterpreter_t* interpreter);
ret_t linterpreter_leave_block(linterpreter_t* interpreter);
const char* linterpreter_to_name(linterpreter_t* interpreter, const sized_str_t* name);

ret_t linterpreter_destroy(linterpreter_t* interpreter);

END_C_DECLS

#endif /*LOX_INTERPRETER_H*/
