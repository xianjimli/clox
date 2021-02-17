#ifndef LOX_NATIVE_EXT_H
#define LOX_NATIVE_EXT_H

#include "interpreter/lcallable.h"
#include "interpreter/linterpreter_types.h"

BEGIN_C_DECLS

ret_t lnative_ext_register(lobject_t* global);

END_C_DECLS

#endif /*LOX_NATIVE_EXT_H*/
