#include "base/lmem.h"
#include "base/lstr_utils.h"

char* lstrdup(const char* str) {
  return lstrndup(str, -1);
}

char* lstrndup(const char* str, int32_t len) {
  char* d = NULL;
  return_value_if_fail(str != NULL, NULL);
  len = len < 0 ? strlen(str) : len;

  d = LMEM_ALLOC(len + 1);
  return_value_if_fail(d != NULL, NULL);
  memcpy(d, str, len);
  d[len] = '\0';

  return d;
}

int32_t lstrcmp(const char* s1, const char* s2) {
  if (s1 == s2) {
    return 0;
  } else if (s1 == NULL || s2 == NULL) {
    return -1;
  } else {
    return strcmp(s1, s2);
  }
}

int32_t lstrncmp(const char* s1, const char* s2, uint32_t len) {
  if (s1 == s2) {
    return 0;
  } else if (s1 == NULL || s2 == NULL) {
    return -1;
  } else {
    return strncmp(s1, s2, len);
  }
}

char* lstrncpy(char* d, uint32_t d_len, const char* s, uint32_t s_len) {
  int32_t len = LMIN(d_len - 1, s_len);
  return_value_if_fail(d != NULL && s != NULL, NULL);
  if (len > 0) {
    memcpy(d, s, len);
    d[len] = '\0';
  } else {
    *d = '\0';
  }

  return d;
}

char* lstrset(char* d, const char* s) {
  uint32_t len = 0;
  char* new_str = NULL;
  return_value_if_fail(s != NULL, d);

  len = strlen(s);
  new_str = LMEM_REALLOC(d, (len + 1));
  return_value_if_fail(new_str != NULL, d);
  memcpy(new_str, s, len);
  new_str[len] = '\0';

  return new_str;
}

uint32_t lstrlen(const char* str) {
  return_value_if_fail(str != NULL, 0);

  return strlen(str);
}

uint32_t lstr_escape(char* str, uint32_t size) {
  uint32_t s = 0;
  uint32_t d = 0;
  char* p = str;
  bool_t escape = FALSE;
  return_value_if_fail(str != NULL, 0);

  for (s = 0; s < size; s++) {
    char c = p[s];

    if (escape) {
      if (c == 'r') {
        p[d] = '\r';
      } else if (c == 'n') {
        p[d] = '\n';
      } else if (c == '\'') {
        p[d] = '\'';
      } else if (c == '\"') {
        p[d] = '\"';
      } else if (c == 't') {
        p[d] = '\t';
      } else {
        p[d] = p[s];
      }
      d++;
      escape = FALSE;
    } else {
      if (p[s] == '\\') {
        escape = TRUE;
      } else {
        p[d] = p[s];
        d++;
      }
    }
  }
  p[d] = '\0';

  return d;
}
