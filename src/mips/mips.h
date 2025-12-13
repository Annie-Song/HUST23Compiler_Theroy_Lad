#ifndef MIPS_H
#define MIPS_H

#include "../ir/ir.h"

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
    MIPS_REG_COUNT
} MipsRegister;

/* 目标代码结构 */
typedef struct MipsCode {
    char *instruction;
    struct MipsCode *prev;
    struct MipsCode *next;
} MipsCode;

typedef struct MipsCodeList {
    MipsCode *head;
    MipsCode *tail;
    int count;
} MipsCodeList;

/* MIPS代码生成函数 */
MipsCodeList *gen_mips_from_ir(IRList *ir_list);
void print_mips_code(MipsCodeList *list);
void save_mips_to_file(MipsCodeList *list, const char *filename);

/* 寄存器分配函数 */
MipsRegister allocate_register(Operand *op);
void free_register(MipsRegister reg);
const char *reg_to_string(MipsRegister reg);

#endif