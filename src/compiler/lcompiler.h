#ifndef LOX_COMPILER_H
#define LOX_COMPILER_H

#include "base/lmap.h"
#include "base/lobject.h"
#include "parser/lparser.h"
#include "compiler/llocals.h"
#include "compiler/lobject_func.h"
#include "compiler/lcompiler_types.h"

BEGIN_C_DECLS

struct _lcompiler_t {
  lobject_func_t* func;
  lcompiler_t* enclosing;
  llocals_t* locals;
  int32_t scope_depth;
};

lcompiler_t* lcompiler_create(lcompiler_t* enclosing);

lobject_func_t* lcompiler_compile(lcompiler_t* compiler, lstmt_t* stmt);

ret_t lcompiler_destroy(lcompiler_t* compiler);

END_C_DECLS

#endif /*LOX_COMPILER_H*/
