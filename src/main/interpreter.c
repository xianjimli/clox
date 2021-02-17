#include "interpreter/lnative_ext.h"
#include "interpreter/linterpreter.h"
#include "parser/lstmt_to_string.h"

#include <sys/types.h>
#include <sys/stat.h>

static void lox_run(const char* code) {
  lvalue_t result;
  lobject_t* global = NULL;
  linterpreter_t* ip = NULL;
  lstr_t* str = lstr_create(100);

  global = LOBJECT(lmap_create());
  lnative_ext_register(global);
  ip = linterpreter_create(global, code, strlen(code));
  lstmt_to_string(ip->root, str);
  printf("=================================\n");
  printf("%s\n", str->str);
  printf("=================================\n");
  linterpreter_exec(ip, &result);
  linterpreter_destroy(ip);
  LOBJECT_UNREF(global);
  LOBJECT_UNREF(str);

  return;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s code|@filename\n", argv[0]);
    return 0;
  } else {
    char* code = argv[1];
    if (*code == '@') {
      struct stat st;
      const char* filename = code + 1;
      FILE* fp = fopen(filename, "rb");
      if (fp != NULL) {
        if (stat(filename, &st) == 0) {
          code = LMEM_ALLOC(st.st_size + 1);
          if (code != NULL) {
            fread(code, st.st_size, 1, fp);
            code[st.st_size] = '\0';
            lox_run(code);
            LMEM_FREE(code);
          }
        }
        fclose(fp);
      }
    } else {
      lox_run(code);
    }
  }

  return 0;
}
