#ifndef LOX_TYPES_DEF_H
#define LOX_TYPES_DEF_H

#include <math.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <stdio.h>

#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

BEGIN_C_DECLS

typedef enum _ret_t {
  RET_OK = 0,
  RET_OOM,
  RET_FAIL,
  RET_BAD_PARAMS,
  RET_NOT_FOUND,
  RET_NOT_IMPL
} ret_t;

typedef void* lfunction_t;

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#if defined(__GNUC__) && !defined(__cplusplus)
typedef _Bool bool_t;
#else
typedef uint8_t bool_t;
#endif

typedef struct _binary_data_t {
  uint32_t size;
  void* data;
} binary_data_t;

typedef struct _sized_str_t {
  uint32_t size;
  const char* str;
} sized_str_t;

#if defined(WIN32)
#include <windows.h>
#if defined(__GNUC__)
/*MINGW*/
#define log_debug(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, ##args)
#define log_info(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, ##args)
#define log_warn(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, ##args)
#define log_error(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, ##args)
#else
/*MSVC*/
#define log_debug(format, ...)                                             \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, __VA_ARGS__); \
  fflush(stdout)
#define log_info(format, ...)                                             \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, __VA_ARGS__); \
  fflush(stdout)
#define log_warn(format, ...)                                             \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, __VA_ARGS__); \
  fflush(stdout)
#define log_error(format, ...)                                             \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, __VA_ARGS__); \
  fflush(stdout)
#endif
#elif defined(HAS_STDIO) || defined(AWLOX_WEB)
#include <stdio.h>
#define log_debug(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_DEBUG) printf(format, ##args)
#define log_info(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_INFO) printf(format, ##args)
#define log_warn(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_WARN) printf(format, ##args)
#define log_error(format, args...) \
  if (log_get_log_level() <= LOG_LEVEL_ERROR) printf(format, ##args)
#else
#define log_debug(format, args, ...) (void)0
#define log_info(format, args, ...) (void)0
#define log_warn(format, args, ...) (void)0
#define log_error(format, args, ...) (void)0
#endif

#if defined(NDEBUG)
#define ENSURE(p) (void)(p)
#define goto_error_if_fail(p) \
  if (!(p)) {                 \
    goto error;               \
  }

#define return_if_fail(p) \
  if (!(p)) {             \
    return;               \
  }

#define break_if_fail(p) \
  if (!(p)) {            \
    break;               \
  }

#define return_value_if_fail(p, value) \
  if (!(p)) {                          \
    return (value);                    \
  }
#else
#define ENSURE(p) assert(p)
#define goto_error_if_fail(p)                                              \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    goto error;                                                            \
  }

#define break_if_fail(p)                                                   \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    break;                                                                 \
  }

#define return_if_fail(p)                                                  \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    return;                                                                \
  }

#define return_value_if_fail(p, value)                                     \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    return (value);                                                        \
  }

#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /*ARRAY_SIZE*/

#define LMIN(a, b) ((a) < (b) ? (a) : (b))
#define LMAX(a, b) ((a) > (b) ? (a) : (b))

struct _lexpr_t;
typedef struct _lexpr_t lexpr_t;

struct _lobject_t;
typedef struct _lobject_t lobject_t;

struct _lexprs_t;
typedef struct _lexprs_t lexprs_t;

struct _lstmt_t;
typedef struct _lstmt_t lstmt_t;

struct _lstmts_t;
typedef struct _lstmts_t lstmts_t;

typedef ret_t (*ldestroy_t)(void* data);
typedef int32_t (*lcompare_t)(const void* a, const char* b);

END_C_DECLS

#endif /*LOX_TYPES_DEF_H*/
