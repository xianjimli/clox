#include "interpreter/linterpreter.h"
#include "interpreter/lcallable_native.h"

static ret_t lext_assert(lcallable_t* callable, linterpreter_t* interpreter, lvalue_t* argv,
                         uint32_t argc, lvalue_t* result) {
  if (argc == 1) {
    assert(lvalue_to_bool(argv));
  }

  return RET_OK;
}

static ret_t lext_print(lcallable_t* callable, linterpreter_t* interpreter, lvalue_t* argv,
                        uint32_t argc, lvalue_t* result) {
  uint32_t i = 0;
  if (argc > 0) {
    lstr_t* str = lstr_create(100);
    for (i = 0; i < argc; i++) {
      lvalue_t* iter = argv + i;
      lstr_append_value(str, iter);
    }
    printf("%s", str->str);
    LOBJECT_UNREF(str);
  }
  lvalue_set_bool(result, argc);

  return RET_OK;
}

ret_t lnative_ext_register_one(lobject_t* global, const char* name, lcallable_call_t call) {
  lcallable_t* obj = lcallable_native_create(call);
  lobject_set_object(global, name, LOBJECT(obj));
  LOBJECT_UNREF(obj);

  return RET_OK;
}

ret_t lnative_ext_register(lobject_t* global) {
  lnative_ext_register_one(global, "print", lext_print);
  lnative_ext_register_one(global, "assert", lext_assert);

  return RET_OK;
}
