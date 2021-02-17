
let ops = [
  {name:'sub', ops:"-", deftype:"double"}, 
  {name:'mul', ops:"*", deftype:"double"}, 
  {name:'div', ops:"/", deftype:"double"}, 
  {name:'mod', ops:"%", deftype:"int32"}, 
  {name:'bit_and', ops:"&", deftype:"int32"}, 
  {name:'bit_or', ops:"|", deftype:"int32"}, 
  {name:'bit_xor', ops:"^", deftype:"int32"}, 
  {name:'lshift', ops:"<<", deftype:"int32"},
  {name:'rshift', ops:">>", deftype:"int32"},
  
]

ops.forEach(iter => {
let str=`
static inline ret_t lvalue_${iter.name}(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_int16(result, left->v.vint8${iter.ops}right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_int16(result, left->v.vuint8${iter.ops}right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_int32(result, left->v.vint16${iter.ops}right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_int32(result, left->v.vuint16${iter.ops}right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_int32(result, left->v.vint32${iter.ops}right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_uint32(result, left->v.vuint32${iter.ops}right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_int64(result, left->v.vint64${iter.ops}right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_uint64(result, left->v.vuint64${iter.ops}right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_${iter.deftype}(result, lvalue_${iter.deftype}(left)${iter.ops}lvalue_${iter.deftype}(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_${iter.deftype}(result, left->v.v${iter.deftype}${iter.ops}right->v.v${iter.deftype});
        break;
      }
      default: {
        lvalue_set_${iter.deftype}(result, lvalue_to_${iter.deftype}(left)${iter.ops}lvalue_to_${iter.deftype}(right));
        break;
      }
    }
  } else {
    lvalue_set_${iter.deftype}(result, lvalue_to_${iter.deftype}(left)${iter.ops}lvalue_to_${iter.deftype}(right));
  }

  return RET_OK;
}`
console.log(str);
})
  
let bool_ops = [
  {name:'and', ops:"&&", deftype:"int32"}, 
  {name:'or', ops:"||", deftype:"int32"}, 
  {name:'eq', ops:"==", deftype:"double"},
  {name:'not_eq', ops:"!=", deftype:"double"},
  {name:'less', ops:"<", deftype:"double"},
  {name:'less_eq', ops:"<=", deftype:"double"},
  {name:'great', ops:">", deftype:"double"},
  {name:'great_eq', ops:">=", deftype:"double"},
]

bool_ops.forEach(iter => {
let str=`
static inline ret_t lvalue_${iter.name}(const lvalue_t* left, const lvalue_t* right, lvalue_t* result) {
  if (left->type == right->type) {
    switch (left->type) {
      case LVALUE_TYPE_INT8: {
        lvalue_set_bool(result, left->v.vint8${iter.ops}right->v.vint8);
        break;
      }
      case LVALUE_TYPE_UINT8: {
        lvalue_set_bool(result, left->v.vuint8${iter.ops}right->v.vuint8);
        break;
      }
      case LVALUE_TYPE_INT16: {
        lvalue_set_bool(result, left->v.vint16${iter.ops}right->v.vint16);
        break;
      }
      case LVALUE_TYPE_UINT16: {
        lvalue_set_bool(result, left->v.vuint16${iter.ops}right->v.vuint16);
        break;
      }
      case LVALUE_TYPE_INT32: {
        lvalue_set_bool(result, left->v.vint32${iter.ops}right->v.vint32);
        break;
      }
      case LVALUE_TYPE_UINT32: {
        lvalue_set_bool(result, left->v.vuint32${iter.ops}right->v.vuint32);
        break;
      }
      case LVALUE_TYPE_INT64: {
        lvalue_set_bool(result, left->v.vint64${iter.ops}right->v.vint64);
        break;
      }
      case LVALUE_TYPE_UINT64: {
        lvalue_set_bool(result, left->v.vuint64${iter.ops}right->v.vuint64);
        break;
      }
      case LVALUE_TYPE_FLOAT: {
        lvalue_set_bool(result, lvalue_${iter.deftype}(left)${iter.ops}lvalue_${iter.deftype}(right));
        break;
      }
      case LVALUE_TYPE_DOUBLE: {
        lvalue_set_bool(result, left->v.v${iter.deftype}${iter.ops}right->v.v${iter.deftype});
        break;
      }
      default: {
        lvalue_set_bool(result, lvalue_to_${iter.deftype}(left)${iter.ops}lvalue_to_${iter.deftype}(right));
        break;
      }
    }
  } else {
    lvalue_set_bool(result, lvalue_to_${iter.deftype}(left)${iter.ops}lvalue_to_${iter.deftype}(right));
  }

  return RET_OK;
}`
console.log(str);
})
