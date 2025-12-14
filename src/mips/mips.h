#ifndef MIPS_H
#define MIPS_H

#include "../ir/ir.h"
#include "../symbol/symbol.h"

/* ================== 前向声明 ================== */
typedef struct MipsCode MipsCode;
typedef struct MipsCodeList MipsCodeList;
typedef struct MipsContext MipsContext;

/* MIPS寄存器定义 */
typedef enum {
    MIPS_ZERO,      // $zero
    MIPS_AT,        // $at (assembler temporary)
    MIPS_V0, MIPS_V1, // $v0-$v1 (function results)
    MIPS_A0, MIPS_A1, MIPS_A2, MIPS_A3, // $a0-$a3 (arguments)
    MIPS_T0, MIPS_T1, MIPS_T2, MIPS_T3, MIPS_T4, MIPS_T5, MIPS_T6, MIPS_T7, // $t0-$t7 (temporaries)
    MIPS_S0, MIPS_S1, MIPS_S2, MIPS_S3, MIPS_S4, MIPS_S5, MIPS_S6, MIPS_S7, // $s0-$s7 (saved)
    MIPS_T8, MIPS_T9, // $t8-$t9 (more temporaries)
    MIPS_K0, MIPS_K1, // $k0-$k1 (kernel)
    MIPS_GP,         // $gp (global pointer)
    MIPS_SP,         // $sp (stack pointer)
    MIPS_FP,         // $fp (frame pointer)
    MIPS_RA,         // $ra (return address)
    MIPS_REG_COUNT,
    MIPS_NO_REG = -1
} MipsRegister;

/* 寄存器描述符 */
typedef struct RegDesc {
    MipsRegister reg;           // 寄存器编号
    Operand *operand;          // 存储的操作数
    int dirty;                 // 是否被修改
    int used;                  // 是否正在使用
} RegDesc;

/* 变量描述符 */
typedef struct VarDesc {
    Operand *operand;          // 变量操作数
    int offset;               // 在栈帧中的偏移
    int in_reg;               // 是否在寄存器中
    MipsRegister reg;         // 所在的寄存器
} VarDesc;

/* MIPS代码生成上下文 */
struct MipsContext {
    MipsCodeList *code_list;   // 生成的代码
    RegDesc registers[MIPS_REG_COUNT];  // 寄存器状态
    int next_offset;           // 下一个可用的栈偏移
    int in_function;           // 当前是否在函数中
    char *current_func;        // 当前函数名
    int arg_count;             // 当前函数调用参数计数
    int label_count;           // 标签计数器
};

/* 目标代码结构 */
struct MipsCode {
    char *instruction;
    struct MipsCode *prev;
    struct MipsCode *next;
};

struct MipsCodeList {
    MipsCode *head;
    MipsCode *tail;
    int count;
};

/* ================== 主要接口函数 ================== */
MipsCodeList *gen_mips_from_ir(IRList *ir_list, SymbolTable *symtab);
void print_mips_code(MipsCodeList *list);
void save_mips_to_file(MipsCodeList *list, const char *filename);

/* ================== 寄存器管理 ================== */
void free_all_temp_registers(MipsContext *ctx);
void init_registers(MipsContext *ctx);
MipsRegister allocate_register(MipsContext *ctx, Operand *op);
MipsRegister get_operand_register(MipsContext *ctx, Operand *op);
void spill_register(MipsContext *ctx, MipsRegister reg);
void free_register(MipsContext *ctx, MipsRegister reg);
const char *reg_to_string(MipsRegister reg);

/* ================== 代码生成辅助函数 ================== */
void gen_mips_from_ir_code(MipsContext *ctx, IRCode *code);
void gen_function_prologue(MipsContext *ctx, IRCode *code);
void gen_label(MipsContext *ctx, IRCode *code);
void gen_goto(MipsContext *ctx, IRCode *code);
void gen_if(MipsContext *ctx, IRCode *code);
void gen_return(MipsContext *ctx, IRCode *code);
void gen_assign(MipsContext *ctx, IRCode *code);
void gen_binary_op(MipsContext *ctx, IRCode *code);
void gen_compare_op(MipsContext *ctx, IRCode *code);
void gen_array_load(MipsContext *ctx, IRCode *code);
void gen_array_store(MipsContext *ctx, IRCode *code);
void gen_call(MipsContext *ctx, IRCode *code);
void gen_call_void(MipsContext *ctx, IRCode *code);
void gen_io_functions(MipsContext *ctx);

#endif