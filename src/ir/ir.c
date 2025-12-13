#include "ir.h"
#include "../ast/ast.h"
#include "../symbol/symbol.h"
#include "../semantic/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern SymbolTable *symbol_table;

/* 全局计数器 */
static int temp_counter = 1;
static int label_counter = 1;

/* ================== 循环上下文结构 ================== */

/* 循环上下文结构 */
typedef struct LoopContext {
    char *break_label;     /* break跳转的目标标签 */
    char *continue_label;  /* continue跳转的目标标签 */
    struct LoopContext *prev;  /* 前一个循环上下文（用于嵌套循环） */
} LoopContext;

/* 全局循环上下文栈 */
static LoopContext *loop_context = NULL;

/* 进入循环 */
static void enter_loop(char *break_label, char *continue_label) {
    LoopContext *ctx = (LoopContext *)malloc(sizeof(LoopContext));
    if (!ctx) {
        printf("[ERROR] Failed to allocate memory for loop context\n");
        return;
    }
    
    ctx->break_label = strdup(break_label);
    ctx->continue_label = strdup(continue_label);
    ctx->prev = loop_context;
    loop_context = ctx;
    
    printf("[IR DEBUG] Entered loop: break=%s, continue=%s\n", 
           break_label, continue_label);
}

/* 退出循环 */
static void exit_loop(void) {
    if (loop_context) {
        LoopContext *prev = loop_context->prev;
        
        printf("[IR DEBUG] Exited loop: break=%s, continue=%s\n", 
               loop_context->break_label, loop_context->continue_label);
        
        free(loop_context->break_label);
        free(loop_context->continue_label);
        free(loop_context);
        loop_context = prev;
        
        if (prev) {
            printf("[IR DEBUG] Restored outer loop: break=%s, continue=%s\n", 
                   prev->break_label, prev->continue_label);
        }
    } else {
        printf("[IR DEBUG] WARNING: exit_loop called with no active loop\n");
    }
}

/* ================== 函数声明 ================== */
static void gen_ir_func_def(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_params(ASTNode *node, IRList *ir_list);
static void gen_ir_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_args(ASTNode *node, IRList *ir_list, SymbolTable *symtab);

/* 将所有函数声明改为 static */
static void gen_ir_binary_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_assign_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_func_call(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_if_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_if_else_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_while_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_for_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_init_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_def_dec_list(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_array_access(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_dec_list(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_array_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_compound_stmt(ASTNode *node, IRList *ir_list, SymbolTable *symtab);

/* ================== 操作数创建函数 ================== */

Operand *new_const_operand_int(int val) {
    Operand *op = (Operand *)malloc(sizeof(Operand));
    if (!op) return NULL;
    
    op->kind = OP_CONST;
    op->int_val = val;
    op->type = new_basic_type(TYPE_INT);
    op->offset = 0;
    return op;
}

Operand *new_const_operand_float(float val) {
    Operand *op = (Operand *)malloc(sizeof(Operand));
    if (!op) return NULL;
    
    op->kind = OP_CONST;
    op->float_val = val;
    op->type = new_basic_type(TYPE_FLOAT);
    op->offset = 0;
    return op;
}

Operand *new_var_operand(SymbolEntry *sym) {
    if (!sym) return NULL;
    
    Operand *op = (Operand *)malloc(sizeof(Operand));
    if (!op) return NULL;
    
    op->kind = OP_VAR;
    op->name = strdup(sym->alias);
    op->type = copy_type(sym->type);
    op->offset = sym->offset;
    return op;
}

/* 创建临时变量操作数 */
Operand *new_temp_operand(Type *type) {
    Operand *op = (Operand *)malloc(sizeof(Operand));
    if (!op) {
        printf("[ERROR] Failed to allocate memory for temp operand\n");
        return NULL;
    }
    
     // 初始化所有字段
    memset(op, 0, sizeof(Operand));

    op->kind = OP_VAR;
    char name[32];
    snprintf(name, sizeof(name), "t%d", temp_counter++);
    op->name = strdup(name);
    if (!op->name) {
        printf("[ERROR] Failed to allocate name for temp operand\n");
        free(op);
        return NULL;
    }
    
    // 1. 检查type是否为NULL
    if (!type) {
        printf("[DEBUG] Temp operand type is NULL, using int as default\n");
        op->type = new_basic_type(TYPE_INT);
        op->offset = 0;
        printf("[DEBUG] Created temp operand %s with default int type\n", op->name);
        return op;
    }
    
    // 2. 检查type的kind是否有效 - 更严格的检查
    // 定义有效的类型种类范围
    int valid_kind = 0;
    if (type->kind == TK_BASIC || type->kind == TK_ARRAY || 
        type->kind == TK_FUNCTION || type->kind == TK_STRUCT) {
        valid_kind = 1;
    }
    
    if (!valid_kind) {
        printf("[DEBUG] WARNING: Temp operand has invalid type kind=%d, using int\n", 
               type->kind);
        
        // 尝试通过basic字段判断是否为BASIC类型
        if (type->basic >= TYPE_VOID && type->basic <= TYPE_CHAR) {
            printf("[DEBUG] Type appears to be BASIC with basic=%d, correcting kind\n", type->basic);
            type->kind = TK_BASIC;
            op->type = copy_type(type);
        } else {
            op->type = new_basic_type(TYPE_INT);
        }
        
        if (!op->type) {
            op->type = new_basic_type(TYPE_INT);
        }
        
        op->offset = 0;
        printf("[DEBUG] Created temp operand %s with corrected type\n", op->name);
        return op;
    }
    
    /* 根据类型种类处理 */
    switch (type->kind) {
        case TK_BASIC:
            /* 基本类型：直接复制 */
            printf("[DEBUG] Temp operand has BASIC type, basic=%d\n", type->basic);
            op->type = copy_type(type);
            if (!op->type) {
                printf("[DEBUG] Failed to copy basic type, using int\n");
                op->type = new_basic_type(TYPE_INT);
            }
            break;
            
        case TK_ARRAY:
            /* 数组类型：使用数组元素类型 */
            printf("[DEBUG] Temp operand has ARRAY type\n");
            
            // 检查数组元素类型是否有效
            if (!type->array.elem) {
                printf("[DEBUG] Array element type is NULL, using int\n");
                op->type = new_basic_type(TYPE_INT);
                break;
            }
            
            // 检查数组元素类型的kind是否有效
            if (type->array.elem->kind < TK_BASIC || type->array.elem->kind > TK_STRUCT) {
                printf("[DEBUG] Array element has invalid type kind=%d, using int\n", 
                       type->array.elem->kind);
                op->type = new_basic_type(TYPE_INT);
                break;
            }
            
            // 处理数组元素类型
            if (type->array.elem->kind == TK_ARRAY) {
                /* 如果是多维数组，递归处理以获取最终的元素类型 */
                printf("[DEBUG] Multi-dimensional array, recursing for element type\n");
                Operand *elem_op = new_temp_operand(type->array.elem);
                if (elem_op && elem_op->type) {
                    op->type = elem_op->type;
                    // 释放临时操作数但不释放类型
                    free(elem_op->name);
                    free(elem_op);
                } else {
                    printf("[DEBUG] Failed to get element type from multi-dimensional array, using int\n");
                    op->type = new_basic_type(TYPE_INT);
                }
            } else {
                /* 单维数组：复制元素类型 */
                op->type = copy_type(type->array.elem);
                if (!op->type) {
                    printf("[DEBUG] Failed to copy array element type, using int\n");
                    op->type = new_basic_type(TYPE_INT);
                }
            }
            break;
            
        case TK_FUNCTION:
            /* 函数类型不能用作临时变量的类型，使用返回类型或默认int */
            printf("[DEBUG] Temp operand has FUNCTION type\n");
            
            if (type->func.return_type && 
                type->func.return_type->kind >= TK_BASIC && 
                type->func.return_type->kind <= TK_STRUCT) {
                // 使用函数的返回类型
                printf("[DEBUG] Using function return type\n");
                op->type = copy_type(type->func.return_type);
                if (!op->type) {
                    printf("[DEBUG] Failed to copy function return type, using int\n");
                    op->type = new_basic_type(TYPE_INT);
                }
            } else {
                // 无法获取有效返回类型，使用int
                printf("[DEBUG] No valid function return type, using int\n");
                op->type = new_basic_type(TYPE_INT);
            }
            break;
            
        case TK_STRUCT:
            /* 结构体类型：复制类型 */
            printf("[DEBUG] Temp operand has STRUCT type\n");
            op->type = copy_type(type);
            if (!op->type) {
                printf("[DEBUG] Failed to copy struct type, using int\n");
                op->type = new_basic_type(TYPE_INT);
            }
            break;
            
        default:
            /* 未知类型：使用int */
            printf("[DEBUG] Temp operand has unknown type kind=%d, using int\n", type->kind);
            op->type = new_basic_type(TYPE_INT);
            break;
    }
    
    /* ========== 最终验证 ========== */
    
    // 确保op->type不为NULL
    if (!op->type) {
        printf("[DEBUG] WARNING: op->type became NULL, using int\n");
        op->type = new_basic_type(TYPE_INT);
    }
    
    op->offset = 0;
    
    // 调试输出最终类型信息
    if (op->type) {
        printf("[DEBUG] Created temp operand %s with type: kind=%d", 
               op->name, op->type->kind);
        if (op->type->kind == TK_BASIC) {
            printf(", basic=%d", op->type->basic);
        } else if (op->type->kind == TK_ARRAY) {
            printf(", array element kind=%d", 
                   op->type->array.elem ? op->type->array.elem->kind : -1);
        }
        printf("\n");
    } else {
        printf("[DEBUG] ERROR: Failed to set type for temp operand %s\n", op->name);
    }
    
    return op;
}

Operand *new_label_operand(void) {
    Operand *op = (Operand *)malloc(sizeof(Operand));
    if (!op) return NULL;
    
    op->kind = OP_LABEL;
    char name[32];
    snprintf(name, sizeof(name), "L%d", label_counter++);
    op->name = strdup(name);
    op->type = NULL;
    op->offset = 0;
    return op;
}

/* ================== 中间代码链表操作 ================== */

IRList *new_ir_list(void) {
    IRList *list = (IRList *)malloc(sizeof(IRList));
    if (!list) return NULL;
    
    list->head = list->tail = NULL;
    list->count = 0;
    return list;
}

IRCode *new_ir_code(IROpCode op, Operand *op1, Operand *op2, Operand *result) {
    IRCode *code = (IRCode *)malloc(sizeof(IRCode));
    if (!code) return NULL;
    
    code->op = op;
    code->op1 = op1;
    code->op2 = op2;
    code->result = result;
    code->prev = code->next = NULL;
    return code;
}

void append_ir_code(IRList *list, IRCode *code) {
    if (!list || !code) return;
    
    if (!list->head) {
        list->head = list->tail = code;
    } else {
        list->tail->next = code;
        code->prev = list->tail;
        list->tail = code;
    }
    list->count++;
}

IRList *merge_ir_lists(int count, ...) {
    va_list args;
    va_start(args, count);
    
    IRList *result = new_ir_list();
    if (!result) {
        va_end(args);
        return NULL;
    }
    
    for (int i = 0; i < count; i++) {
        IRList *list = va_arg(args, IRList *);
        if (!list || !list->head) continue;
        
        if (!result->head) {
            result->head = list->head;
            result->tail = list->tail;
        } else {
            result->tail->next = list->head;
            list->head->prev = result->tail;
            result->tail = list->tail;
        }
        result->count += list->count;
    }
    
    va_end(args);
    return result;
}

/* ================== 显示函数 ================== */

void print_operand(Operand *op) {
    if (!op) {
        printf("NULL");
        return;
    }
    
    switch (op->kind) {
        case OP_CONST:
            if (op->type && op->type->kind == TK_BASIC) {
                if (op->type->basic == TYPE_INT)
                    printf("%d", op->int_val);
                else if (op->type->basic == TYPE_FLOAT)
                    printf("%.2f", op->float_val);
            }
            break;
        case OP_VAR:
        case OP_TEMP:  // 新增：处理临时变量
            printf("%s", op->name);
            break;
        case OP_LABEL:
        case OP_FUNC:
            printf("%s", op->name);
            break;
        case OP_NONE:
            printf("-");
            break;
    }
}

void print_ir_code(IRCode *code) {
    if (!code) return;
    
    switch (code->op) {
        case IR_LABEL:
            printf("%s:\n", code->result->name);
            break;
        case IR_FUNCTION:
            printf("FUNCTION %s:\n", code->result->name);
            break;
        case IR_ASSIGN:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf("\n");
            break;
        case IR_ADD:
        case IR_SUB:
        case IR_MUL:
        case IR_DIV:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" %c ", 
                   code->op == IR_ADD ? '+' : 
                   code->op == IR_SUB ? '-' : 
                   code->op == IR_MUL ? '*' : '/');
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_LT:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" < ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_LE:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" <= ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_GT:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" > ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_GE:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" >= ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_EQ:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" == ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_NE:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            print_operand(code->op1);
            printf(" != ");
            print_operand(code->op2);
            printf("\n");
            break;
        case IR_GOTO:
            printf("  GOTO %s\n", code->result->name);
            break;
        case IR_IF:
            printf("  IF ");
            print_operand(code->op1);
            printf(" != 0 GOTO ");
            print_operand(code->result);
            printf("\n");
            break;
        case IR_RETURN:
            if (code->op1) {
                printf("  RETURN ");
                print_operand(code->op1);
                printf("\n");
            } else {
                printf("  RETURN\n");
            }
            break;
        case IR_ARG:
            printf("  ARG ");
            print_operand(code->op1);
            printf("\n");
            break;
        case IR_PARAM:
            printf("  PARAM ");
            print_operand(code->op1);
            printf("\n");
            break;
        case IR_CALL:
            printf("  ");
            print_operand(code->result);
            printf(" := CALL %s\n", code->op1->name);
            break;
        case IR_CALL_VOID:
            printf("  CALL %s\n", code->op1->name);
            break;
        case IR_READ:
            printf("  READ ");
            print_operand(code->result);
            printf("\n");
            break;
        case IR_WRITE:
            printf("  WRITE ");
            print_operand(code->op1);
            printf("\n");
            break;
        case IR_ARRAY_ALLOC:
            printf("  ARRAY_ALLOC ");
            if (code->result && code->result->name) {
                printf("%s", code->result->name);
            }
            printf("[");
            if (code->op1) {
                print_operand(code->op1);
            }
            printf("]\n");
            break;
            
        case IR_ARRAY_LOAD:
            printf("  ");
            print_operand(code->result);
            printf(" := ");
            if (code->op1 && code->op1->name) {
                printf("%s", code->op1->name);
            } else {
                printf("array");
            }
            printf("[");
            if (code->op2) {
                print_operand(code->op2);
            }
            printf("]\n");
            break;
            
        case IR_ARRAY_STORE:
            printf("  ");
            if (code->op1 && code->op1->name) {
                printf("%s", code->op1->name);
            } else {
                printf("array");
            }
            printf("[");
            if (code->op2) {
                print_operand(code->op2);
            }
            printf("] := ");
            if (code->result) {
                print_operand(code->result);
            }
            printf("\n");
            break;
            
        case IR_ADDRESS:
            printf("  ");
            print_operand(code->result);
            printf(" = &");
            print_operand(code->op1);
            printf("\n");
            break;
            
        case IR_DEREF:
            printf("  ");
            print_operand(code->result);
            printf(" = *");
            print_operand(code->op1);
            printf("\n");
            break;

    }
}

void print_ir_list(IRList *list) {
    if (!list || !list->head) {
        printf("Empty IR list\n");
        return;
    }
    
    printf("\n=== Intermediate Code (TAC) ===\n");
    
    IRCode *current = list->head;
    while (current) {
        print_ir_code(current);
        current = current->next;
    }
}

/* ================== 中间代码生成主函数 ================== */

IRList *gen_ir_from_ast(ASTNode *node, SymbolTable *symtab) {
    if (!node) return NULL;
    
    IRList *ir_list = new_ir_list();
    if (!ir_list) return NULL;
    
    // 修复：优先使用全局符号表
    SymbolTable *table_to_use = NULL;
    
    if (symtab) {
        table_to_use = symtab;  // 如果传入了符号表，使用它
    } else if (symbol_table) {
        table_to_use = symbol_table;  // 否则使用全局符号表
    } else {
        printf("[ERROR] No symbol table available for IR generation\n");
        free(ir_list);
        return NULL;
    }
    
    printf("[IR DEBUG] Using symbol table for IR generation\n");
    gen_ir_program(node, ir_list, table_to_use);
    return ir_list;
}

void gen_ir_program(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    switch (node->kind) {
        case EXT_DEF_LIST:
            if (node->ptr[0]) gen_ir_program(node->ptr[0], ir_list, symtab);
            if (node->ptr[1]) gen_ir_program(node->ptr[1], ir_list, symtab);
            break;
            
        case EXT_VAR_DEF:
            // 全局变量初始化 - 暂不处理
            break;
            
        case FUNC_DEF:
            gen_ir_func_def(node, ir_list, symtab);
            break;
            
        default:
            for (int i = 0; i < 4; i++) {
                if (node->ptr[i]) gen_ir_program(node->ptr[i], ir_list, symtab);
            }
            break;
    }
}

/* 生成函数定义的中间代码 */
static void gen_ir_func_def(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || node->kind != FUNC_DEF || !ir_list) return;
    
    ASTNode *func_dec = node->ptr[1];
    if (!func_dec) return;
    
    // 获取函数名
    char *func_name = "unknown";
    if (func_dec->ptr[0] ) {
        func_name = func_dec->ptr[0]->type_id;
    }
    
    // 生成FUNCTION指令
    Operand *func_op = (Operand *)malloc(sizeof(Operand));
    if (func_op) {
        func_op->kind = OP_FUNC;
        func_op->name = strdup(func_name);
        
        IRCode *func_code = new_ir_code(IR_FUNCTION, NULL, NULL, func_op);
        if (func_code) {
            append_ir_code(ir_list, func_code);
        }
    }
    
    // 生成函数参数
    if (func_dec->ptr[1]) {
        gen_ir_params(func_dec->ptr[1], ir_list);
    }
    
    // 生成函数体
    ASTNode *func_body = node->ptr[2];
    if (func_body) {
        gen_ir_stmt(func_body, ir_list, NULL, symtab);
    }
}

/* 生成函数参数的中间代码 */
static void gen_ir_params(ASTNode *node, IRList *ir_list) {
    if (!node || !ir_list) return;
    
    if (node->kind == VAR_LIST) {
        if (node->ptr[0]) {
            ASTNode *param_dec = node->ptr[0];
            if (param_dec->kind == PARAM_DEC && param_dec->ptr[1]) {
                ASTNode *param_name = param_dec->ptr[1];
                if (param_name->kind == ID_NODE) {
                    Operand *param_op = (Operand *)malloc(sizeof(Operand));
                    if (param_op) {
                        param_op->kind = OP_VAR;
                        param_op->name = strdup(param_name->type_id);
                        
                        IRCode *param_code = new_ir_code(IR_PARAM, param_op, NULL, NULL);
                        if (param_code) {
                            append_ir_code(ir_list, param_code);
                        }
                    }
                }
            }
        }
        
        if (node->ptr[1]) {
            gen_ir_params(node->ptr[1], ir_list);
        }
    } else if (node->kind == PARAM_DEC) {
        // 单个参数
        if (node->ptr[1]) {
            ASTNode *param_name = node->ptr[1];
            if (param_name->kind == ID_NODE) {
                Operand *param_op = (Operand *)malloc(sizeof(Operand));
                if (param_op) {
                    param_op->kind = OP_VAR;
                    param_op->name = strdup(param_name->type_id);
                    
                    IRCode *param_code = new_ir_code(IR_PARAM, param_op, NULL, NULL);
                    if (param_code) {
                        append_ir_code(ir_list, param_code);
                    }
                }
            }
        }
    }
}

/* 生成语句的中间代码 */
static void gen_ir_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    printf("[IR DEBUG] Generating IR for statement kind: %d\n", node->kind);
    
    switch (node->kind) {
        case ASSIGN_EXP:  // 新增：处理赋值表达式语句
            printf("[IR DEBUG] Processing ASSIGN_EXP as statement\n");
            if (node->ptr[0]) {
                Type *expr_type = node->type_info;
                if (!expr_type) expr_type = new_basic_type(TYPE_INT);
                
                Operand *temp_result = new_temp_operand(expr_type);
                if (temp_result) {
                    gen_ir_expr(node, ir_list, temp_result, symtab);
                }
            }
            break;
        
        case DEF:
            printf("[IR DEBUG] Processing DEF statement\n");
            // 处理变量定义
            if (node->ptr[0] && node->ptr[1]) {
                ASTNode *dec_list = node->ptr[1];
                if (dec_list) {
                    gen_ir_dec_list(dec_list, ir_list, symtab);
                }
            }
            break;

        case DEF_LIST:  // 新增：处理定义列表
            printf("[IR DEBUG] Processing DEF_LIST\n");
            if (node->ptr[0]) {
                // 处理第一个定义
                gen_ir_stmt(node->ptr[0], ir_list, next_label, symtab);
            }
            if (node->ptr[1]) {
                // 处理剩余的定义列表
                gen_ir_stmt(node->ptr[1], ir_list, next_label, symtab);
            }
            break;
            
        case COMP_ST:
            printf("[IR DEBUG] Processing COMP_ST statement\n");
            // 使用新的复合语句处理函数
            gen_ir_compound_stmt(node, ir_list, symtab);
            break;
            
        case EXP_STMT:
            printf("[IR DEBUG] Processing EXP_STMT\n");
            if (node->ptr[0]) {
                Type *expr_type = node->ptr[0]->type_info;
                if (!expr_type) expr_type = new_basic_type(TYPE_INT);
                
                Operand *temp_result = new_temp_operand(expr_type);
                if (temp_result) {
                    gen_ir_expr(node->ptr[0], ir_list, temp_result, symtab);
                }
            }
            break;
        
        case RETURN_STMT:
            printf("[IR DEBUG] Processing RETURN_STMT\n");
            if (node->ptr[0]) {
                Type *expr_type = node->ptr[0]->type_info;
                if (!expr_type) expr_type = new_basic_type(TYPE_INT);
                
                Operand *ret_val = new_temp_operand(expr_type);
                if (ret_val) {
                    gen_ir_expr(node->ptr[0], ir_list, ret_val, symtab);
                    
                    IRCode *ret_code = new_ir_code(IR_RETURN, ret_val, NULL, NULL);
                    if (ret_code) {
                        append_ir_code(ir_list, ret_code);
                    }
                }
            } else {
                IRCode *ret_code = new_ir_code(IR_RETURN, NULL, NULL, NULL);
                if (ret_code) {
                    append_ir_code(ir_list, ret_code);
                }
            }
            break;
            
        case IF_STMT:
            printf("[IR DEBUG] Processing IF_STMT\n");
            gen_ir_if_stmt(node, ir_list, next_label, symtab);
            break;
            
        case IF_ELSE_STMT:  // 新增：处理 if-else 语句
            printf("[IR DEBUG] Processing IF_ELSE_STMT\n");
            gen_ir_if_else_stmt(node, ir_list, next_label, symtab);
            break;
            
        case WHILE_STMT:
            printf("[IR DEBUG] Processing WHILE_STMT\n");
            gen_ir_while_stmt(node, ir_list, next_label, symtab);
            break;
            
        case FOR_STMT:  // 新增：处理 for 语句
            printf("[IR DEBUG] Processing FOR_STMT\n");
            gen_ir_for_stmt(node, ir_list, next_label, symtab);
            break;
            
        case BREAK_STMT:
            printf("[IR DEBUG] Processing BREAK_STMT\n");
            if (loop_context && loop_context->break_label) {
                // 生成跳转到break标签的指令
                Operand *break_label_op = (Operand *)malloc(sizeof(Operand));
                if (break_label_op) {
                    break_label_op->kind = OP_LABEL;
                    break_label_op->name = strdup(loop_context->break_label);
                    IRCode *goto_break = new_ir_code(IR_GOTO, NULL, NULL, break_label_op);
                    if (goto_break) {
                        append_ir_code(ir_list, goto_break);
                    }
                }
            } else {
                printf("[IR DEBUG] WARNING: break statement outside loop\n");
            }
            break;
            
        case CONTINUE_STMT:
            printf("[IR DEBUG] Processing CONTINUE_STMT\n");
            if (loop_context && loop_context->continue_label) {
                // 生成跳转到continue标签的指令
                Operand *continue_label_op = (Operand *)malloc(sizeof(Operand));
                if (continue_label_op) {
                    continue_label_op->kind = OP_LABEL;
                    continue_label_op->name = strdup(loop_context->continue_label);
                    IRCode *goto_continue = new_ir_code(IR_GOTO, NULL, NULL, continue_label_op);
                    if (goto_continue) {
                        append_ir_code(ir_list, goto_continue);
                    }
                }
            } else {
                printf("[IR DEBUG] WARNING: continue statement outside loop\n");
            }
            break;
            
        case STMT_LIST:
            printf("[IR DEBUG] Processing STMT_LIST\n");
            printf("[IR DEBUG] STMT_LIST ptr[0] = %p, ptr[1] = %p\n", 
                   (void*)node->ptr[0], (void*)node->ptr[1]);
            
            // 关键修复：确保所有语句都被处理
            if (node->ptr[0]) {
                printf("[IR DEBUG] Processing first statement in STMT_LIST\n");
                gen_ir_stmt(node->ptr[0], ir_list, next_label, symtab);
            } else {
                printf("[IR DEBUG] First statement in STMT_LIST is NULL\n");
            }
            
            if (node->ptr[1]) {
                printf("[IR DEBUG] Processing remaining statements in STMT_LIST\n");
                gen_ir_stmt(node->ptr[1], ir_list, next_label, symtab);
            } else {
                printf("[IR DEBUG] No more statements in STMT_LIST\n");
            }
            break;
            
        default:
            printf("[IR DEBUG] Unhandled statement kind: %d\n", node->kind);
            // 尝试递归处理
            for (int i = 0; i < 4; i++) {
                if (node->ptr[i]) {
                    printf("[IR DEBUG] Recursing to child %d\n", i);
                    gen_ir_stmt(node->ptr[i], ir_list, next_label, symtab);
                }
            }
            break;
    }
}

/* 生成初始化声明的中间代码 */
static void gen_ir_init_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || node->kind != INIT_DEC || !ir_list) return;
    
    printf("[IR DEBUG] Processing INIT_DEC\n");
    
    ASTNode *var_node = node->ptr[0];
    ASTNode *init_expr = node->ptr[1];
    
    if (!var_node || !init_expr) return;
    
    // ========== 修复：正确处理函数调用的类型 ==========
    Type *init_type = NULL;
    
    if (init_expr->kind == FUNC_CALL) {
        printf("[IR DEBUG] INIT_DEC: init_expr is FUNC_CALL, special handling\n");
        
        // 对于函数调用，我们需要获取函数的返回类型
        // 首先尝试从type_info获取
        if (init_expr->type_info) {
            printf("[IR DEBUG] FUNC_CALL has type_info kind=%d\n", init_expr->type_info->kind);
            
            // 如果type_info是函数类型，提取返回类型
            if (init_expr->type_info->kind == TK_FUNCTION) {
                if (init_expr->type_info->func.return_type) {
                    printf("[IR DEBUG] Extracting return type from function type\n");
                    init_type = copy_type(init_expr->type_info->func.return_type);
                } else {
                    printf("[IR DEBUG] No return type in function type, using int\n");
                    init_type = new_basic_type(TYPE_INT);
                }
            }
            // 如果type_info是数组类型，但表达式是函数调用，这是错误情况
            else if (init_expr->type_info->kind == TK_ARRAY) {
                printf("[IR DEBUG] ERROR: Function call has ARRAY type! Using int instead.\n");
                init_type = new_basic_type(TYPE_INT);
            }
            // 其他情况（BASIC类型），直接使用
            else {
                init_type = copy_type(init_expr->type_info);
            }
        } else {
            printf("[IR DEBUG] FUNC_CALL has no type_info, using int\n");
            init_type = new_basic_type(TYPE_INT);
        }
    } else {
        // 非函数调用表达式，正常处理
        init_type = init_expr->type_info ? copy_type(init_expr->type_info) : new_basic_type(TYPE_INT);
    }
    
    // 确保init_type不为NULL
    if (!init_type) {
        printf("[IR DEBUG] WARNING: init_type is NULL, using int\n");
        init_type = new_basic_type(TYPE_INT);
    }
    
    printf("[IR DEBUG] Final init_type: kind=%d\n", init_type->kind);
    
    Operand *init_temp = new_temp_operand(init_type);
    if (!init_temp) return;
    
    gen_ir_expr(init_expr, ir_list, init_temp, symtab);
    
    // 如果是变量定义，将初始化值赋给变量
    if (var_node->kind == ID_NODE && var_node->symbol_ref) {
        Operand *var_op = new_var_operand(var_node->symbol_ref);
        if (var_op) {
            IRCode *assign_code = new_ir_code(IR_ASSIGN, init_temp, NULL, var_op);
            if (assign_code) {
                append_ir_code(ir_list, assign_code);
            }
        }
    }
}

/* 生成表达式的中间代码 */
static void gen_ir_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list || !result) return;
    
    printf("[IR DEBUG] gen_ir_expr for node kind=%d\n", node->kind);
    
    switch (node->kind) {
        case ID_NODE:
            printf("[IR DEBUG] Processing identifier: %s\n", node->type_id);
            printf("[IR DEBUG] Node has type_info: %s\n", node->type_info ? "YES" : "NO");
            if (node->type_info) {
                printf("[IR DEBUG] type_info kind=%d\n", node->type_info->kind);
                if (node->type_info->kind == TK_ARRAY) {
                    printf("[IR DEBUG] WARNING: identifier %s has array type!\n", node->type_id);
                }
            }
            printf("[IR DEBUG] Node has symbol_ref: %s\n", node->symbol_ref ? "YES" : "NO");
            
            Operand *src_op = NULL;
            
            // 如果已有 symbol_ref，使用它
            if (node->symbol_ref) {
                printf("[IR DEBUG] Using existing symbol_ref\n");
                src_op = new_var_operand(node->symbol_ref);
            } else {
                // 如果 symbol_ref 为 NULL，创建临时操作数
                printf("[IR DEBUG] Symbol ref is NULL for %s, creating var operand\n", node->type_id);
                
                // 创建一个临时的变量操作数
                src_op = (Operand *)malloc(sizeof(Operand));
                if (src_op) {
                    src_op->kind = OP_VAR;
                    src_op->name = strdup(node->type_id);
                    // 使用节点的类型信息，如果没有则用int
                    if (node->type_info) {
                        src_op->type = copy_type(node->type_info);
                    } else {
                        src_op->type = new_basic_type(TYPE_INT);
                    }
                    src_op->offset = 0;
                }
            }
            
            if (src_op) {
                IRCode *assign_code = new_ir_code(IR_ASSIGN, src_op, NULL, result);
                if (assign_code) {
                    append_ir_code(ir_list, assign_code);
                }
            } else {
                printf("[ERROR] Failed to create operand for identifier: %s\n", node->type_id);
                // 如果连操作数都创建失败，使用常量0
                Operand *const_op = new_const_operand_int(0);
                if (const_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
            break;
            
        case INT_NODE:
            {
                printf("[IR DEBUG] Processing integer constant: %d\n", node->type_int);
                Operand *const_op = new_const_operand_int(node->type_int);
                if (const_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
            break;
            
        case FLOAT_NODE:
            {
                printf("[IR DEBUG] Processing float constant: %f\n", node->type_float);
                Operand *const_op = new_const_operand_float(node->type_float);
                if (const_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
            break;
            
        case CHAR_NODE:
            {
                printf("[IR DEBUG] Processing char constant: %c\n", (char)node->type_int);
                // 将字符转换为整数
                Operand *const_op = new_const_operand_int(node->type_int);
                if (const_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
            break;
            
        case BINARY_EXP:
            printf("[IR DEBUG] Processing BINARY_EXP\n");
            gen_ir_binary_expr(node, ir_list, result, symtab);
            break;
            
        case ASSIGN_EXP:
            printf("[IR DEBUG] Processing ASSIGN_EXP\n");
            gen_ir_assign_expr(node, ir_list, result, symtab);
            break;
            
        case FUNC_CALL:
            printf("[IR DEBUG] Processing FUNC_CALL\n");
            gen_ir_func_call(node, ir_list, result, symtab);
            break;
            
        case UNARY_EXP:  // 处理一元表达式
            printf("[IR DEBUG] Processing UNARY_EXP\n");
            if (node->ptr[0]) {
                // 为操作数生成临时变量
                Type *expr_type = node->ptr[0]->type_info;
                if (!expr_type) expr_type = new_basic_type(TYPE_INT);
                
                Operand *expr_temp = new_temp_operand(expr_type);
                if (expr_temp) {
                    gen_ir_expr(node->ptr[0], ir_list, expr_temp, symtab);
                    
                    // 根据一元操作符生成代码
                    if (strcmp(node->type_id, "-") == 0) {
                        printf("[IR DEBUG] Unary operator: -\n");
                        // 负号：result = 0 - expr
                        Operand *zero = new_const_operand_int(0);
                        IRCode *neg_code = new_ir_code(IR_SUB, zero, expr_temp, result);
                        if (neg_code) {
                            append_ir_code(ir_list, neg_code);
                        }
                    } else if (strcmp(node->type_id, "!") == 0) {
                        printf("[IR DEBUG] Unary operator: !\n");
                        // 逻辑非：result = expr == 0 ? 1 : 0
                        // 这里简化处理：result = expr == 0
                        Operand *zero = new_const_operand_int(0);
                        Operand *one = new_const_operand_int(1);
                        
                        // 创建临时变量用于比较
                        Operand *cmp_temp = new_temp_operand(new_basic_type(TYPE_INT));
                        if (cmp_temp) {
                            // 比较：cmp_temp = (expr == 0)
                            IRCode *eq_code = new_ir_code(IR_EQ, expr_temp, zero, cmp_temp);
                            if (eq_code) {
                                append_ir_code(ir_list, eq_code);
                            }
                            // 赋值给结果
                            IRCode *assign_code = new_ir_code(IR_ASSIGN, cmp_temp, NULL, result);
                            if (assign_code) {
                                append_ir_code(ir_list, assign_code);
                            }
                        }
                    } else {
                        printf("[IR DEBUG] Unknown unary operator: %s, using direct assignment\n", node->type_id);
                        // 默认：直接赋值
                        IRCode *assign_code = new_ir_code(IR_ASSIGN, expr_temp, NULL, result);
                        if (assign_code) {
                            append_ir_code(ir_list, assign_code);
                        }
                    }
                }
            }
            break;
            
        case POST_INC_EXP:  // 处理后置++
        case POST_DEC_EXP:  // 处理后置--
            printf("[IR DEBUG] Processing POST_INC/DEC_EXP: %s\n", node->type_id);
            if (node->ptr[0]) {
                // 获取操作数
                ASTNode *operand = node->ptr[0];
                
                // 为操作数生成临时变量
                Type *operand_type = operand->type_info;
                if (!operand_type) operand_type = new_basic_type(TYPE_INT);
                
                Operand *operand_temp = new_temp_operand(operand_type);
                if (operand_temp) {
                    // 获取操作数的值
                    gen_ir_expr(operand, ir_list, operand_temp, symtab);
                    
                    // 将操作数的值赋给结果（后置：先返回原值）
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, operand_temp, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                    
                    // 生成增量/减量操作
                    Operand *one = new_const_operand_int(1);
                    Operand *new_value = new_temp_operand(operand_type);
                    if (new_value) {
                        IROpCode op_code = (node->kind == POST_INC_EXP) ? IR_ADD : IR_SUB;
                        IRCode *inc_dec_code = new_ir_code(op_code, operand_temp, one, new_value);
                        if (inc_dec_code) {
                            append_ir_code(ir_list, inc_dec_code);
                        }
                        
                        // 如果是变量，需要更新原变量
                        if (operand->symbol_ref) {
                            Operand *var_op = new_var_operand(operand->symbol_ref);
                            if (var_op) {
                                IRCode *update_code = new_ir_code(IR_ASSIGN, new_value, NULL, var_op);
                                if (update_code) {
                                    append_ir_code(ir_list, update_code);
                                }
                            }
                        }
                    }
                }
            }
            break;
            
        case ARRAY_ACCESS:
            printf("[IR DEBUG] Processing ARRAY_ACCESS\n");
            gen_ir_array_access(node, ir_list, result, symtab);
            break;
            
        default:
            printf("[IR DEBUG] Unhandled expression kind=%d, using default\n", node->kind);
            // 默认处理：生成一个0常量
            Operand *const_op = new_const_operand_int(0);
            if (const_op) {
                IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                if (assign_code) {
                    append_ir_code(ir_list, assign_code);
                }
            }
            break;
    }
}

/* 生成二元表达式的中间代码 */
static void gen_ir_binary_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list || !result) return;
    
    printf("[DEBUG] Generating binary expression, op: %s\n", node->type_id);
    
    ASTNode *left = node->ptr[0];
    ASTNode *right = node->ptr[1];
    
    if (!left || !right) return;
    
    // 为左右操作数生成临时变量
    Type *left_type = left->type_info;
    Type *right_type = right->type_info;
    
    /* 确保类型不是函数类型 */
    if (!left_type) {
        left_type = new_basic_type(TYPE_INT);
        printf("[DEBUG] Using int as left type (was NULL)\n");
    }
    
    if (!right_type) {
        right_type = new_basic_type(TYPE_INT);
        printf("[DEBUG] Using int as right type (was NULL)\n");
    }
    
    Operand *left_temp = new_temp_operand(left_type);
    Operand *right_temp = new_temp_operand(right_type);
    
    if (!left_temp || !right_temp) {
        printf("[ERROR] Failed to create temp operands for binary expression\n");
        return;
    }
    
    // 生成左右操作数的代码
    gen_ir_expr(left, ir_list, left_temp, symtab);
    gen_ir_expr(right, ir_list, right_temp, symtab);
    
    // 根据操作符生成相应的中间代码
    IROpCode op_code;
    char *op_str = node->type_id;
    
    if (op_str) {
        if (strcmp(op_str, "+") == 0) op_code = IR_ADD;
        else if (strcmp(op_str, "-") == 0) op_code = IR_SUB;
        else if (strcmp(op_str, "*") == 0) op_code = IR_MUL;
        else if (strcmp(op_str, "/") == 0) op_code = IR_DIV;
        else if (strcmp(op_str, "%") == 0) {
            // 模运算：a % b = a - (a / b) * b
            printf("[DEBUG] Generating modulo operation\n");
            
            // 创建临时变量用于计算
            Operand *div_result = new_temp_operand(left_temp->type);
            Operand *mul_result = new_temp_operand(left_temp->type);
            
            if (div_result && mul_result) {
                // div_result = left / right
                IRCode *div_code = new_ir_code(IR_DIV, left_temp, right_temp, div_result);
                if (div_code) {
                    printf("[DEBUG] Created division for modulo\n");
                    append_ir_code(ir_list, div_code);
                }
                
                // mul_result = div_result * right
                IRCode *mul_code = new_ir_code(IR_MUL, div_result, right_temp, mul_result);
                if (mul_code) {
                    printf("[DEBUG] Created multiplication for modulo\n");
                    append_ir_code(ir_list, mul_code);
                }
                
                // result = left - mul_result
                IRCode *sub_code = new_ir_code(IR_SUB, left_temp, mul_result, result);
                if (sub_code) {
                    printf("[DEBUG] Created subtraction for modulo result\n");
                    append_ir_code(ir_list, sub_code);
                }
            } else {
                printf("[ERROR] Failed to create temp operands for modulo operation\n");
                // 如果创建临时变量失败，使用默认的除法
                op_code = IR_DIV;
                IRCode *binary_code = new_ir_code(op_code, left_temp, right_temp, result);
                if (binary_code) {
                    append_ir_code(ir_list, binary_code);
                }
            }
            return;  // 模运算已经处理完毕，直接返回
        }
        else if (strcmp(op_str, "<") == 0) op_code = IR_LT;
        else if (strcmp(op_str, "<=") == 0) op_code = IR_LE;
        else if (strcmp(op_str, ">") == 0) op_code = IR_GT;
        else if (strcmp(op_str, ">=") == 0) op_code = IR_GE;
        else if (strcmp(op_str, "==") == 0) op_code = IR_EQ;
        else if (strcmp(op_str, "!=") == 0) op_code = IR_NE;
        else if (strcmp(op_str, "&&") == 0 || strcmp(op_str, "||") == 0) {
            // 逻辑运算符：使用短路求值
            printf("[DEBUG] Generating logical operator: %s\n", op_str);
            
            // 创建标签
            Operand *true_label = new_label_operand();
            Operand *false_label = new_label_operand();
            Operand *end_label = new_label_operand();
            
            if (!true_label || !false_label || !end_label) {
                printf("[ERROR] Failed to create labels for logical operator\n");
                return;
            }
            
            if (strcmp(op_str, "&&") == 0) {
                // 逻辑与：先检查左操作数，如果为假直接跳到false
                IRCode *if_false = new_ir_code(IR_IF, left_temp, NULL, false_label);
                if (if_false) append_ir_code(ir_list, if_false);
                
                // 左操作数为真，检查右操作数
                IRCode *if_true = new_ir_code(IR_IF, right_temp, NULL, true_label);
                if (if_true) append_ir_code(ir_list, if_true);
                
                // 右操作数为假，跳转到false
                IRCode *goto_false = new_ir_code(IR_GOTO, NULL, NULL, false_label);
                if (goto_false) append_ir_code(ir_list, goto_false);
            } else {
                // 逻辑或：先检查左操作数，如果为真直接跳到true
                IRCode *if_true = new_ir_code(IR_IF, left_temp, NULL, true_label);
                if (if_true) append_ir_code(ir_list, if_true);
                
                // 左操作数为假，检查右操作数
                IRCode *if_false = new_ir_code(IR_IF, right_temp, NULL, true_label);
                if (if_false) append_ir_code(ir_list, if_false);
                
                // 右操作数为假，跳转到false
                IRCode *goto_false = new_ir_code(IR_GOTO, NULL, NULL, false_label);
                if (goto_false) append_ir_code(ir_list, goto_false);
            }
            
            // true标签：结果为1
            IRCode *label_true = new_ir_code(IR_LABEL, NULL, NULL, true_label);
            if (label_true) append_ir_code(ir_list, label_true);
            
            Operand *one = new_const_operand_int(1);
            IRCode *assign_one = new_ir_code(IR_ASSIGN, one, NULL, result);
            if (assign_one) append_ir_code(ir_list, assign_one);
            
            IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label);
            if (goto_end) append_ir_code(ir_list, goto_end);
            
            // false标签：结果为0
            IRCode *label_false = new_ir_code(IR_LABEL, NULL, NULL, false_label);
            if (label_false) append_ir_code(ir_list, label_false);
            
            Operand *zero = new_const_operand_int(0);
            IRCode *assign_zero = new_ir_code(IR_ASSIGN, zero, NULL, result);
            if (assign_zero) append_ir_code(ir_list, assign_zero);
            
            // end标签
            IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label);
            if (label_end) append_ir_code(ir_list, label_end);
            
            return;  // 逻辑运算已经处理完毕，直接返回
        }
        else {
            printf("[WARNING] Unknown binary operator: %s, using ADD\n", op_str);
            op_code = IR_ADD;
        }
    } else {
        printf("[WARNING] Binary operator is NULL, using ADD\n");
        op_code = IR_ADD;
    }
    
    // 对于非特殊运算，正常生成代码
    if (op_str && strcmp(op_str, "%") != 0 && 
        strcmp(op_str, "&&") != 0 && strcmp(op_str, "||") != 0) {
        IRCode *binary_code = new_ir_code(op_code, left_temp, right_temp, result);
        if (binary_code) {
            append_ir_code(ir_list, binary_code);
            printf("[DEBUG] Created binary IR code: %s\n", op_str);
        } else {
            printf("[ERROR] Failed to create binary IR code\n");
        }
    }
}

/* 生成赋值表达式的中间代码 */
static void gen_ir_assign_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list || !result) return;
    
    printf("[IR DEBUG] Processing ASSIGN_EXP: %s\n", node->type_id);
    
    ASTNode *left = node->ptr[0];
    ASTNode *right = node->ptr[1];
    
    if (!left || !right) return;
    
    // 处理右操作数值
    Type *right_type = right->type_info;
    if (!right_type) right_type = new_basic_type(TYPE_INT);
    
    Operand *right_temp = new_temp_operand(right_type);
    if (!right_temp) {
        printf("[ERROR] Failed to create right temp operand\n");
        return;
    }
    
    gen_ir_expr(right, ir_list, right_temp, symtab);
    
    // 将结果赋给result（作为表达式的值）
    IRCode *assign_result = new_ir_code(IR_ASSIGN, right_temp, NULL, result);
    if (assign_result) {
        append_ir_code(ir_list, assign_result);
    }
    
    // 检查左操作数类型并处理赋值
    if (left->kind == ARRAY_ACCESS) {
        // 数组元素赋值
        printf("[IR DEBUG] Array element assignment\n");
        
        // 生成多维数组存储的关键代码
        // 计算左操作数的地址
        Type *target_type = NULL;
        if (left->type_info) {
            target_type = left->type_info;
        } else if (left->ptr[0] && left->ptr[0]->type_info) {
            // 如果数组访问本身没有类型，使用数组基址的类型
            target_type = left->ptr[0]->type_info;
        } else {
            target_type = new_basic_type(TYPE_INT);
        }
        
        Operand *array_addr = new_temp_operand(target_type);
        if (!array_addr) {
            printf("[ERROR] Failed to create array address temp operand\n");
            return;
        }
        
        // 获取数组基址和索引
        gen_ir_array_access(left, ir_list, array_addr, symtab);
        
        // 现在我们需要将right_temp存储到array_addr指向的位置
        // 由于我们只是计算了地址，需要生成实际的存储指令
        // 简化方案：创建一个ARRAY_STORE指令
        if (left->ptr[0] && left->ptr[0]->kind == ID_NODE) {
            // 如果是基本数组：arr[i] = value
            SymbolEntry *array_sym = left->ptr[0]->symbol_ref;
            if (!array_sym && symtab) {
                array_sym = lookup_symbol(symtab, left->ptr[0]->type_id);
            }
            
            if (array_sym) {
                // 创建数组操作数
                Operand *array_op = (Operand *)calloc(1, sizeof(Operand));
                if (!array_op) {
                    printf("[ERROR] Failed to allocate array operand\n");
                    return;
                }
                
                array_op->kind = OP_VAR;
                array_op->name = strdup(left->ptr[0]->type_id);
                if (array_sym->type) {
                    array_op->type = copy_type(array_sym->type);
                } else {
                    array_op->type = new_basic_type(TYPE_INT);
                }
                array_op->offset = array_sym->offset;
                
                // 处理索引
                Operand *index_temp = new_temp_operand(new_basic_type(TYPE_INT));
                if (!index_temp) {
                    printf("[ERROR] Failed to create index operand\n");
                    free(array_op->name);
                    free(array_op);
                    return;
                }
                
                gen_ir_expr(left->ptr[1], ir_list, index_temp, symtab);
                
                // 生成数组存储指令
                IRCode *store_code = new_ir_code(IR_ARRAY_STORE, array_op, index_temp, right_temp);
                if (store_code) {
                    printf("[IR DEBUG] Generated ARRAY_STORE for %s[%s] = %s\n", 
                           left->ptr[0]->type_id, index_temp->name, right_temp->name);
                    append_ir_code(ir_list, store_code);
                }
            } else {
                printf("[ERROR] Array symbol not found for %s\n", left->ptr[0]->type_id);
            }
        } else {
            // 多维数组或复杂表达式，使用简化方案
            printf("[IR DEBUG] Multi-dimensional array store - using simplified approach\n");
            // 这里需要更复杂的地址计算，暂时使用简单赋值
            IRCode *assign_code = new_ir_code(IR_ASSIGN, right_temp, NULL, array_addr);
            if (assign_code) {
                append_ir_code(ir_list, assign_code);
            }
        }
        
    } else {
        // 非数组赋值的原有代码保持不变
        // 检查是否是复合赋值运算符（+=, -=, *=, /=, %=）
        char *op = node->type_id;
        int is_compound = 0;
        IROpCode compound_op = IR_ADD;
        
        if (op) {
            if (strcmp(op, "+=") == 0) {
                is_compound = 1;
                compound_op = IR_ADD;
            } else if (strcmp(op, "-=") == 0) {
                is_compound = 1;
                compound_op = IR_SUB;
            } else if (strcmp(op, "*=") == 0) {
                is_compound = 1;
                compound_op = IR_MUL;
            } else if (strcmp(op, "/=") == 0) {
                is_compound = 1;
                compound_op = IR_DIV;
            } else if (strcmp(op, "%=") == 0) {
                is_compound = 1;
                // 模运算需要特殊处理
            }
        }
        
        if (is_compound) {
            printf("[IR DEBUG] Processing compound assignment: %s\n", op);
            
            // 对于复合赋值：a += b 等价于 a = a + b
            
            // 首先获取左操作数的值
            Type *left_type = left->type_info;
            if (!left_type) left_type = new_basic_type(TYPE_INT);
            
            Operand *left_temp = new_temp_operand(left_type);
            if (!left_temp) return;
            
            // 获取左操作数的当前值
            if (left->symbol_ref) {
                Operand *left_var = new_var_operand(left->symbol_ref);
                if (left_var) {
                    IRCode *load_left = new_ir_code(IR_ASSIGN, left_var, NULL, left_temp);
                    if (load_left) append_ir_code(ir_list, load_left);
                }
            } else {
                gen_ir_expr(left, ir_list, left_temp, symtab);
            }
            
            // 生成右操作数的值
            Type *right_type = right->type_info;
            if (!right_type) right_type = new_basic_type(TYPE_INT);
            
            Operand *right_temp = new_temp_operand(right_type);
            if (!right_temp) return;
            
            gen_ir_expr(right, ir_list, right_temp, symtab);
            
            // 执行运算
            if (strcmp(op, "%=") == 0) {
                // 模赋值：a %= b 等价于 a = a % b
                // 需要特殊处理模运算
                Operand *div_result = new_temp_operand(left_temp->type);
                Operand *mul_result = new_temp_operand(left_temp->type);
                
                if (div_result && mul_result) {
                    // div_result = left / right
                    IRCode *div_code = new_ir_code(IR_DIV, left_temp, right_temp, div_result);
                    if (div_code) append_ir_code(ir_list, div_code);
                    
                    // mul_result = div_result * right
                    IRCode *mul_code = new_ir_code(IR_MUL, div_result, right_temp, mul_result);
                    if (mul_code) append_ir_code(ir_list, mul_code);
                    
                    // result = left - mul_result
                    IRCode *sub_code = new_ir_code(IR_SUB, left_temp, mul_result, result);
                    if (sub_code) append_ir_code(ir_list, sub_code);
                }
            } else {
                // 其他复合赋值：a += b 等
                IRCode *binary_code = new_ir_code(compound_op, left_temp, right_temp, result);
                if (binary_code) append_ir_code(ir_list, binary_code);
            }
            
            // 将结果赋给左操作数
            if (left->symbol_ref) {
                Operand *left_op = new_var_operand(left->symbol_ref);
                if (left_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, result, NULL, left_op);
                    if (assign_code) append_ir_code(ir_list, assign_code);
                }
            }
            
        } else {
            // 普通赋值：a = b
            printf("[IR DEBUG] Processing simple assignment: =\n");
            
            // 生成右操作数的代码，直接赋值给result
            Type *right_type = right->type_info;
            if (!right_type) right_type = new_basic_type(TYPE_INT);
            
            // 直接使用传入的result作为右操作数的目标
            gen_ir_expr(right, ir_list, result, symtab);
            
            // 如果是变量赋值，还需要将结果赋给左操作数
            if (left->symbol_ref) {
                Operand *left_op = new_var_operand(left->symbol_ref);
                if (left_op) {
                    // 将result的值赋给左操作数
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, result, NULL, left_op);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
        }
    }
}

/* 生成函数调用的中间代码 */
static void gen_ir_func_call(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    ASTNode *func_name_node = node->ptr[0];
    ASTNode *args_node = node->ptr[1];
    
    if (!func_name_node || func_name_node->kind != ID_NODE) {
        printf("[ERROR] Invalid function call node\n");
        return;
    }
    
    char *func_name = func_name_node->type_id;
    if (!func_name) {
        printf("[ERROR] Function name is NULL\n");
        return;
    }
    
    printf("[IR DEBUG] Generating function call to %s\n", func_name);
    
    // 生成实参的代码
    if (args_node) {
        printf("[IR DEBUG] Generating arguments for function call\n");
        gen_ir_args(args_node, ir_list, symtab);
    } else {
        printf("[IR DEBUG] No arguments for function call\n");
    }
    
    // 生成CALL指令
    Operand *func_op = (Operand *)malloc(sizeof(Operand));
    if (!func_op) {
        printf("[ERROR] Failed to create function operand\n");
        return;
    }
    
    func_op->kind = OP_FUNC;
    func_op->name = strdup(func_name);
    
    // 假设函数有返回值
    if (result) {
        printf("[IR DEBUG] Creating CALL with result %s\n", result->name);
        IRCode *call_code = new_ir_code(IR_CALL, func_op, NULL, result);
        if (call_code) {
            append_ir_code(ir_list, call_code);
        } else {
            printf("[ERROR] Failed to create CALL instruction\n");
        }
    } else {
        printf("[IR DEBUG] Creating CALL without result\n");
        IRCode *call_code = new_ir_code(IR_CALL_VOID, func_op, NULL, NULL);
        if (call_code) {
            append_ir_code(ir_list, call_code);
        } else {
            printf("[ERROR] Failed to create CALL_VOID instruction\n");
        }
    }
}

/* 生成实参的中间代码 - 简化版 */
static void gen_ir_args(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    printf("[IR DEBUG] Generating args, node kind=%d\n", node->kind);
    
    if (node->kind == ARGS) {
        if (node->ptr[0]) {
            printf("[IR DEBUG] Processing argument at ptr[0], kind=%d\n", node->ptr[0]->kind);
            
            // 为实参表达式生成临时变量
            Type *arg_type = NULL;
            
            // 首先使用节点已有的类型信息
            if (node->ptr[0]->type_info) {
                arg_type = node->ptr[0]->type_info;
                printf("[IR DEBUG] Using existing type_info: kind=%d\n", arg_type->kind);
            } 
            // 如果节点有symbol_ref，使用它的类型
            else if (node->ptr[0]->symbol_ref && node->ptr[0]->symbol_ref->type) {
                arg_type = node->ptr[0]->symbol_ref->type;
                printf("[IR DEBUG] Using type from symbol_ref: kind=%d\n", arg_type->kind);
            }
            // 最后才创建默认类型
            else {
                printf("[IR DEBUG] No type info found, using int\n");
                arg_type = new_basic_type(TYPE_INT);
            }
            
            Operand *arg_temp = new_temp_operand(arg_type);
            if (arg_temp) {
                printf("[IR DEBUG] Generated temp operand for argument: %s\n", arg_temp->name);
                
                // 生成参数表达式的IR代码
                gen_ir_expr(node->ptr[0], ir_list, arg_temp, symtab);
                
                // 生成ARG指令
                IRCode *arg_code = new_ir_code(IR_ARG, arg_temp, NULL, NULL);
                if (arg_code) {
                    printf("[IR DEBUG] Created ARG instruction for %s\n", arg_temp->name);
                    append_ir_code(ir_list, arg_code);
                }
            } else {
                printf("[ERROR] Failed to create temp operand for argument\n");
            }
        }
        
        // 递归处理剩余实参
        if (node->ptr[1]) {
            printf("[IR DEBUG] Processing next argument\n");
            gen_ir_args(node->ptr[1], ir_list, symtab);
        } else {
            printf("[IR DEBUG] No more arguments\n");
        }
    } else {
        printf("[ERROR] Expected ARGS node, got kind=%d\n", node->kind);
    }
}

/* 生成if语句的中间代码 */
static void gen_ir_if_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || node->kind != IF_STMT || !ir_list) return;
    
    ASTNode *cond = node->ptr[0];
    ASTNode *then_stmt = node->ptr[1];
    
    if (!cond || !then_stmt) return;
    
    // 为条件表达式生成临时变量
    Type *cond_type = cond->type_info;
    if (!cond_type) cond_type = new_basic_type(TYPE_INT);
    
    Operand *cond_temp = new_temp_operand(cond_type);
    if (!cond_temp) return;
    
    gen_ir_expr(cond, ir_list, cond_temp, symtab);
    
    // 生成标签
    Operand *then_label = new_label_operand();
    Operand *end_label = new_label_operand();
    
    if (!then_label || !end_label) return;
    
    // 生成条件跳转
    IRCode *if_code = new_ir_code(IR_IF, cond_temp, NULL, then_label);
    if (if_code) {
        append_ir_code(ir_list, if_code);
    }
    
    // 生成跳转到结束的指令
    IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label);
    if (goto_end) {
        append_ir_code(ir_list, goto_end);
    }
    
    // 生成then标签
    IRCode *label_then = new_ir_code(IR_LABEL, NULL, NULL, then_label);
    if (label_then) {
        append_ir_code(ir_list, label_then);
    }
    
    // 生成then语句
    gen_ir_stmt(then_stmt, ir_list, NULL, symtab);
    
    // 生成结束标签
    IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label);
    if (label_end) {
        append_ir_code(ir_list, label_end);
    }
}

/* 生成if-else语句的中间代码 */
static void gen_ir_if_else_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || node->kind != IF_ELSE_STMT || !ir_list) return;
    
    ASTNode *cond = node->ptr[0];
    ASTNode *then_stmt = node->ptr[1];
    ASTNode *else_stmt = node->ptr[2];
    
    if (!cond || !then_stmt || !else_stmt) return;
    
    // 为条件表达式生成临时变量
    Type *cond_type = cond->type_info;
    if (!cond_type) cond_type = new_basic_type(TYPE_INT);
    
    Operand *cond_temp = new_temp_operand(cond_type);
    if (!cond_temp) return;
    
    gen_ir_expr(cond, ir_list, cond_temp, symtab);
    
    // 生成标签
    Operand *then_label = new_label_operand();
    Operand *else_label = new_label_operand();
    Operand *end_label = new_label_operand();
    
    if (!then_label || !else_label || !end_label) return;
    
    // 生成条件跳转到then分支
    IRCode *if_code = new_ir_code(IR_IF, cond_temp, NULL, then_label);
    if (if_code) {
        append_ir_code(ir_list, if_code);
    }
    
    // 生成跳转到else分支的指令
    IRCode *goto_else = new_ir_code(IR_GOTO, NULL, NULL, else_label);
    if (goto_else) {
        append_ir_code(ir_list, goto_else);
    }
    
    // 生成then标签
    IRCode *label_then = new_ir_code(IR_LABEL, NULL, NULL, then_label);
    if (label_then) {
        append_ir_code(ir_list, label_then);
    }
    
    // 生成then语句
    gen_ir_stmt(then_stmt, ir_list, NULL, symtab);
    
    // 生成跳转到结束的指令
    IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label);
    if (goto_end) {
        append_ir_code(ir_list, goto_end);
    }
    
    // 生成else标签
    IRCode *label_else = new_ir_code(IR_LABEL, NULL, NULL, else_label);
    if (label_else) {
        append_ir_code(ir_list, label_else);
    }
    
    // 生成else语句
    gen_ir_stmt(else_stmt, ir_list, NULL, symtab);
    
    // 生成结束标签
    IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label);
    if (label_end) {
        append_ir_code(ir_list, label_end);
    }
}

/* 生成while语句的中间代码 */
static void gen_ir_while_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || node->kind != WHILE_STMT || !ir_list) return;
    
    ASTNode *cond = node->ptr[0];
    ASTNode *body = node->ptr[1];
    
    if (!cond || !body) return;
    
    // 生成标签
    Operand *test_label_op = new_label_operand();
    Operand *body_label_op = new_label_operand();
    Operand *end_label_op = new_label_operand();
    
    if (!test_label_op || !body_label_op || !end_label_op) return;
    
    char *test_label = test_label_op->name;
    char *body_label = body_label_op->name;
    char *end_label = end_label_op->name;
    
    printf("[IR DEBUG] WHILE_STMT: test_label=%s, body_label=%s, end_label=%s\n", 
           test_label, body_label, end_label);
    
    // ========== 进入循环上下文 ==========
    enter_loop(end_label, test_label);
    
    // 测试标签
    IRCode *label_test = new_ir_code(IR_LABEL, NULL, NULL, test_label_op);
    if (label_test) {
        append_ir_code(ir_list, label_test);
    }
    
    // 为条件表达式生成临时变量
    Type *cond_type = cond->type_info;
    if (!cond_type) cond_type = new_basic_type(TYPE_INT);
    
    Operand *cond_temp = new_temp_operand(cond_type);
    if (!cond_temp) {
        printf("[ERROR] Failed to create temp operand for while condition\n");
        exit_loop();
        return;
    }
    
    // 生成条件表达式代码
    gen_ir_expr(cond, ir_list, cond_temp, symtab);
    
    // 条件跳转到循环体
    IRCode *if_code = new_ir_code(IR_IF, cond_temp, NULL, body_label_op);
    if (if_code) {
        append_ir_code(ir_list, if_code);
    }
    
    // 跳转到结束
    IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label_op);
    if (goto_end) {
        append_ir_code(ir_list, goto_end);
    }
    
    // 循环体标签
    IRCode *label_body = new_ir_code(IR_LABEL, NULL, NULL, body_label_op);
    if (label_body) {
        append_ir_code(ir_list, label_body);
    }
    
    // 生成循环体
    gen_ir_stmt(body, ir_list, NULL, symtab);
    
    // 跳回测试
    IRCode *goto_test = new_ir_code(IR_GOTO, NULL, NULL, test_label_op);
    if (goto_test) {
        append_ir_code(ir_list, goto_test);
    }
    
    // 结束标签
    IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label_op);
    if (label_end) {
        append_ir_code(ir_list, label_end);
    }
    
    // ========== 退出循环上下文 ==========
    exit_loop();
}

/* 生成for语句的中间代码 */
static void gen_ir_for_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || node->kind != FOR_STMT || !ir_list) return;
    
    ASTNode *init = node->ptr[0];    // 初始化
    ASTNode *cond = node->ptr[1];    // 条件
    ASTNode *update = node->ptr[2];  // 更新
    ASTNode *body = node->ptr[3];    // 循环体
    
    if (!body) return;
    
    // 生成标签
    Operand *test_label_op = new_label_operand();
    Operand *body_label_op = new_label_operand();
    Operand *update_label_op = new_label_operand();
    Operand *end_label_op = new_label_operand();
    
    if (!test_label_op || !body_label_op || !update_label_op || !end_label_op) {
        printf("[ERROR] Failed to create labels for for statement\n");
        return;
    }
    
    char *test_label = test_label_op->name;
    char *body_label = body_label_op->name;
    char *update_label = update_label_op->name;
    char *end_label = end_label_op->name;
    
    printf("[IR DEBUG] FOR_STMT: test_label=%s, body_label=%s, update_label=%s, end_label=%s\n", 
           test_label, body_label, update_label, end_label);
    
    // ========== 进入循环上下文 ==========
    // for循环中，continue应该跳转到update标签
    enter_loop(end_label, update_label);
    
    // 生成初始化代码
    if (init) {
        printf("[IR DEBUG] Generating for loop initialization\n");
        gen_ir_stmt(init, ir_list, NULL, symtab);
    } else {
        printf("[IR DEBUG] No initialization in for loop\n");
    }
    
    // 测试标签
    IRCode *label_test = new_ir_code(IR_LABEL, NULL, NULL, test_label_op);
    if (label_test) {
        append_ir_code(ir_list, label_test);
    }
    
    // 如果有条件，生成条件检查
    if (cond) {
        printf("[IR DEBUG] Generating for loop condition\n");
        Type *cond_type = cond->type_info;
        if (!cond_type) cond_type = new_basic_type(TYPE_INT);
        
        Operand *cond_temp = new_temp_operand(cond_type);
        if (cond_temp) {
            gen_ir_expr(cond, ir_list, cond_temp, symtab);
            
            // 条件跳转到循环体
            IRCode *if_code = new_ir_code(IR_IF, cond_temp, NULL, body_label_op);
            if (if_code) {
                append_ir_code(ir_list, if_code);
            }
        }
    } else {
        // 没有条件，直接跳转到循环体
        printf("[IR DEBUG] No condition in for loop, jumping directly to body\n");
        IRCode *goto_body = new_ir_code(IR_GOTO, NULL, NULL, body_label_op);
        if (goto_body) {
            append_ir_code(ir_list, goto_body);
        }
    }
    
    // 跳转到结束
    IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label_op);
    if (goto_end) {
        append_ir_code(ir_list, goto_end);
    }
    
    // 循环体标签
    IRCode *label_body = new_ir_code(IR_LABEL, NULL, NULL, body_label_op);
    if (label_body) {
        append_ir_code(ir_list, label_body);
    }
    
    // 生成循环体
    gen_ir_stmt(body, ir_list, NULL, symtab);
    
    // 更新标签
    IRCode *label_update = new_ir_code(IR_LABEL, NULL, NULL, update_label_op);
    if (label_update) {
        append_ir_code(ir_list, label_update);
    }
    
    // 生成更新代码
    if (update) {
        printf("[IR DEBUG] Generating for loop update\n");
        Type *update_type = update->type_info;
        if (!update_type) update_type = new_basic_type(TYPE_INT);
        
        Operand *update_temp = new_temp_operand(update_type);
        if (update_temp) {
            // 生成更新表达式，但不需要赋值给任何变量
            // 更新表达式通常有副作用（如i++），所以需要执行它
            gen_ir_expr(update, ir_list, update_temp, symtab);
        }
    } else {
        printf("[IR DEBUG] No update in for loop\n");
    }
    
    // 跳回测试
    IRCode *goto_test = new_ir_code(IR_GOTO, NULL, NULL, test_label_op);
    if (goto_test) {
        append_ir_code(ir_list, goto_test);
    }
    
    // 结束标签
    IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label_op);
    if (label_end) {
        append_ir_code(ir_list, label_end);
    }
    
    // ========== 退出循环上下文 ==========
    exit_loop();
}

/* 生成定义声明列表的中间代码 */
static void gen_ir_def_dec_list(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    printf("[IR DEBUG] Processing DEF_DEC_LIST kind=%d\n", node->kind);
    
    switch (node->kind) {
        case ARRAY_DEC:  // 数组声明
            printf("[IR DEBUG] Processing ARRAY_DEC\n");
            // 数组声明在IR中通常不需要生成代码
            // 内存分配在目标代码生成阶段处理
            break;
            
        case ID_NODE:  // 变量声明
            printf("[IR DEBUG] Processing variable declaration: %s\n", node->type_id);
            // 简单变量声明，不需要生成初始化代码
            break;
            
        case INIT_DEC:  // 初始化声明
            printf("[IR DEBUG] Processing INIT_DEC in DEF\n");
            gen_ir_init_dec(node, ir_list, symtab);
            break;
            
        case DEC_LIST:  // 声明列表
            printf("[IR DEBUG] Processing DEC_LIST\n");
            if (node->ptr[0]) {
                gen_ir_def_dec_list(node->ptr[0], ir_list, symtab);
            }
            if (node->ptr[1]) {
                gen_ir_def_dec_list(node->ptr[1], ir_list, symtab);
            }
            break;
            
        default:
            printf("[IR DEBUG] Unhandled declaration kind=%d in DEF\n", node->kind);
            break;
    }
}

/* 处理声明列表 */
static void gen_ir_dec_list(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    printf("[IR DEBUG] Processing DEC_LIST, kind=%d\n", node->kind);
    
    if (node->kind == DEC_LIST) {
        // 处理当前声明
        ASTNode *current_dec = node->ptr[0];
        if (current_dec) {
            gen_ir_dec(current_dec, ir_list, symtab);
        }
        
        // 递归处理剩余的声明
        ASTNode *next_dec = node->ptr[1];
        if (next_dec) {
            gen_ir_dec_list(next_dec, ir_list, symtab);
        }
    } else if (node->kind == INIT_DEC) {
        // 处理初始化声明
        gen_ir_init_dec(node, ir_list, symtab);
    } else {
        // 也可能是单个声明
        gen_ir_dec(node, ir_list, symtab);
    }
}

/* 处理单个声明 */
static void gen_ir_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || !ir_list) return;
    
    printf("[IR DEBUG] Processing declaration, kind=%d\n", node->kind);
    
    switch (node->kind) {
        case ARRAY_DEC:
            printf("[IR DEBUG] Processing ARRAY_DEC\n");
            gen_ir_array_dec(node, ir_list, symtab);
            break;
            
        case ID_NODE:
            printf("[IR DEBUG] Processing variable declaration: %s\n", node->type_id);
            // 普通变量声明，不需要生成代码
            break;
            
        case INIT_DEC:
            printf("[IR DEBUG] Processing INIT_DEC in dec\n");
            gen_ir_init_dec(node, ir_list, symtab);
            break;
            
        default:
            printf("[IR DEBUG] Unhandled declaration kind: %d\n", node->kind);
            break;
    }
}

/* 处理数组声明 - 修复多维数组处理 */
static void gen_ir_array_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || node->kind != ARRAY_DEC || !ir_list) return;
    
    ASTNode *array_name = node->ptr[0];
    ASTNode *array_size = node->ptr[1];
    
    if (!array_name) return;
    
    printf("[IR DEBUG] Processing array declaration (ARRAY_DEC)\n");
    
    // 处理多维数组：递归处理内层数组声明
    if (array_name->kind == ARRAY_DEC) {
        printf("[IR DEBUG] Recursing for multi-dimensional array\n");
        gen_ir_array_dec(array_name, ir_list, symtab);
        // 继续处理当前维度的声明
    }
    
    // 如果是数组名节点，生成分配指令
    if (array_name->kind == ID_NODE) {
        printf("[IR DEBUG] Found array name: %s\n", array_name->type_id);
        
        // 直接使用节点的symbol_ref
        SymbolEntry *array_sym = array_name->symbol_ref;
        if (!array_sym) {
            printf("[ERROR] No symbol_ref for array %s\n", array_name->type_id);
            return;
        }
        
        // 创建数组操作数
        Operand *array_op = (Operand *)calloc(1, sizeof(Operand));
        if (!array_op) {
            printf("[ERROR] Failed to allocate array operand\n");
            return;
        }
        
        array_op->kind = OP_VAR;
        array_op->name = strdup(array_name->type_id ? array_name->type_id : "unnamed_array");
        if (array_sym->type) {
            array_op->type = copy_type(array_sym->type);
        } else {
            array_op->type = new_basic_type(TYPE_INT);
        }
        array_op->offset = array_sym->offset;
        
        // 获取数组大小
        int size = 1;
        if (array_size && array_size->kind == INT_NODE) {
            size = array_size->type_int;
        }
        
        Operand *size_op = new_const_operand_int(size);
        if (!size_op) {
            printf("[ERROR] Failed to create size operand\n");
            free(array_op->name);
            free(array_op);
            return;
        }
        
        // 生成分配指令
        IRCode *alloc_code = new_ir_code(IR_ARRAY_ALLOC, size_op, NULL, array_op);
        if (alloc_code) {
            printf("[IR DEBUG] Generated ARRAY_ALLOC for %s[%d]\n", array_name->type_id, size);
            append_ir_code(ir_list, alloc_code);
        }
    }
}

/* 处理数组访问 - 修复多层数组访问 */
static void gen_ir_array_access(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || node->kind != ARRAY_ACCESS || !ir_list || !result) return;
    
    ASTNode *array_node = node->ptr[0];  // 数组名或数组访问表达式
    ASTNode *index_node = node->ptr[1];  // 索引
    
    if (!array_node || !index_node) {
        printf("[ERROR] Invalid array access node\n");
        return;
    }
    
    printf("[IR DEBUG] Array access: array kind=%d\n", array_node->kind);
    
    // 处理索引表达式
    Type *index_type = index_node->type_info;
    if (!index_type) index_type = new_basic_type(TYPE_INT);
    
    Operand *index_temp = new_temp_operand(index_type);
    if (!index_temp) {
        printf("[ERROR] Failed to create index operand\n");
        return;
    }
    
    gen_ir_expr(index_node, ir_list, index_temp, symtab);
    
    // 处理数组表达式
    Operand *base_addr = NULL;
    
    if (array_node->kind == ID_NODE) {
        // 基本数组名
        printf("[IR DEBUG] Loading from base array: %s\n", array_node->type_id);
        
        // 获取数组符号
        SymbolEntry *array_sym = array_node->symbol_ref;
        if (!array_sym) {
            if (symtab) {
                array_sym = lookup_symbol(symtab, array_node->type_id);
            }
            if (!array_sym) {
                printf("[ERROR] Array symbol not found for %s\n", array_node->type_id);
                // 使用默认值
                Operand *zero = new_const_operand_int(0);
                if (zero) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, zero, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
                return;
            }
        }
        
        // 创建基址操作数
        base_addr = (Operand *)calloc(1, sizeof(Operand));
        if (!base_addr) {
            printf("[ERROR] Failed to allocate base address operand\n");
            return;
        }
        base_addr->kind = OP_VAR;
        base_addr->name = strdup(array_node->type_id);
        if (array_sym->type) {
            base_addr->type = copy_type(array_sym->type);
        } else {
            base_addr->type = new_basic_type(TYPE_INT);
        }
        base_addr->offset = array_sym->offset;
        
        // 生成数组加载指令
        IRCode *load_code = new_ir_code(IR_ARRAY_LOAD, base_addr, index_temp, result);
        if (load_code) {
            printf("[IR DEBUG] Generated ARRAY_LOAD for %s[%s]\n", 
                   array_node->type_id, index_temp->name);
            append_ir_code(ir_list, load_code);
        }
        
    } else if (array_node->kind == ARRAY_ACCESS) {
        // 多层数组访问
        printf("[IR DEBUG] Multi-level array access, getting intermediate address\n");
        
        // 递归处理内层数组访问，得到一个中间地址
        Type *element_type = NULL;
        if (array_node->type_info && array_node->type_info->kind == TK_ARRAY) {
            element_type = copy_type(array_node->type_info->array.elem);
        } else {
            element_type = new_basic_type(TYPE_INT);
        }
        
        Operand *temp_addr = new_temp_operand(element_type);
        if (!temp_addr) {
            printf("[ERROR] Failed to create temp address operand\n");
            free_type(element_type);
            return;
        }
        
        // 递归处理内层数组访问
        gen_ir_array_access(array_node, ir_list, temp_addr, symtab);
        
        // 对于多维数组，我们需要加载内层数组的地址
        // 但如果是赋值上下文，我们可能需要计算最终地址
        // 这里简化处理：直接使用中间结果
        if (result && result->name && strncmp(result->name, "t", 1) == 0) {
            // 如果是临时变量，执行加载操作
            IRCode *load_code = new_ir_code(IR_ARRAY_LOAD, temp_addr, index_temp, result);
            if (load_code) {
                printf("[IR DEBUG] Generated multi-level ARRAY_LOAD\n");
                append_ir_code(ir_list, load_code);
            }
        } else {
            // 如果是地址计算，直接传递
            IRCode *assign_code = new_ir_code(IR_ASSIGN, temp_addr, NULL, result);
            if (assign_code) {
                append_ir_code(ir_list, assign_code);
            }
        }
        
    } else {
        printf("[ERROR] Unsupported array expression kind: %d\n", array_node->kind);
        return;
    }
}

/* 生成复合语句的中间代码 */
static void gen_ir_compound_stmt(ASTNode *node, IRList *ir_list, SymbolTable *symtab) {
    if (!node || node->kind != COMP_ST || !ir_list) return;
    
    printf("[IR DEBUG] Processing COMP_ST statement\n");
    
    // 处理复合语句中的所有子节点
    for (int i = 0; i < 4; i++) {
        if (node->ptr[i]) {
            printf("[IR DEBUG] COMP_ST child %d: kind=%d\n", i, node->ptr[i]->kind);
            
            // 如果是声明列表，处理声明
            if (node->ptr[i]->kind == DEF_LIST) {
                printf("[IR DEBUG] Processing declaration list in COMP_ST\n");
                gen_ir_stmt(node->ptr[i], ir_list, NULL, symtab);
            }
            // 如果是语句列表，处理语句
            else if (node->ptr[i]->kind == STMT_LIST) {
                printf("[IR DEBUG] Processing statement list in COMP_ST\n");
                gen_ir_stmt(node->ptr[i], ir_list, NULL, symtab);
            }
            // 其他情况，直接处理
            else {
                gen_ir_stmt(node->ptr[i], ir_list, NULL, symtab);
            }
        }
    }
}