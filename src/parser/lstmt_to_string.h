#ifndef LOX_STMT_TO_STRING_H
#define LOX_STMT_TO_STRING_H

#include "parser/lstmt.h"

BEGIN_C_DECLS

ret_t lstmt_to_string(lstmt_t* stmt, lstr_t* str);

END_C_DECLS

#endif /*LOX_STMT_TO_STRING_H*/
