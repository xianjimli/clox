#ifndef LOX_STMT_EVAL_H
#define LOX_STMT_EVAL_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

ret_t lstmt_eval(lstmt_t* stmt, linterpreter_t* interpreter, lvalue_t* result);

END_C_DECLS

#endif /*LOX_STMT_EVAL_H*/
