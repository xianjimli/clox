#include "compiler/ldisassembler.h"
#include "compiler/lcompiler_types.h"

static int disassemble_constant_instr(const char* name, lobject_func_t* func, int offset,
                                      lstr_t* str) {
  char buff[128];
  uint8_t constant = func->code->data[offset + 1];

  snprintf(buff, sizeof(buff) - 1, "%-16s", name);

  lstr_append(str, buff, -1);
  lstr_append_value(str, func->consts->data + constant);
  lstr_append(str, "\n", 1);

  return offset + 2;
}

static int disassemble_invoke_instr(const char* name, lobject_func_t* func, int offset,
                                    lstr_t* str) {
  char buff[128];
  uint8_t constant = func->code->data[offset + 1];
  uint8_t argCount = func->code->data[offset + 2];

  snprintf(buff, sizeof(buff) - 1, "%-16s (%d args) %4d '", name, argCount, constant);
  lstr_append(str, buff, -1);

  lstr_append_value(str, func->consts->data + constant);
  lstr_append(str, "\n", 1);

  return offset + 3;
}

static int disassemble_simple_instr(const char* name, int offset, lstr_t* str) {
  lstr_append_format_str(str, "%s\n", name);

  return offset + 1;
}

static int disassemble_byte_instr(const char* name, lobject_func_t* func, int offset, lstr_t* str) {
  char buff[128];
  uint8_t slot = func->code->data[offset + 1];
  snprintf(buff, sizeof(buff) - 1, "%-16s %4d\n", name, slot);
  lstr_append(str, buff, -1);

  return offset + 2;  // [debug]
}

static int disassemble_jump_instr(const char* name, int sign, lobject_func_t* func, int offset,
                                  lstr_t* str) {
  char buff[128];
  uint16_t jump = (uint16_t)(func->code->data[offset + 1] << 8);
  jump |= func->code->data[offset + 2];

  snprintf(buff, sizeof(buff) - 1, "%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
  lstr_append(str, buff, -1);

  return offset + 3;
}

int disassemble_instr(lobject_func_t* func, int offset, lstr_t* str) {
  uint8_t instruction = func->code->data[offset];
  lstr_append_int(str, "%04d ", offset);
  switch (instruction) {
    case OP_CONSTANT:
      return disassemble_constant_instr("OP_CONSTANT", func, offset, str);
    case OP_NIL:
      return disassemble_simple_instr("OP_NIL", offset, str);
    case OP_TRUE:
      return disassemble_simple_instr("OP_TRUE", offset, str);
    case OP_FALSE:
      return disassemble_simple_instr("OP_FALSE", offset, str);
    case OP_POP:
      return disassemble_simple_instr("OP_POP", offset, str);
    case OP_GET_LOCAL:
      return disassemble_byte_instr("OP_GET_LOCAL", func, offset, str);
    case OP_SET_LOCAL:
      return disassemble_byte_instr("OP_SET_LOCAL", func, offset, str);
    case OP_GET_GLOBAL:
      return disassemble_constant_instr("OP_GET_GLOBAL", func, offset, str);
    case OP_DEFINE_GLOBAL:
      return disassemble_constant_instr("OP_DEFINE_GLOBAL", func, offset, str);
    case OP_SET_GLOBAL:
      return disassemble_constant_instr("OP_SET_GLOBAL", func, offset, str);
    case OP_GET_UPVALUE:
      return disassemble_byte_instr("OP_GET_UPVALUE", func, offset, str);
    case OP_SET_UPVALUE:
      return disassemble_byte_instr("OP_SET_UPVALUE", func, offset, str);
    case OP_GET_PROPERTY:
      return disassemble_constant_instr("OP_GET_PROPERTY", func, offset, str);
    case OP_SET_PROPERTY:
      return disassemble_constant_instr("OP_SET_PROPERTY", func, offset, str);
    case OP_GET_SUPER:
      return disassemble_constant_instr("OP_GET_SUPER", func, offset, str);
    case OP_EQUAL:
      return disassemble_simple_instr("OP_EQUAL", offset, str);
    case OP_GREATER:
      return disassemble_simple_instr("OP_GREATER", offset, str);
    case OP_LESS:
      return disassemble_simple_instr("OP_LESS", offset, str);
    case OP_ADD:
      return disassemble_simple_instr("OP_ADD", offset, str);
    case OP_SUBTRACT:
      return disassemble_simple_instr("OP_SUBTRACT", offset, str);
    case OP_MULTIPLY:
      return disassemble_simple_instr("OP_MULTIPLY", offset, str);
    case OP_DIVIDE:
      return disassemble_simple_instr("OP_DIVIDE", offset, str);
    case OP_NOT:
      return disassemble_simple_instr("OP_NOT", offset, str);
    case OP_NEGATE:
      return disassemble_simple_instr("OP_NEGATE", offset, str);
    case OP_PRINT:
      return disassemble_simple_instr("OP_PRINT", offset, str);
    case OP_JUMP:
      return disassemble_jump_instr("OP_JUMP", 1, func, offset, str);
    case OP_JUMP_IF_FALSE:
      return disassemble_jump_instr("OP_JUMP_IF_FALSE", 1, func, offset, str);
    case OP_LOOP:
      return disassemble_jump_instr("OP_LOOP", -1, func, offset, str);
    case OP_CALL:
      return disassemble_byte_instr("OP_CALL", func, offset, str);
    case OP_INVOKE:
      return disassemble_invoke_instr("OP_INVOKE", func, offset, str);
    case OP_SUPER_INVOKE:
      return disassemble_invoke_instr("OP_SUPER_INVOKE", func, offset, str);
    case OP_CLOSURE: {
      offset++;
#if 0
      uint8_t constant = func->code->data[offset++];
      printf("%-16s %4d ", "OP_CLOSURE", constant);
      printValue(func->constants.values[constant]);
      printf("\n");
      
      ObjFunction* function = AS_FUNCTION(
          func->constants.values[constant]);
      for (int j = 0; j < function->upvalueCount; j++) {
        int isLocal = func->code->data[offset++];
        int index = func->code->data[offset++];
        printf("%04d      |                     %s %d\n",
               offset - 2, isLocal ? "local" : "upvalue", index);
      }
#endif
      return offset;
    }
    case OP_CLOSE_UPVALUE:
      return disassemble_simple_instr("OP_CLOSE_UPVALUE", offset, str);
    case OP_RETURN:
      return disassemble_simple_instr("OP_RETURN", offset, str);
    case OP_CLASS:
      return disassemble_constant_instr("OP_CLASS", func, offset, str);
    case OP_INHERIT:
      return disassemble_simple_instr("OP_INHERIT", offset, str);
    case OP_METHOD:
      return disassemble_constant_instr("OP_METHOD", func, offset, str);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}

ret_t ldisassemble(lobject_func_t* func, lstr_t* str) {
  int32_t offset = 0;
  return_value_if_fail(func != NULL && func->code != NULL && str != NULL, RET_BAD_PARAMS);

  for (offset = 0; offset < func->code->cursor;) {
    offset = disassemble_instr(func, offset, str);
  }

  return RET_OK;
}
