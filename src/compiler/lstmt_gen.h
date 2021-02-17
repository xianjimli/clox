#ifndef LOX_STMT_GEN_H
#define LOX_STMT_GEN_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "compiler/lcompiler_types.h"

BEGIN_C_DECLS

ret_t lstmt_gen(lstmt_t* stmt, lcompiler_t* compiler);

END_C_DECLS

#endif /*LOX_STMT_GEN_H*/
