#ifndef LOX_EXPR_EVAL_H
#define LOX_EXPR_EVAL_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

ret_t lexpr_eval(lexpr_t* expr, linterpreter_t* interpreter, lvalue_t* result);

END_C_DECLS

#endif /*LOX_EXPR_EVAL_H*/
