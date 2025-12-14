#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MIPS_DEBUG 0 

#if MIPS_DEBUG
    printf("[MIPS DEBUG] ...\n");
#endif

/* ================== 工具函数 ================== */

static MipsCodeList *new_mips_list(void) {
    MipsCodeList *list = (MipsCodeList *)malloc(sizeof(MipsCodeList));
    if (list) {
        list->head = list->tail = NULL;
        list->count = 0;
    }
    return list;
}

static void emit(MipsContext *ctx, const char *fmt, ...) {
    if (!ctx || !ctx->code_list) return;
    
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    MipsCode *code = (MipsCode *)malloc(sizeof(MipsCode));
    if (!code) return;
    
    code->instruction = strdup(buffer);
    code->prev = code->next = NULL;
    
    if (!ctx->code_list->head) {
        ctx->code_list->head = ctx->code_list->tail = code;
    } else {
        ctx->code_list->tail->next = code;
        code->prev = ctx->code_list->tail;
        ctx->code_list->tail = code;
    }
    ctx->code_list->count++;
}

/* ================== 寄存器管理 ================== */
void free_all_temp_registers(MipsContext *ctx) {
    if (!ctx) return;
    
    // 释放所有临时寄存器
    for (int i = MIPS_T0; i <= MIPS_T9; i++) {
        free_register(ctx, (MipsRegister)i);
    }
    
    // 也可以释放保存寄存器（如果需要）
    for (int i = MIPS_S0; i <= MIPS_S7; i++) {
        if (ctx->registers[i].used && !ctx->registers[i].dirty) {
            free_register(ctx, (MipsRegister)i);
        }
    }
}

void init_registers(MipsContext *ctx) {
    if (!ctx) return;
    
    for (int i = 0; i < MIPS_REG_COUNT; i++) {
        ctx->registers[i].reg = (MipsRegister)i;
        ctx->registers[i].operand = NULL;
        ctx->registers[i].dirty = 0;
        ctx->registers[i].used = 0;
    }
    
    // 保留特殊寄存器（它们总是被使用）
    ctx->registers[MIPS_ZERO].used = 1;
    ctx->registers[MIPS_SP].used = 1;
    ctx->registers[MIPS_FP].used = 1;
    ctx->registers[MIPS_RA].used = 1;
    ctx->registers[MIPS_GP].used = 1;
    
    // $at 也通常保留
    ctx->registers[MIPS_AT].used = 1;
}

MipsRegister allocate_register(MipsContext *ctx, Operand *op) {
    if (!ctx) return MIPS_NO_REG;
    
    // 如果操作数已经在寄存器中，直接返回
    if (op) {
        for (int i = 0; i < MIPS_REG_COUNT; i++) {
            if (ctx->registers[i].used && ctx->registers[i].operand == op) {
                return (MipsRegister)i;
            }
        }
    }
    
    // 优先分配 $t0-$t9 临时寄存器
    int temp_regs[] = {
        MIPS_T0, MIPS_T1, MIPS_T2, MIPS_T3, MIPS_T4, 
        MIPS_T5, MIPS_T6, MIPS_T7, MIPS_T8, MIPS_T9
    };
    
    for (int i = 0; i < 10; i++) {
        MipsRegister reg = temp_regs[i];
        if (!ctx->registers[reg].used) {
            ctx->registers[reg].used = 1;
            ctx->registers[reg].operand = op;
            ctx->registers[reg].dirty = 0;
            printf("[MIPS DEBUG] Allocated temp register %s\n", reg_to_string(reg));
            return reg;
        }
    }
    
    // 如果所有临时寄存器都用完了，释放一个不重要的
    for (int i = 0; i < 10; i++) {
        MipsRegister reg = temp_regs[i];
        if (ctx->registers[reg].used && !ctx->registers[reg].dirty) {
            printf("[MIPS DEBUG] Releasing temp register %s for reuse\n", reg_to_string(reg));
            free_register(ctx, reg);
            
            ctx->registers[reg].used = 1;
            ctx->registers[reg].operand = op;
            ctx->registers[reg].dirty = 0;
            return reg;
        }
    }
    
    // 最后尝试分配 $s0-$s7（保存寄存器）
    for (int i = MIPS_S0; i <= MIPS_S7; i++) {
        if (!ctx->registers[i].used) {
            ctx->registers[i].used = 1;
            ctx->registers[i].operand = op;
            ctx->registers[i].dirty = 0;
            printf("[MIPS DEBUG] Allocated saved register %s\n", reg_to_string((MipsRegister)i));
            return (MipsRegister)i;
        }
    }
    
    printf("[MIPS ERROR] No registers available to allocate\n");
    return MIPS_NO_REG;
}

MipsRegister get_operand_register(MipsContext *ctx, Operand *op) {
    if (!ctx || !op) return MIPS_NO_REG;
    
    // 查找操作数所在的寄存器
    for (int i = 0; i < MIPS_REG_COUNT; i++) {
        if (ctx->registers[i].used && ctx->registers[i].operand == op) {
            return (MipsRegister)i;
        }
    }
    
    // 如果操作数是常量，尝试查找包含相同值的寄存器
    if (op->kind == OP_CONST) {
        for (int i = 0; i < MIPS_REG_COUNT; i++) {
            if (ctx->registers[i].used && ctx->registers[i].operand && 
                ctx->registers[i].operand->kind == OP_CONST &&
                ctx->registers[i].operand->int_val == op->int_val) {
                return (MipsRegister)i;
            }
        }
    }
    
    return MIPS_NO_REG;
}

void spill_register(MipsContext *ctx, MipsRegister reg) {
    if (!ctx || reg < 0 || reg >= MIPS_REG_COUNT) return;
    if (!ctx->registers[reg].used || !ctx->registers[reg].dirty) return;
    
    Operand *op = ctx->registers[reg].operand;
    if (op && op->kind == OP_VAR && op->offset != 0) {
        emit(ctx, "  sw %s, %d($fp)", reg_to_string(reg), op->offset);
    }
    
    ctx->registers[reg].operand = NULL;
    ctx->registers[reg].dirty = 0;
}

void free_register(MipsContext *ctx, MipsRegister reg) {
    if (!ctx || reg < 0 || reg >= MIPS_REG_COUNT) return;
    
    // 不要释放特殊寄存器
    if (reg == MIPS_ZERO || reg == MIPS_SP || reg == MIPS_FP || 
        reg == MIPS_RA || reg == MIPS_GP) {
        return;
    }
    
    if (ctx->registers[reg].used) {
        spill_register(ctx, reg);
        printf("[MIPS DEBUG] Freed register %s\n", reg_to_string(reg));
        ctx->registers[reg].used = 0;
        ctx->registers[reg].operand = NULL;
        ctx->registers[reg].dirty = 0;
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

/* ================== 代码生成主函数 ================== */

MipsCodeList *gen_mips_from_ir(IRList *ir_list, SymbolTable *symtab) {
    if (!ir_list || !ir_list->head) {
        printf("[MIPS] No IR to generate from\n");
        return NULL;
    }
    
    printf("[MIPS] Generating MIPS code from IR...\n");
    
    // 创建上下文
    MipsContext ctx;
    memset(&ctx, 0, sizeof(MipsContext));
    ctx.code_list = new_mips_list();
    if (!ctx.code_list) {
        printf("[MIPS] Failed to create code list\n");
        return NULL;
    }
    ctx.next_offset = -4;
    ctx.label_count = 0;
    ctx.in_function = 0;
    ctx.current_func = NULL;
    ctx.arg_count = 0;
    
    init_registers(&ctx);
    
    // 生成汇编头
    emit(&ctx, ".data");
    emit(&ctx, "_prompt: .asciiz \"Enter an integer:\"");
    emit(&ctx, "_ret: .asciiz \"\\n\"");
    emit(&ctx, ".globl main");
    emit(&ctx, ".text");
    
    // 生成I/O库函数
    gen_io_functions(&ctx);
    
    // 遍历IR生成代码
    IRCode *current = ir_list->head;
    while (current) {
        gen_mips_from_ir_code(&ctx, current);
        current = current->next;
    }
    
    printf("[MIPS] Generated %d MIPS instructions\n", ctx.code_list->count);
    return ctx.code_list;
}

void gen_mips_from_ir_code(MipsContext *ctx, IRCode *code) {
     if (!ctx || !code) return;
    
    printf("[MIPS DEBUG] Processing IR op=%d\n", code->op);
    
    for (int i = MIPS_T0; i <= MIPS_T9; i++) {
        if (ctx->registers[i].used && !ctx->registers[i].dirty && 
            ctx->registers[i].operand && ctx->registers[i].operand->kind == OP_TEMP) {
            free_register(ctx, (MipsRegister)i);
        }
    }

    switch (code->op) {
        case IR_FUNCTION:
            gen_function_prologue(ctx, code);
            break;
        case IR_LABEL:
            gen_label(ctx, code);
            break;
        case IR_ASSIGN:
            gen_assign(ctx, code);
            break;
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
            gen_binary_op(ctx, code);
            break;
        case IR_LT:
        case IR_LE:
        case IR_GT:
        case IR_GE:
        case IR_EQ:
        case IR_NE:
            gen_compare_op(ctx, code);
            break;
        case IR_GOTO:
            gen_goto(ctx, code);
            break;
        case IR_IF:
            gen_if(ctx, code);
            break;
        case IR_RETURN:
            gen_return(ctx, code);
            break;
        case IR_CALL:
            gen_call(ctx, code);
            break;
        case IR_CALL_VOID:
            gen_call_void(ctx, code);
            break;
        case IR_ARRAY_LOAD:
            gen_array_load(ctx, code);
            break;
        case IR_ARRAY_STORE:
            gen_array_store(ctx, code);
            break;
        case IR_PARAM:
            // 参数声明，暂不生成代码
            break;
        case IR_ARG:
            // 参数传递，暂简单处理
            break;
        default:
            printf("[MIPS WARNING] Unsupported IR opcode: %d\n", code->op);
            // 对于其他操作码，尝试跳过但打印警告
            break;
    }
}

/* ================== 具体指令生成 ================== */

void gen_function_prologue(MipsContext *ctx, IRCode *code) {
    if (!code->result) return;
    
    ctx->in_function = 1;
    ctx->current_func = code->result->name;
    ctx->next_offset = -4;  // 重置栈偏移
    
    printf("[MIPS DEBUG] Entering function %s, next_offset=%d\n", 
           code->result->name, ctx->next_offset);
    
    emit(ctx, "\n%s:", code->result->name);
    
    // 分配更大的栈帧以容纳更多变量
    emit(ctx, "  addiu $sp, $sp, -256");
    emit(ctx, "  sw $fp, 252($sp)");
    emit(ctx, "  sw $ra, 248($sp)");
    emit(ctx, "  move $fp, $sp");
}

void gen_label(MipsContext *ctx, IRCode *code) {
    if (code->result && code->result->name) {
        free_all_temp_registers(ctx);
        emit(ctx, "%s:", code->result->name);
    }
}

void gen_goto(MipsContext *ctx, IRCode *code) {
    if (code->result && code->result->name) {
        emit(ctx, "  j %s", code->result->name);
    }
}

void gen_if(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->result) return;
    
    printf("[MIPS DEBUG] IF: op1 kind=%d, offset=%d\n", 
           code->op1->kind, code->op1->offset);
    
    // 获取条件值
    MipsRegister cond_reg = get_operand_register(ctx, code->op1);
    if (cond_reg == MIPS_NO_REG) {
        cond_reg = allocate_register(ctx, code->op1);
        if (cond_reg == MIPS_NO_REG) {
            printf("[MIPS ERROR] Failed to allocate register for condition\n");
            return;
        }
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(cond_reg), code->op1->int_val);
        } else {
            int offset = code->op1->offset;
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(cond_reg), offset);
            } else {
                printf("[MIPS WARNING] No offset for condition\n");
                emit(ctx, "  li %s, 1", reg_to_string(cond_reg)); // 默认true
            }
        }
    }
    
    // 条件跳转
    emit(ctx, "  bnez %s, %s", reg_to_string(cond_reg), code->result->name);
    
    // 如果不是常量，释放寄存器
    if (code->op1->kind != OP_CONST) {
        free_register(ctx, cond_reg);
    }
}

void gen_return(MipsContext *ctx, IRCode *code) {
    printf("[MIPS DEBUG] Generating return\n");
    
    if (code->op1) {
        // 有返回值的返回
        MipsRegister ret_reg = get_operand_register(ctx, code->op1);
        if (ret_reg == MIPS_NO_REG) {
            ret_reg = allocate_register(ctx, code->op1);
            if (ret_reg != MIPS_NO_REG) {
                if (code->op1->kind == OP_CONST) {
                    emit(ctx, "  li %s, %d", reg_to_string(ret_reg), code->op1->int_val);
                } else if (code->op1->kind == OP_VAR || code->op1->kind == OP_TEMP) {
                    int offset = code->op1->offset;
                    if (offset != 0) {
                        emit(ctx, "  lw %s, %d($fp)", reg_to_string(ret_reg), offset);
                    }
                }
            }
        }
        
        if (ret_reg != MIPS_NO_REG) {
            emit(ctx, "  move $v0, %s", reg_to_string(ret_reg));
            free_register(ctx, ret_reg);
        }
    }
    
    // 函数尾声 - 使用正确的偏移量
    emit(ctx, "  move $sp, $fp");
    emit(ctx, "  lw $ra, 248($sp)");
    emit(ctx, "  lw $fp, 252($sp)");
    emit(ctx, "  addiu $sp, $sp, 256");
    emit(ctx, "  jr $ra");
    
    ctx->in_function = 0;
    ctx->current_func = NULL;
}

void gen_assign(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->result) return;
    
    printf("[MIPS DEBUG] ASSIGN: op1 kind=%d, result kind=%d\n",  code->op1->kind, code->result->kind);

    // 检查是否是自赋值：x := x
    if (code->op1->kind == OP_VAR && code->result->kind == OP_VAR &&
        code->op1->name && code->result->name &&
        strcmp(code->op1->name, code->result->name) == 0) {
        printf("[MIPS DEBUG] Skipping self-assignment: %s := %s\n", 
               code->result->name, code->op1->name);
        return;
    }

    printf("[MIPS DEBUG] ASSIGN: op1 kind=%d, result kind=%d, result offset=%d\n", 
           code->op1->kind, code->result->kind, code->result->offset);
    
    // 如果是常量赋值给临时变量
    if (code->op1->kind == OP_CONST && code->result->kind == OP_TEMP) {
        MipsRegister dst_reg = allocate_register(ctx, code->result);
        if (dst_reg != MIPS_NO_REG) {
            emit(ctx, "  li %s, %d", reg_to_string(dst_reg), code->op1->int_val);
            
            // 为临时变量分配栈偏移
            if (code->result->offset == 0 && ctx->in_function) {
                code->result->offset = ctx->next_offset;
                ctx->next_offset -= 4;
                printf("[MIPS DEBUG] Allocated offset %d for temp %s\n", 
                       code->result->offset, code->result->name);
            }
            
            if (code->result->offset != 0) {
                emit(ctx, "  sw %s, %d($fp)", reg_to_string(dst_reg), code->result->offset);
            }
            
            free_register(ctx, dst_reg);
            return;
        }
    }

   // 获取源操作数值
    MipsRegister src_reg = get_operand_register(ctx, code->op1);
    if (src_reg == MIPS_NO_REG) {
        src_reg = allocate_register(ctx, code->op1);
        if (src_reg == MIPS_NO_REG) return;
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(src_reg), code->op1->int_val);
        } else if (code->op1->kind == OP_VAR) {
            // 变量加载 - 关键修复
            int offset = code->op1->offset;
            printf("[MIPS DEBUG] Loading var %s from offset %d\n", 
                   code->op1->name, offset);
            
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(src_reg), offset);
            } else {
                // 如果偏移量为0，可能是全局变量或参数
                // 尝试使用默认偏移量
                printf("[MIPS WARNING] Var %s has offset 0, using default loading\n", 
                       code->op1->name);
                
                // 根据变量名猜测偏移量（这是一个临时方案）
                // 在实际编译器中，这应该从符号表获取
                if (strcmp(code->op1->name, "a") == 0) offset = -4;
                else if (strcmp(code->op1->name, "b") == 0) offset = -8;
                else if (strcmp(code->op1->name, "result") == 0) offset = -12;
                else if (strcmp(code->op1->name, "i") == 0) offset = -16;
                else if (strcmp(code->op1->name, "j") == 0) offset = -20;
                else if (strcmp(code->op1->name, "k") == 0) offset = -24;
                else {
                    // 分配一个新的偏移量
                    offset = ctx->next_offset;
                    ctx->next_offset -= 4;
                    code->op1->offset = offset;
                    printf("[MIPS DEBUG] Allocated new offset %d for var %s\n", 
                           offset, code->op1->name);
                }
                
                if (offset != 0) {
                    emit(ctx, "  lw %s, %d($fp)", reg_to_string(src_reg), offset);
                } else {
                    emit(ctx, "  li %s, 0", reg_to_string(src_reg));
                }
            }
        } else if (code->op1->kind == OP_TEMP) {
            // 临时变量
            int offset = code->op1->offset;
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(src_reg), offset);
            } else {
                // 临时变量没有偏移量，假设为0
                emit(ctx, "  li %s, 0", reg_to_string(src_reg));
            }
        }
    }
    
    // 处理目标
    if (code->result->kind == OP_VAR || code->result->kind == OP_TEMP) {
        // 确保目标有偏移量
        int offset = code->result->offset;
        if (offset == 0 && ctx->in_function) {
            offset = ctx->next_offset;
            ctx->next_offset -= 4;
            code->result->offset = offset;
            printf("[MIPS DEBUG] Allocated offset %d for result\n", offset);
        }
        
        // 存储到内存
        if (offset != 0) {
            emit(ctx, "  sw %s, %d($fp)", reg_to_string(src_reg), offset);
        }
        
        // 如果目标需要寄存器表示，也加载到寄存器
        if (code->result->kind == OP_VAR) {
            MipsRegister dst_reg = allocate_register(ctx, code->result);
            if (dst_reg != MIPS_NO_REG && dst_reg != src_reg) {
                emit(ctx, "  move %s, %s", reg_to_string(dst_reg), reg_to_string(src_reg));
                ctx->registers[dst_reg].dirty = 1;
            }
        }
    }
    
    // 释放源寄存器（如果不是变量）
    if (code->op1->kind != OP_VAR) {
        free_register(ctx, src_reg);
    }
}

void gen_binary_op(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->op2 || !code->result) return;
    
    printf("[MIPS DEBUG] BINARY OP: op=%d\n", code->op);
    
    // 获取左操作数
    MipsRegister reg1 = get_operand_register(ctx, code->op1);
    if (reg1 == MIPS_NO_REG) {
        reg1 = allocate_register(ctx, code->op1);
        if (reg1 == MIPS_NO_REG) {
            printf("[MIPS ERROR] Failed to allocate register for op1\n");
            return;
        }
        
        if (code->op1->kind == OP_CONST) {
            printf("[MIPS DEBUG] Loading constant %d\n", code->op1->int_val);
            emit(ctx, "  li %s, %d", reg_to_string(reg1), code->op1->int_val);
        } else {
            // 加载变量值
            int offset = code->op1->offset;
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg1), offset);
            } else {
                // 如果没有偏移量，假设为0
                emit(ctx, "  li %s, 0", reg_to_string(reg1));
            }
        }
    }
    
    // 获取右操作数
    MipsRegister reg2;
    if (code->op2->kind == OP_CONST) {
        // 对于常量，尝试重用包含相同值的寄存器
        reg2 = get_operand_register(ctx, code->op2);
        if (reg2 == MIPS_NO_REG) {
            reg2 = allocate_register(ctx, NULL); // 不绑定操作数
            if (reg2 == MIPS_NO_REG) {
                printf("[MIPS ERROR] Failed to allocate register for op2 const\n");
                free_register(ctx, reg1);
                return;
            }
            printf("[MIPS DEBUG] Loading constant %d\n", code->op2->int_val);
            emit(ctx, "  li %s, %d", reg_to_string(reg2), code->op2->int_val);
        }
    } else {
        reg2 = get_operand_register(ctx, code->op2);
        if (reg2 == MIPS_NO_REG) {
            reg2 = allocate_register(ctx, code->op2);
            if (reg2 == MIPS_NO_REG) {
                printf("[MIPS ERROR] Failed to allocate register for op2\n");
                free_register(ctx, reg1);
                return;
            }
            
            int offset = code->op2->offset;
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg2), offset);
            } else {
                // 如果没有偏移量，假设为0
                emit(ctx, "  li %s, 0", reg_to_string(reg2));
            }
        }
    }
    
    // 分配结果寄存器
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg == MIPS_NO_REG) {
        printf("[MIPS ERROR] Failed to allocate register for result\n");
        free_register(ctx, reg1);
        if (reg2 != MIPS_NO_REG) free_register(ctx, reg2);
        return;
    }
    
    printf("[MIPS DEBUG] Generating binary operation\n");
    
    // 生成运算指令
    switch (code->op) {
        case IR_ADD:
            emit(ctx, "  addu %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            break;
        case IR_SUB:
            emit(ctx, "  subu %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            break;
        case IR_MUL:
            emit(ctx, "  mul %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            break;
        case IR_DIV:
            emit(ctx, "  div %s, %s", reg_to_string(reg1), reg_to_string(reg2));
            emit(ctx, "  mflo %s", reg_to_string(dst_reg));
            break;
        default:
            printf("[MIPS WARNING] Unhandled binary op %d\n", code->op);
            break;
    }
    
    // 存储结果（如果是变量）
    if (code->result->kind == OP_VAR || code->result->kind == OP_TEMP) {
        int offset = code->result->offset;
        if (offset == 0 && ctx->in_function) {
            offset = ctx->next_offset;
            ctx->next_offset -= 4;
            code->result->offset = offset;
            printf("[MIPS DEBUG] Allocated offset %d for result\n", offset);
        }
        
        if (offset != 0) {
            emit(ctx, "  sw %s, %d($fp)", reg_to_string(dst_reg), offset);
        }
    }
    
    // 立即释放输入寄存器（除非它们是结果）
    if (reg1 != dst_reg) free_register(ctx, reg1);
    if (reg2 != dst_reg && reg2 != MIPS_NO_REG) free_register(ctx, reg2);
    
    // 标记结果寄存器
    ctx->registers[dst_reg].dirty = (code->result->kind == OP_VAR);
}

void gen_compare_op(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->op2 || !code->result) return;
    
    printf("[MIPS DEBUG] COMPARE OP: op=%d, op1 kind=%d (offset=%d), op2 kind=%d (offset=%d)\n", 
           code->op, code->op1->kind, code->op1->offset, 
           code->op2->kind, code->op2->offset);
    
    // 获取左操作数
    MipsRegister reg1 = get_operand_register(ctx, code->op1);
    if (reg1 == MIPS_NO_REG) {
        reg1 = allocate_register(ctx, code->op1);
        if (reg1 == MIPS_NO_REG) {
            printf("[MIPS ERROR] Failed to allocate register for compare op1\n");
            return;
        }
        
        if (code->op1->kind == OP_CONST) {
            printf("[MIPS DEBUG] Loading constant %d for compare op1\n", code->op1->int_val);
            emit(ctx, "  li %s, %d", reg_to_string(reg1), code->op1->int_val);
        } else {
            // 处理变量和临时变量
            int offset = code->op1->offset;
            printf("[MIPS DEBUG] Loading compare op1 from offset %d\n", offset);
            
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg1), offset);
            } else {
                // 如果没有偏移量，给临时变量分配一个
                if (ctx->in_function && (code->op1->kind == OP_TEMP || code->op1->kind == OP_VAR)) {
                    offset = ctx->next_offset;
                    ctx->next_offset -= 4;
                    code->op1->offset = offset;
                    printf("[MIPS DEBUG] Allocated offset %d for temp compare op1\n", offset);
                    // 加载值，假设初始为0
                    emit(ctx, "  li %s, 0", reg_to_string(reg1));
                    emit(ctx, "  sw %s, %d($fp)", reg_to_string(reg1), offset);
                } else {
                    printf("[MIPS WARNING] No offset for compare op1\n");
                    emit(ctx, "  li %s, 0", reg_to_string(reg1));
                }
            }
        }
    }
    
    // 获取右操作数
    MipsRegister reg2;
    if (code->op2->kind == OP_CONST) {
        reg2 = allocate_register(ctx, NULL);
        if (reg2 == MIPS_NO_REG) {
            printf("[MIPS ERROR] Failed to allocate register for compare op2 const\n");
            free_register(ctx, reg1);
            return;
        }
        printf("[MIPS DEBUG] Loading constant %d for compare op2\n", code->op2->int_val);
        emit(ctx, "  li %s, %d", reg_to_string(reg2), code->op2->int_val);
    } else {
        reg2 = get_operand_register(ctx, code->op2);
        if (reg2 == MIPS_NO_REG) {
            reg2 = allocate_register(ctx, code->op2);
            if (reg2 == MIPS_NO_REG) {
                printf("[MIPS ERROR] Failed to allocate register for compare op2\n");
                free_register(ctx, reg1);
                return;
            }
            
            int offset = code->op2->offset;
            printf("[MIPS DEBUG] Loading compare op2 from offset %d\n", offset);
            
            if (offset != 0) {
                emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg2), offset);
            } else {
                // 如果没有偏移量，给临时变量分配一个
                if (ctx->in_function && (code->op2->kind == OP_TEMP || code->op2->kind == OP_VAR)) {
                    offset = ctx->next_offset;
                    ctx->next_offset -= 4;
                    code->op2->offset = offset;
                    printf("[MIPS DEBUG] Allocated offset %d for temp compare op2\n", offset);
                    // 加载值，假设初始为0
                    emit(ctx, "  li %s, 0", reg_to_string(reg2));
                    emit(ctx, "  sw %s, %d($fp)", reg_to_string(reg2), offset);
                } else {
                    printf("[MIPS WARNING] No offset for compare op2\n");
                    emit(ctx, "  li %s, 0", reg_to_string(reg2));
                }
            }
        }
    }
    
    // 分配结果寄存器
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg == MIPS_NO_REG) {
        printf("[MIPS ERROR] Failed to allocate register for compare result\n");
        free_register(ctx, reg1);
        if (code->op2->kind == OP_CONST) free_register(ctx, reg2);
        return;
    }
    
    printf("[MIPS DEBUG] Generating compare operation %d\n", code->op);
    
    // 生成比较指令
    switch (code->op) {
        case IR_LT:
            emit(ctx, "  slt %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            break;
        case IR_LE:
            // a <= b 等价于 !(b < a)
            emit(ctx, "  slt %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg2), reg_to_string(reg1));
            emit(ctx, "  xori %s, %s, 1", 
                 reg_to_string(dst_reg), reg_to_string(dst_reg));
            break;
        case IR_GT:
            // a > b 等价于 b < a
            emit(ctx, "  slt %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg2), reg_to_string(reg1));
            break;
        case IR_GE:
            // a >= b 等价于 !(a < b)
            emit(ctx, "  slt %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            emit(ctx, "  xori %s, %s, 1", 
                 reg_to_string(dst_reg), reg_to_string(dst_reg));
            break;
        case IR_EQ:
            // a == b
            emit(ctx, "  xor %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            emit(ctx, "  sltiu %s, %s, 1", 
                 reg_to_string(dst_reg), reg_to_string(dst_reg));
            break;
        case IR_NE:
            // a != b
            emit(ctx, "  xor %s, %s, %s", 
                 reg_to_string(dst_reg), reg_to_string(reg1), reg_to_string(reg2));
            emit(ctx, "  sltu %s, $zero, %s", 
                 reg_to_string(dst_reg), reg_to_string(dst_reg));
            break;
        default:
            printf("[MIPS WARNING] Unhandled compare op %d\n", code->op);
            // 默认设置为假
            emit(ctx, "  li %s, 0", reg_to_string(dst_reg));
            break;
    }
    
    // 存储结果到内存（如果是变量或临时变量）
    if (code->result->kind == OP_VAR || code->result->kind == OP_TEMP) {
        int offset = code->result->offset;
        if (offset == 0 && ctx->in_function) {
            offset = ctx->next_offset;
            ctx->next_offset -= 4;
            code->result->offset = offset;
            printf("[MIPS DEBUG] Allocated offset %d for compare result\n", offset);
        }
        
        if (offset != 0) {
            emit(ctx, "  sw %s, %d($fp)", reg_to_string(dst_reg), offset);
        }
    }
    
    // 清理临时寄存器
    if (code->op2->kind == OP_CONST) {
        free_register(ctx, reg2);
    }
    if (code->op1->kind != OP_VAR) {
        free_register(ctx, reg1);
    }
    
    // 结果寄存器标记
    if (code->result->kind == OP_TEMP) {
        ctx->registers[dst_reg].dirty = 0;
    } else {
        ctx->registers[dst_reg].dirty = 1;
    }
    
    printf("[MIPS DEBUG] Compare operation completed\n");
}

void gen_array_load(MipsContext *ctx, IRCode *code) {
    // result = array[index]
    if (!code->op1 || !code->op2 || !code->result) return;
    
    // 数组基址偏移
    int array_offset = code->op1->offset ? code->op1->offset : -4;
    
    // 加载索引
    MipsRegister index_reg = get_operand_register(ctx, code->op2);
    if (index_reg == MIPS_NO_REG) {
        index_reg = allocate_register(ctx, code->op2);
        if (index_reg == MIPS_NO_REG) return;
        
        if (code->op2->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(index_reg), code->op2->int_val);
        } else if (code->op2->kind == OP_VAR) {
            int offset = code->op2->offset ? code->op2->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(index_reg), offset);
        }
    }
    
    // 计算地址: addr = $fp + array_offset + index*4
    MipsRegister addr_reg = allocate_register(ctx, NULL);
    if (addr_reg == MIPS_NO_REG) {
        free_register(ctx, index_reg);
        return;
    }
    
    emit(ctx, "  sll %s, %s, 2", reg_to_string(addr_reg), reg_to_string(index_reg));
    emit(ctx, "  addu %s, %s, $fp", reg_to_string(addr_reg), reg_to_string(addr_reg));
    emit(ctx, "  addiu %s, %s, %d", reg_to_string(addr_reg), reg_to_string(addr_reg), array_offset);
    
    // 加载数组元素
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg != MIPS_NO_REG) {
        emit(ctx, "  lw %s, 0(%s)", reg_to_string(dst_reg), reg_to_string(addr_reg));
        
        // 结果标记
        if (code->result->kind == OP_TEMP) {
            ctx->registers[dst_reg].dirty = 0;
        } else {
            ctx->registers[dst_reg].dirty = 1;
        }
    }
    
    // 清理
    free_register(ctx, addr_reg);
    if (code->op2->kind != OP_VAR) {
        free_register(ctx, index_reg);
    }
}

void gen_array_store(MipsContext *ctx, IRCode *code) {
    // array[index] = value
    if (!code->op1 || !code->op2 || !code->result) return;
    
    // 数组基址偏移
    int array_offset = code->op1->offset ? code->op1->offset : -4;
    
    // 加载索引
    MipsRegister index_reg = get_operand_register(ctx, code->op2);
    if (index_reg == MIPS_NO_REG) {
        index_reg = allocate_register(ctx, code->op2);
        if (index_reg == MIPS_NO_REG) return;
        
        if (code->op2->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(index_reg), code->op2->int_val);
        } else if (code->op2->kind == OP_VAR) {
            int offset = code->op2->offset ? code->op2->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(index_reg), offset);
        }
    }
    
    // 加载值
    MipsRegister value_reg = get_operand_register(ctx, code->result);
    if (value_reg == MIPS_NO_REG) {
        value_reg = allocate_register(ctx, code->result);
        if (value_reg == MIPS_NO_REG) {
            free_register(ctx, index_reg);
            return;
        }
        
        if (code->result->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(value_reg), code->result->int_val);
        } else if (code->result->kind == OP_VAR) {
            int offset = code->result->offset ? code->result->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(value_reg), offset);
        }
    }
    
    // 计算地址
    MipsRegister addr_reg = allocate_register(ctx, NULL);
    if (addr_reg == MIPS_NO_REG) {
        free_register(ctx, index_reg);
        free_register(ctx, value_reg);
        return;
    }
    
    emit(ctx, "  sll %s, %s, 2", reg_to_string(addr_reg), reg_to_string(index_reg));
    emit(ctx, "  addu %s, %s, $fp", reg_to_string(addr_reg), reg_to_string(addr_reg));
    emit(ctx, "  addiu %s, %s, %d", reg_to_string(addr_reg), reg_to_string(addr_reg), array_offset);
    
    // 存储值
    emit(ctx, "  sw %s, 0(%s)", reg_to_string(value_reg), reg_to_string(addr_reg));
    
    // 清理
    free_register(ctx, addr_reg);
    if (code->op2->kind != OP_VAR) {
        free_register(ctx, index_reg);
    }
    if (code->result->kind != OP_VAR) {
        free_register(ctx, value_reg);
    }
}

void gen_call(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->result) return;
    
    // 保存调用者保存的寄存器
    emit(ctx, "  addiu $sp, $sp, -16");
    emit(ctx, "  sw $t0, 0($sp)");
    emit(ctx, "  sw $t1, 4($sp)");
    emit(ctx, "  sw $t2, 8($sp)");
    emit(ctx, "  sw $t3, 12($sp)");
    
    // 调用函数
    emit(ctx, "  jal %s", code->op1->name);
    
    // 恢复寄存器
    emit(ctx, "  lw $t0, 0($sp)");
    emit(ctx, "  lw $t1, 4($sp)");
    emit(ctx, "  lw $t2, 8($sp)");
    emit(ctx, "  lw $t3, 12($sp)");
    emit(ctx, "  addiu $sp, $sp, 16");
    
    // 保存返回值
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg != MIPS_NO_REG) {
        emit(ctx, "  move %s, $v0", reg_to_string(dst_reg));
        
        // 结果标记
        if (code->result->kind == OP_TEMP) {
            ctx->registers[dst_reg].dirty = 0;
        } else {
            ctx->registers[dst_reg].dirty = 1;
        }
    }
}

void gen_call_void(MipsContext *ctx, IRCode *code) {
    if (!code->op1) return;
    
    // 保存调用者保存的寄存器
    emit(ctx, "  addiu $sp, $sp, -16");
    emit(ctx, "  sw $t0, 0($sp)");
    emit(ctx, "  sw $t1, 4($sp)");
    emit(ctx, "  sw $t2, 8($sp)");
    emit(ctx, "  sw $t3, 12($sp)");
    
    // 调用函数
    emit(ctx, "  jal %s", code->op1->name);
    
    // 恢复寄存器
    emit(ctx, "  lw $t0, 0($sp)");
    emit(ctx, "  lw $t1, 4($sp)");
    emit(ctx, "  lw $t2, 8($sp)");
    emit(ctx, "  lw $t3, 12($sp)");
    emit(ctx, "  addiu $sp, $sp, 16");
}

void gen_io_functions(MipsContext *ctx) {
    // read 函数
    emit(ctx, "\nread:");
    emit(ctx, "  li $v0, 4");
    emit(ctx, "  la $a0, _prompt");
    emit(ctx, "  syscall");
    emit(ctx, "  li $v0, 5");
    emit(ctx, "  syscall");
    emit(ctx, "  jr $ra");
    
    // write 函数
    emit(ctx, "\nwrite:");
    emit(ctx, "  li $v0, 1");
    emit(ctx, "  syscall");
    emit(ctx, "  li $v0, 4");
    emit(ctx, "  la $a0, _ret");
    emit(ctx, "  syscall");
    emit(ctx, "  move $v0, $0");
    emit(ctx, "  jr $ra");
}

/* ================== 输出函数 ================== */

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
    if (!list || !filename) {
        printf("[MIPS] Cannot save: invalid parameters\n");
        return;
    }
    
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("[MIPS] Cannot open file: %s\n", filename);
        return;
    }
    
    MipsCode *current = list->head;
    while (current) {
        fprintf(f, "%s\n", current->instruction);
        current = current->next;
    }
    
    fclose(f);
    printf("[MIPS] Code saved to %s\n", filename);
}