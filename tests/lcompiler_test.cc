
#include "gtest/gtest.h"
#include "compiler/lcompiler.h"
#include "compiler/ldisassembler.h"

static void test_compiler(const char* input, const char* expected) {
  lparser_t p;
  lstmt_t* stmt = NULL;
  lstr_t* str = lstr_create(100);
  lcompiler_t* compiler = lcompiler_create(NULL);
  lparser_init(&p, input);
  stmt = lparser_parse(&p);
  lobject_func_t* func = lcompiler_compile(compiler, stmt);

  ldisassemble(func, str);
  ASSERT_STREQ(str->str, expected);
  lstmt_destroy(stmt);
  lparser_deinit(&p);
  lcompiler_destroy(compiler);
  LOBJECT_UNREF(str);
}

TEST(LCompiler, basic) {
  test_compiler("1+2",
                "0000 OP_CONSTANT     1\n0002 OP_CONSTANT     2\n0004 OP_ADD\n0005 OP_POP\n");
}
