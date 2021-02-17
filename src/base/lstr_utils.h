#include "base/lmem.h"

#ifndef LSTR_UTILS_H
#define LSTR_UTILS_H

BEGIN_C_DECLS

char* lstrdup(const char* str);
uint32_t lstrlen(const char* str);
char* lstrndup(const char* str, int32_t len);
int32_t lstrcmp(const char* s1, const char* s2);
int32_t lstrncmp(const char* s1, const char* s2, uint32_t len);
char* lstrncpy(char* d, uint32_t d_len, const char* s, uint32_t s_len);
char* lstrset(char* d, const char* s);
uint32_t lstr_escape(char* str, uint32_t size);

#define LSTR_EQ(a, b) (lstrcmp(a, b) == 0)

END_C_DECLS

#endif /*LSTR_UTILS_H*/
