#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================== 简单寄存器分配（朴素策略） ================== */

static int register_used[MIPS_REG_COUNT] = {0};

MipsRegister allocate_register(Operand *op) {
    // 朴素分配：总是使用$t0-$t3
    static int next_temp = MIPS_T0;
    
    if (next_temp > MIPS_T3) {
        next_temp = MIPS_T0;  // 循环使用
    }
    
    register_used[next_temp] = 1;
    return next_temp++;
}

void free_register(MipsRegister reg) {
    if (reg >= 0 && reg < MIPS_REG_COUNT) {
        register_used[reg] = 0;
    }
}

const char *reg_to_string(MipsRegister reg) {
    static const char *reg_names[] = {
        "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
        "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
    };
    
    if (reg >= 0 && reg < MIPS_REG_COUNT) {
        return reg_names[reg];
    }
    return "$unknown";
}

/* ================== MIPS代码链表操作 ================== */

MipsCodeList *new_mips_list(void) {
    MipsCodeList *list = (MipsCodeList *)malloc(sizeof(MipsCodeList));
    list->head = list->tail = NULL;
    list->count = 0;
    return list;
}

void append_mips_code(MipsCodeList *list, const char *fmt, ...) {
    if (!list) return;
    
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    MipsCode *code = (MipsCode *)malloc(sizeof(MipsCode));
    code->instruction = strdup(buffer);
    code->prev = code->next = NULL;
    
    if (!list->head) {
        list->head = list->tail = code;
    } else {
        list->tail->next = code;
        code->prev = list->tail;
        list->tail = code;
    }
    list->count++;
}

/* ================== MIPS代码生成 ================== */

MipsCodeList *gen_mips_from_ir(IRList *ir_list) {
    if (!ir_list || !ir_list->head) return NULL;
    
    MipsCodeList *mips_list = new_mips_list();
    
    // 生成MIPS汇编头
    append_mips_code(mips_list, ".data");
    append_mips_code(mips_list, "_prompt: .asciiz \"Enter an integer:\"");
    append_mips_code(mips_list, "_ret: .asciiz \"\\n\"");
    append_mips_code(mips_list, ".globl main");
    append_mips_code(mips_list, ".text");
    
    IRCode *current = ir_list->head;
    while (current) {
        gen_mips_from_ir_code(current, mips_list);
        current = current->next;
    }
    
    return mips_list;
}

void gen_mips_from_ir_code(IRCode *code, MipsCodeList *mips_list) {
    if (!code || !mips_list) return;
    
    switch (code->op) {
        case IR_FUNCTION:
            append_mips_code(mips_list, "%s:", code->result->name);
            if (strcmp(code->result->name, "main") == 0) {
                // 主函数序言
                append_mips_code(mips_list, "  addiu $sp, $sp, -64");
                append_mips_code(mips_list, "  sw $fp, 60($sp)");
                append_mips_code(mips_list, "  sw $ra, 56($sp)");
                append_mips_code(mips_list, "  move $fp, $sp");
            }
            break;
            
        case IR_LABEL:
            append_mips_code(mips_list, "%s:", code->result->name);
            break;
            
        case IR_ASSIGN:
            // t := x 或 t := #5
            if (code->op1 && code->result) {
                MipsRegister src_reg = allocate_register(code->op1);
                MipsRegister dst_reg = allocate_register(code->result);
                
                if (code->op1->kind == OP_CONST) {
                    // 加载立即数
                    append_mips_code(mips_list, "  li %s, %d", 
                                    reg_to_string(dst_reg), code->op1->int_val);
                } else if (code->op1->kind == OP_VAR) {
                    // 从内存加载
                    append_mips_code(mips_list, "  lw %s, %d($fp)", 
                                    reg_to_string(src_reg), code->op1->offset);
                    append_mips_code(mips_list, "  move %s, %s", 
                                    reg_to_string(dst_reg), reg_to_string(src_reg));
                }
                
                free_register(src_reg);
                free_register(dst_reg);
            }
            break;
            
        case IR_ADD:
            if (code->op1 && code->op2 && code->result) {
                MipsRegister reg1 = allocate_register(code->op1);
                MipsRegister reg2 = allocate_register(code->op2);
                MipsRegister dst_reg = allocate_register(code->result);
                
                // 加载操作数（简化：假设已在寄存器）
                append_mips_code(mips_list, "  add %s, %s, %s", 
                                reg_to_string(dst_reg), 
                                reg_to_string(reg1), 
                                reg_to_string(reg2));
                
                free_register(reg1);
                free_register(reg2);
                free_register(dst_reg);
            }
            break;
            
        case IR_SUB:
            if (code->op1 && code->op2 && code->result) {
                MipsRegister reg1 = allocate_register(code->op1);
                MipsRegister reg2 = allocate_register(code->op2);
                MipsRegister dst_reg = allocate_register(code->result);
                
                append_mips_code(mips_list, "  sub %s, %s, %s", 
                                reg_to_string(dst_reg), 
                                reg_to_string(reg1), 
                                reg_to_string(reg2));
                
                free_register(reg1);
                free_register(reg2);
                free_register(dst_reg);
            }
            break;
            
        case IR_MUL:
            if (code->op1 && code->op2 && code->result) {
                MipsRegister reg1 = allocate_register(code->op1);
                MipsRegister reg2 = allocate_register(code->op2);
                MipsRegister dst_reg = allocate_register(code->result);
                
                append_mips_code(mips_list, "  mul %s, %s, %s", 
                                reg_to_string(dst_reg), 
                                reg_to_string(reg1), 
                                reg_to_string(reg2));
                
                free_register(reg1);
                free_register(reg2);
                free_register(dst_reg);
            }
            break;
            
        case IR_DIV:
            if (code->op1 && code->op2 && code->result) {
                MipsRegister reg1 = allocate_register(code->op1);
                MipsRegister reg2 = allocate_register(code->op2);
                MipsRegister dst_reg = allocate_register(code->result);
                
                append_mips_code(mips_list, "  div %s, %s", 
                                reg_to_string(reg1), reg_to_string(reg2));
                append_mips_code(mips_list, "  mflo %s", 
                                reg_to_string(dst_reg));
                
                free_register(reg1);
                free_register(reg2);
                free_register(dst_reg);
            }
            break;
            
        case IR_GOTO:
            if (code->result) {
                append_mips_code(mips_list, "  j %s", code->result->name);
            }
            break;
            
        case IR_IF:
            // 简化：if x != 0 goto label
            if (code->op1 && code->result) {
                MipsRegister cond_reg = allocate_register(code->op1);
                append_mips_code(mips_list, "  bnez %s, %s", 
                                reg_to_string(cond_reg), code->result->name);
                free_register(cond_reg);
            }
            break;
            
        case IR_RETURN:
            if (code->op1) {
                MipsRegister ret_reg = allocate_register(code->op1);
                append_mips_code(mips_list, "  move $v0, %s", 
                                reg_to_string(ret_reg));
                free_register(ret_reg);
            }
            // 函数尾声
            append_mips_code(mips_list, "  move $sp, $fp");
            append_mips_code(mips_list, "  lw $ra, 56($sp)");
            append_mips_code(mips_list, "  lw $fp, 60($sp)");
            append_mips_code(mips_list, "  addiu $sp, $sp, 64");
            append_mips_code(mips_list, "  jr $ra");
            break;
            
        case IR_CALL:
            if (code->op1 && code->result) {
                // 函数调用（简化版）
                append_mips_code(mips_list, "  jal %s", code->op1->name);
                // 保存返回值
                MipsRegister dst_reg = allocate_register(code->result);
                append_mips_code(mips_list, "  move %s, $v0", 
                                reg_to_string(dst_reg));
                free_register(dst_reg);
            }
            break;
            
        case IR_CALL_VOID:
            if (code->op1) {
                append_mips_code(mips_list, "  jal %s", code->op1->name);
            }
            break;
            
        default:
            // 其他指令暂时忽略
            break;
    }
}

void print_mips_code(MipsCodeList *list) {
    if (!list || !list->head) {
        printf("Empty MIPS code\n");
        return;
    }
    
    printf("\n=== MIPS Assembly Code ===\n");
    
    MipsCode *current = list->head;
    while (current) {
        printf("%s\n", current->instruction);
        current = current->next;
    }
}

void save_mips_to_file(MipsCodeList *list, const char *filename) {
    if (!list || !filename) return;
    
    FILE *f = fopen(filename, "w");
    if (!f) return;
    
    MipsCode *current = list->head;
    while (current) {
        fprintf(f, "%s\n", current->instruction);
        current = current->next;
    }
    
    fclose(f);
    printf("MIPS code saved to %s\n", filename);
}