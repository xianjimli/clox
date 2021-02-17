
#include "compiler/lcompiler.h"
#include "compiler/lstmt_gen.h"

lcompiler_t* lcompiler_create(lcompiler_t* enclosing) {
  lcompiler_t* compiler = LMEM_NEW(lcompiler_t);
  return_value_if_fail(compiler != NULL, NULL);
  compiler->enclosing = enclosing;
  compiler->locals = llocals_create(10);

  return compiler;
}

lobject_func_t* lcompiler_compile(lcompiler_t* compiler, lstmt_t* stmt) {
  return_value_if_fail(compiler != NULL && stmt != NULL, NULL);
  compiler->func = lobject_func_create("<noname>", 0);

  lstmt_gen(stmt, compiler);

  return compiler->func;
}

ret_t lcompiler_destroy(lcompiler_t* compiler) {
  return_value_if_fail(compiler != NULL, RET_BAD_PARAMS);
  llocals_destroy(compiler->locals);
  LMEM_FREE(compiler);

  return RET_OK;
}
