#ifndef LOX_DISASSEMBLER_H
#define LOX_DISASSEMBLER_H

#include "base/lstr.h"
#include "compiler/lobject_func.h"

BEGIN_C_DECLS

ret_t ldisassemble(lobject_func_t* func, lstr_t* result);

END_C_DECLS

#endif /*LOX_DISASSEMBLER_H*/
