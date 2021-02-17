
#ifndef LOX_VALUE_OPS_H
#define LOX_VALUE_OPS_H

#include "base/lmem.h"
#include "base/lstr.h"
#include "base/lvalue.h"

BEGIN_C_DECLS

static inline ret_t lvalue_add(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == LVALUE_TYPE_STRING || right->type == LVALUE_TYPE_STRING ||
      left->type == LVALUE_TYPE_SIZED_STRING || right->type == LVALUE_TYPE_SIZED_STRING) {
    lstr_t* str = lstr_create(100);
    lstr_append_value(str, left);
    lstr_append_value(str, right);
    lvalue_set_string(result, str->str);
    result->free_handle = TRUE;
    str->str = NULL;
    LOBJECT_UNREF(str);
    return RET_OK;
  }

  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 + right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 + right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 + right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 + right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 + right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 + right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 + right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 + right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_float(result, left->v.vfloat + right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_double(result, left->v.vdouble + right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_double(result, lvalue_to_double(left) + lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_double(result, lvalue_to_double(left) + lvalue_to_double(right));
  }
  return RET_OK;
}

static inline ret_t lvalue_sub(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 - right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 - right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 - right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 - right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 - right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 - right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 - right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 - right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_double(result, lvalue_double(left) - lvalue_double(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_double(result, left->v.vdouble - right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_double(result, lvalue_to_double(left) - lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_double(result, lvalue_to_double(left) - lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_mul(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 * right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 * right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 * right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 * right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 * right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 * right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 * right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 * right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_double(result, lvalue_double(left) * lvalue_double(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_double(result, left->v.vdouble * right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_double(result, lvalue_to_double(left) * lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_double(result, lvalue_to_double(left) * lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_div(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 / right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 / right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 / right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 / right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 / right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 / right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 / right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 / right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_double(result, lvalue_double(left) / lvalue_double(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_double(result, left->v.vdouble / right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_double(result, lvalue_to_double(left) / lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_double(result, lvalue_to_double(left) / lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_mod(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 % right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 % right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 % right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 % right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 % right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 % right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 % right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 % right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_int32(result, lvalue_int32(left) % lvalue_int32(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, left->v.vint32 % right->v.vint32);
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) % lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) % lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_bit_and(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 & right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 & right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 & right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 & right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 & right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 & right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 & right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 & right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT:
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, lvalue_int32(left) & lvalue_int32(right));
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) & lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) & lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_bit_or(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 | right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 | right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 | right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 | right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 | right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 | right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 | right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 | right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT:
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, lvalue_int32(left) | lvalue_int32(right));
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) | lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) | lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_bit_xor(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 ^ right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 ^ right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 ^ right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 ^ right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 ^ right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 ^ right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 ^ right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 ^ right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT:
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, lvalue_int32(left) ^ lvalue_int32(right));
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) ^ lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) ^ lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_lshift(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 << right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 << right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 << right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 << right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 << right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 << right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 << right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 << right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT:
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, lvalue_int32(left) << lvalue_int32(right));
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) << lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) << lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_rshift(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8 >> right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8 >> right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16 >> right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16 >> right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32 >> right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32 >> right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64 >> right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64 >> right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT:
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_int32(result, lvalue_int32(left) >> lvalue_int32(right));
        break;
      }
      default: {
        lvalue_set_int32(result, lvalue_to_int32(left) >> lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_int32(result, lvalue_to_int32(left) >> lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_and(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 && right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 && right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 && right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 && right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 && right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 && right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 && right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 && right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat && right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble && right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_int32(left) && lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_int32(left) && lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_or(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 || right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 || right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 || right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 || right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 || right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 || right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 || right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 || right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat || right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble || right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_int32(left) || lvalue_to_int32(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_int32(left) || lvalue_to_int32(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_eq(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 == right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 == right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 == right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 == right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 == right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 == right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 == right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 == right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat == right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble == right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) == lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) == lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_not_eq(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 != right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 != right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 != right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 != right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 != right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 != right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 != right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 != right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat != right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble != right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) != lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) != lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_less(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 < right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 < right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 < right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 < right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 < right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 < right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 < right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 < right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat < right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble < right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) < lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) < lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_less_eq(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 <= right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 <= right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 <= right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 <= right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 <= right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 <= right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 <= right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 <= right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat <= right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble <= right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) <= lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) <= lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_great(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 > right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 > right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 > right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 > right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 > right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 > right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 > right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 > right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat > right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble > right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) > lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) > lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_great_eq(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8 >= right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8 >= right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16 >= right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16 >= right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32 >= right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32 >= right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64 >= right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64 >= right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, left->v.vfloat >= right->v.vfloat);
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.vdouble >= right->v.vdouble);
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_double(left) >= lvalue_to_double(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_double(left) >= lvalue_to_double(right));
  }

  return RET_OK;
}

static inline ret_t lvalue_minus(const lvalue_t* left, lvalue_t* result) {
  switch (left->type) {
    case LVALUE_TYPE_INT8: {
      lvalue_set_int16(result, -left->v.vint8);
      break;
    }
    case LVALUE_TYPE_UINT8: {
      lvalue_set_int16(result, -left->v.vuint8);
      break;
    }
    case LVALUE_TYPE_INT16: {
      lvalue_set_int32(result, -left->v.vint16);
      break;
    }
    case LVALUE_TYPE_UINT16: {
      lvalue_set_int32(result, -left->v.vuint16);
      break;
    }
    case LVALUE_TYPE_INT32: {
      lvalue_set_int32(result, -left->v.vint32);
      break;
    }
    case LVALUE_TYPE_UINT32: {
      lvalue_set_uint32(result, -left->v.vuint32);
      break;
    }
    case LVALUE_TYPE_INT64: {
      lvalue_set_int64(result, -left->v.vint64);
      break;
    }
    case LVALUE_TYPE_UINT64: {
      lvalue_set_uint64(result, -left->v.vuint64);
      break;
    }
    case LVALUE_TYPE_FLOAT: {
      lvalue_set_float(result, -left->v.vfloat);
      break;
    }
    case LVALUE_TYPE_DOUBLE: {
      lvalue_set_double(result, -left->v.vdouble);
      break;
    }
    default: {
      lvalue_set_double(result, -lvalue_to_double(left));
      break;
    }
  }
  return RET_OK;
}

static inline ret_t lvalue_bit_not(const lvalue_t* left, lvalue_t* result) {
  switch (left->type) {
    case LVALUE_TYPE_INT8: {
      lvalue_set_int16(result, ~left->v.vint8);
      break;
    }
    case LVALUE_TYPE_UINT8: {
      lvalue_set_int16(result, ~left->v.vuint8);
      break;
    }
    case LVALUE_TYPE_INT16: {
      lvalue_set_int32(result, ~left->v.vint16);
      break;
    }
    case LVALUE_TYPE_UINT16: {
      lvalue_set_int32(result, ~left->v.vuint16);
      break;
    }
    case LVALUE_TYPE_INT32: {
      lvalue_set_int32(result, ~left->v.vint32);
      break;
    }
    case LVALUE_TYPE_UINT32: {
      lvalue_set_uint32(result, ~left->v.vuint32);
      break;
    }
    case LVALUE_TYPE_INT64: {
      lvalue_set_int64(result, ~left->v.vint64);
      break;
    }
    case LVALUE_TYPE_UINT64: {
      lvalue_set_uint64(result, ~left->v.vuint64);
      break;
    }
    case LVALUE_TYPE_FLOAT:
    case LVALUE_TYPE_DOUBLE:
    default: {
      lvalue_set_uint32(result, ~lvalue_to_int(left));
      break;
    }
  }
  return RET_OK;
}

static inline ret_t lvalue_not(const lvalue_t* left, lvalue_t* result) {
  switch (left->type) {
    case LVALUE_TYPE_BOOL: {
      lvalue_set_bool(result, !left->v.vbool);
      break;
    }
    case LVALUE_TYPE_INT8: {
      lvalue_set_bool(result, !left->v.vint8);
      break;
    }
    case LVALUE_TYPE_UINT8: {
      lvalue_set_bool(result, !left->v.vuint8);
      break;
    }
    case LVALUE_TYPE_INT16: {
      lvalue_set_bool(result, !left->v.vint16);
      break;
    }
    case LVALUE_TYPE_UINT16: {
      lvalue_set_bool(result, !left->v.vuint16);
      break;
    }
    case LVALUE_TYPE_INT32: {
      lvalue_set_bool(result, !left->v.vint32);
      break;
    }
    case LVALUE_TYPE_UINT32: {
      lvalue_set_bool(result, !left->v.vuint32);
      break;
    }
    case LVALUE_TYPE_INT64: {
      lvalue_set_bool(result, !left->v.vint64);
      break;
    }
    case LVALUE_TYPE_UINT64: {
      lvalue_set_bool(result, !left->v.vuint64);
      break;
    }
    case LVALUE_TYPE_FLOAT:
    case LVALUE_TYPE_DOUBLE:
    default: {
      lvalue_set_bool(result, !lvalue_to_int(left));
      break;
    }
  }
  return RET_OK;
}
END_C_DECLS

#endif /*LOX_VALUE_OPS_H*/
