#include "gtest/gtest.h"
#include "base/lstr_utils.h"

TEST(LStrUtils, lstrdup) {
  char* s = lstrdup(NULL);
  ASSERT_EQ(s == NULL, true);
  s = lstrdup("abc");
  ASSERT_STREQ(s, "abc");
  LMEM_FREE(s);
}

TEST(LStrUtils, lstrndup) {
  char* s = lstrndup(NULL, 1);
  ASSERT_EQ(s == NULL, true);
  s = lstrndup("abc", -1);
  ASSERT_STREQ(s, "abc");
  LMEM_FREE(s);

  s = lstrndup("abc", 1);
  ASSERT_STREQ(s, "a");
  LMEM_FREE(s);
}

TEST(LStrUtils, lstrcmp) {
  char str[32];
  strcpy(str, "abc");
  ASSERT_EQ(lstrcmp(NULL, NULL), 0);
  ASSERT_EQ(lstrcmp("a", "a"), 0);
  ASSERT_EQ(lstrcmp(str, "abc"), 0);
  ASSERT_NE(lstrcmp("a", "abc"), 0);
}

TEST(LStrUtils, lstrncmp) {
  char str[32];
  strcpy(str, "abc");
  ASSERT_EQ(lstrncmp(NULL, NULL, 1), 0);
  ASSERT_EQ(lstrncmp("a", "a", 1), 0);
  ASSERT_EQ(lstrncmp(str, "abc", 1), 0);
  ASSERT_EQ(lstrncmp(str, "abc", 2), 0);
  ASSERT_EQ(lstrncmp(str, "abc", 3), 0);
  ASSERT_EQ(lstrncmp(str, "abc", 4), 0);
  ASSERT_NE(lstrncmp("a", "abc", 3), 0);
}

TEST(LStrUtils, lstrncpy) {
  char d[4];
  ASSERT_STREQ(lstrncpy(d, sizeof(d), "abcde", 1), "a");
  ASSERT_STREQ(lstrncpy(d, sizeof(d), "abcde", 2), "ab");
  ASSERT_STREQ(lstrncpy(d, sizeof(d), "abcde", 4), "abc");
  ASSERT_STREQ(lstrncpy(d, sizeof(d), "abcde", 5), "abc");
}

TEST(LStrUtils, lstrset) {
  char* d = lstrdup("abc");
  d = lstrset(d, "1");
  ASSERT_STREQ(d, "1");

  d = lstrset(d, "12");
  ASSERT_STREQ(d, "12");

  d = lstrset(d, "123");
  ASSERT_STREQ(d, "123");

  d = lstrset(d, "123abc");
  ASSERT_STREQ(d, "123abc");
  LMEM_FREE(d);
}

TEST(LStrUtils, EQ) {
  ASSERT_EQ(LSTR_EQ("a", "a"), true);
  ASSERT_EQ(LSTR_EQ(NULL, NULL), true);
  ASSERT_EQ(LSTR_EQ(NULL, "a"), false);
  ASSERT_EQ(LSTR_EQ("a", "ab"), false);
  ASSERT_EQ(LSTR_EQ("a", "b"), false);
}

TEST(LStrUtils, escape) {
  char str[64];
#if 0
  strcpy(str, "abc");
  ASSERT_EQ(lstr_escape(str, strlen(str)), 3u);
  ASSERT_STREQ(str, "abc");
#endif
  strcpy(str, "\\r\\n");
  ASSERT_EQ(lstr_escape(str, strlen(str)), 2u);
  ASSERT_STREQ(str, "\r\n");

  strcpy(str, "\\r\\n\\t");
  ASSERT_EQ(lstr_escape(str, strlen(str)), 3u);
  ASSERT_STREQ(str, "\r\n\t");

  strcpy(str, "\\r\\n\\\"");
  ASSERT_EQ(lstr_escape(str, strlen(str)), 3u);
  ASSERT_STREQ(str, "\r\n\"");
}
