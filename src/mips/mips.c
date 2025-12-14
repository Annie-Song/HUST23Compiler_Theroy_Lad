#include "mips.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

void init_registers(MipsContext *ctx) {
    if (!ctx) return;
    
    for (int i = 0; i < MIPS_REG_COUNT; i++) {
        ctx->registers[i].reg = (MipsRegister)i;
        ctx->registers[i].operand = NULL;
        ctx->registers[i].dirty = 0;
        ctx->registers[i].used = 0;
    }
    
    // 保留特殊寄存器
    ctx->registers[MIPS_ZERO].used = 1;
    ctx->registers[MIPS_SP].used = 1;
    ctx->registers[MIPS_FP].used = 1;
    ctx->registers[MIPS_RA].used = 1;
    ctx->registers[MIPS_GP].used = 1;
}

MipsRegister allocate_register(MipsContext *ctx, Operand *op) {
    if (!ctx) return MIPS_NO_REG;
    
    // 优先分配 $t0-$t7
    for (int i = MIPS_T0; i <= MIPS_T7; i++) {
        if (!ctx->registers[i].used) {
            ctx->registers[i].used = 1;
            ctx->registers[i].operand = op;
            ctx->registers[i].dirty = 0;
            return (MipsRegister)i;
        }
    }
    
    // 然后分配 $s0-$s7
    for (int i = MIPS_S0; i <= MIPS_S7; i++) {
        if (!ctx->registers[i].used) {
            ctx->registers[i].used = 1;
            ctx->registers[i].operand = op;
            ctx->registers[i].dirty = 0;
            return (MipsRegister)i;
        }
    }
    
    // 最后分配 $t8-$t9
    for (int i = MIPS_T8; i <= MIPS_T9; i++) {
        if (!ctx->registers[i].used) {
            ctx->registers[i].used = 1;
            ctx->registers[i].operand = op;
            ctx->registers[i].dirty = 0;
            return (MipsRegister)i;
        }
    }
    
    return MIPS_NO_REG;
}

MipsRegister get_operand_register(MipsContext *ctx, Operand *op) {
    if (!ctx || !op) return MIPS_NO_REG;
    
    for (int i = 0; i < MIPS_REG_COUNT; i++) {
        if (ctx->registers[i].used && ctx->registers[i].operand == op) {
            return (MipsRegister)i;
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
    
    spill_register(ctx, reg);
    ctx->registers[reg].used = 0;
    ctx->registers[reg].operand = NULL;
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
            printf("[MIPS] Unsupported IR opcode: %d\n", code->op);
            break;
    }
}

/* ================== 具体指令生成 ================== */

void gen_function_prologue(MipsContext *ctx, IRCode *code) {
    if (!code->result) return;
    
    ctx->in_function = 1;
    ctx->current_func = code->result->name;
    ctx->next_offset = -4;  // 重置栈偏移
    
    emit(ctx, "\n%s:", code->result->name);
    
    if (strcmp(code->result->name, "main") == 0) {
        // 主函数序言
        emit(ctx, "  addiu $sp, $sp, -64");
        emit(ctx, "  sw $fp, 60($sp)");
        emit(ctx, "  sw $ra, 56($sp)");
        emit(ctx, "  move $fp, $sp");
    } else {
        // 普通函数序言
        emit(ctx, "  addiu $sp, $sp, -48");
        emit(ctx, "  sw $fp, 44($sp)");
        emit(ctx, "  sw $ra, 40($sp)");
        emit(ctx, "  move $fp, $sp");
    }
}

void gen_label(MipsContext *ctx, IRCode *code) {
    if (code->result && code->result->name) {
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
    
    // 获取条件值
    MipsRegister cond_reg = get_operand_register(ctx, code->op1);
    if (cond_reg == MIPS_NO_REG) {
        // 加载条件值
        cond_reg = allocate_register(ctx, code->op1);
        if (cond_reg == MIPS_NO_REG) return;
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(cond_reg), code->op1->int_val);
        } else if (code->op1->kind == OP_VAR) {
            int offset = code->op1->offset ? code->op1->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(cond_reg), offset);
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
    if (code->op1) {
        // 有返回值的返回
        MipsRegister ret_reg = get_operand_register(ctx, code->op1);
        if (ret_reg == MIPS_NO_REG) {
            ret_reg = allocate_register(ctx, code->op1);
            if (ret_reg != MIPS_NO_REG) {
                if (code->op1->kind == OP_CONST) {
                    emit(ctx, "  li %s, %d", reg_to_string(ret_reg), code->op1->int_val);
                } else if (code->op1->kind == OP_VAR) {
                    int offset = code->op1->offset ? code->op1->offset : -4;
                    emit(ctx, "  lw %s, %d($fp)", reg_to_string(ret_reg), offset);
                }
            }
        }
        
        if (ret_reg != MIPS_NO_REG) {
            emit(ctx, "  move $v0, %s", reg_to_string(ret_reg));
            free_register(ctx, ret_reg);
        }
    }
    
    // 函数尾声
    if (ctx->current_func && strcmp(ctx->current_func, "main") == 0) {
        emit(ctx, "  move $sp, $fp");
        emit(ctx, "  lw $ra, 56($sp)");
        emit(ctx, "  lw $fp, 60($sp)");
        emit(ctx, "  addiu $sp, $sp, 64");
        emit(ctx, "  jr $ra");
    } else {
        emit(ctx, "  move $sp, $fp");
        emit(ctx, "  lw $ra, 40($sp)");
        emit(ctx, "  lw $fp, 44($sp)");
        emit(ctx, "  addiu $sp, $sp, 48");
        emit(ctx, "  jr $ra");
    }
    
    ctx->in_function = 0;
    ctx->current_func = NULL;
}

void gen_assign(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->result) return;
    
    // 处理源操作数
    MipsRegister src_reg = get_operand_register(ctx, code->op1);
    if (src_reg == MIPS_NO_REG) {
        src_reg = allocate_register(ctx, code->op1);
        if (src_reg == MIPS_NO_REG) return;
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(src_reg), code->op1->int_val);
        } else if (code->op1->kind == OP_VAR) {
            int offset = code->op1->offset ? code->op1->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(src_reg), offset);
        }
    }
    
    // 处理目标操作数
    if (code->result->kind == OP_VAR) {
        // 变量：存储到内存
        if (code->result->offset == 0) {
            code->result->offset = ctx->next_offset;
            ctx->next_offset -= 4;
        }
        
        emit(ctx, "  sw %s, %d($fp)", reg_to_string(src_reg), code->result->offset);
        
        // 如果目标也在寄存器中，更新它
        MipsRegister dst_reg = get_operand_register(ctx, code->result);
        if (dst_reg != MIPS_NO_REG) {
            emit(ctx, "  move %s, %s", reg_to_string(dst_reg), reg_to_string(src_reg));
            ctx->registers[dst_reg].dirty = 1;
        }
    } else if (code->result->kind == OP_TEMP) {
        // 临时变量：移动到另一个寄存器
        MipsRegister dst_reg = allocate_register(ctx, code->result);
        if (dst_reg != MIPS_NO_REG) {
            emit(ctx, "  move %s, %s", reg_to_string(dst_reg), reg_to_string(src_reg));
            // 临时变量不需要写回
            ctx->registers[dst_reg].dirty = 0;
        }
    }
    
    // 清理源寄存器
    if (code->op1->kind != OP_VAR) {
        free_register(ctx, src_reg);
    }
}

void gen_binary_op(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->op2 || !code->result) return;
    
    // 获取左操作数
    MipsRegister reg1 = get_operand_register(ctx, code->op1);
    if (reg1 == MIPS_NO_REG) {
        reg1 = allocate_register(ctx, code->op1);
        if (reg1 == MIPS_NO_REG) return;
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(reg1), code->op1->int_val);
        } else if (code->op1->kind == OP_VAR) {
            int offset = code->op1->offset ? code->op1->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg1), offset);
        }
    }
    
    // 获取右操作数
    MipsRegister reg2;
    if (code->op2->kind == OP_CONST) {
        reg2 = allocate_register(ctx, NULL);
        if (reg2 == MIPS_NO_REG) {
            free_register(ctx, reg1);
            return;
        }
        emit(ctx, "  li %s, %d", reg_to_string(reg2), code->op2->int_val);
    } else {
        reg2 = get_operand_register(ctx, code->op2);
        if (reg2 == MIPS_NO_REG) {
            reg2 = allocate_register(ctx, code->op2);
            if (reg2 == MIPS_NO_REG) {
                free_register(ctx, reg1);
                return;
            }
            int offset = code->op2->offset ? code->op2->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg2), offset);
        }
    }
    
    // 分配结果寄存器
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg == MIPS_NO_REG) {
        free_register(ctx, reg1);
        if (code->op2->kind == OP_CONST) free_register(ctx, reg2);
        return;
    }
    
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
}

void gen_compare_op(MipsContext *ctx, IRCode *code) {
    if (!code->op1 || !code->op2 || !code->result) return;
    
    // 获取左操作数
    MipsRegister reg1 = get_operand_register(ctx, code->op1);
    if (reg1 == MIPS_NO_REG) {
        reg1 = allocate_register(ctx, code->op1);
        if (reg1 == MIPS_NO_REG) return;
        
        if (code->op1->kind == OP_CONST) {
            emit(ctx, "  li %s, %d", reg_to_string(reg1), code->op1->int_val);
        } else if (code->op1->kind == OP_VAR) {
            int offset = code->op1->offset ? code->op1->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg1), offset);
        }
    }
    
    // 获取右操作数
    MipsRegister reg2;
    if (code->op2->kind == OP_CONST) {
        reg2 = allocate_register(ctx, NULL);
        if (reg2 == MIPS_NO_REG) {
            free_register(ctx, reg1);
            return;
        }
        emit(ctx, "  li %s, %d", reg_to_string(reg2), code->op2->int_val);
    } else {
        reg2 = get_operand_register(ctx, code->op2);
        if (reg2 == MIPS_NO_REG) {
            reg2 = allocate_register(ctx, code->op2);
            if (reg2 == MIPS_NO_REG) {
                free_register(ctx, reg1);
                return;
            }
            int offset = code->op2->offset ? code->op2->offset : -4;
            emit(ctx, "  lw %s, %d($fp)", reg_to_string(reg2), offset);
        }
    }
    
    // 分配结果寄存器
    MipsRegister dst_reg = allocate_register(ctx, code->result);
    if (dst_reg == MIPS_NO_REG) {
        free_register(ctx, reg1);
        if (code->op2->kind == OP_CONST) free_register(ctx, reg2);
        return;
    }
    
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
    }
    
    // 清理
    if (code->op2->kind == OP_CONST) {
        free_register(ctx, reg2);
    }
    if (code->op1->kind != OP_VAR) {
        free_register(ctx, reg1);
    }
    
    // 结果标记为临时变量
    ctx->registers[dst_reg].dirty = 0;
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