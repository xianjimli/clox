
const types = [
  {name:'bool', type:"bool_t"}, 
  {name:'int8', type:"int8_t"}, 
  {name:'uint8', type:"uint8_t"}, 
  {name:'int16', type:"int16_t"}, 
  {name:'uint16', type:"uint16_t"}, 
  {name:'int32', type:"int32_t"}, 
  {name:'uint32', type:"uint32_t"}, 
  {name:'int64', type:"uint32_t"}, 
  {name:'uint64', type:"uint64_t"}, 
  {name:'float', type:"float"}, 
  {name:'double', type:"double"},
  {name:'string', type:"const char*"},
  {name:'object', type:"lobject_t*"}
]

types.forEach(iter => {
let str = `
${iter.type} lobject_get_${iter.name}(lobject_t* o, const char* name, ${iter.type} defval);
ret_t lobject_set_${iter.name}(lobject_t* o, const char* name, ${iter.type} value);
`
console.log(str);
});

types.forEach(iter => {
let str = `
${iter.type} lobject_get_${iter.name}(lobject_t* o, const char* name, ${iter.type} defval) {
  lvalue_t v;
  if(lobject_get(o, name, &v) == RET_OK) {
    return lvalue_${iter.name}(&v);
  } else {
    return defval;
  }
}

ret_t lobject_set_${iter.name}(lobject_t* o, const char* name, ${iter.type} value) {
  lvalue_t v;
  lvalue_set_${iter.name}(&v, value);

  return lobject_set(o, name, &v);
}
`
console.log(str);
});
