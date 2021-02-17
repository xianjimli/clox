#ifndef LOX_EXPR_GEN_H
#define LOX_EXPR_GEN_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "compiler/lcompiler_types.h"

BEGIN_C_DECLS

ret_t lexpr_gen(lexpr_t* expr, lcompiler_t* compiler);

END_C_DECLS

#endif /*LOX_EXPR_GEN_H*/
