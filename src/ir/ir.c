#include "ir.h"
#include "../ast/ast.h"
#include "../symbol/symbol.h"
#include "../semantic/type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* 全局计数器 */
static int temp_counter = 1;
static int label_counter = 1;

/* ================== 函数声明 ================== */
static void gen_ir_func_def(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_params(ASTNode *node, IRList *ir_list);
static void gen_ir_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_binary_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_assign_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_func_call(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab);
static void gen_ir_args(ASTNode *node, IRList *ir_list, SymbolTable *symtab);
static void gen_ir_if_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_while_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab);
static void gen_ir_init_dec(ASTNode *node, IRList *ir_list, SymbolTable *symtab);  

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
    
    op->kind = OP_VAR;
    char name[32];
    snprintf(name, sizeof(name), "t%d", temp_counter++);
    op->name = strdup(name);
    
    /* ========== 加强类型检查和错误处理 ========== */
    
    // 1. 检查type是否为NULL
    if (!type) {
        printf("[DEBUG] Temp operand type is NULL, using int as default\n");
        op->type = new_basic_type(TYPE_INT);
        op->offset = 0;
        printf("[DEBUG] Created temp operand %s with default int type\n", op->name);
        return op;
    }
    
    // 2. 检查type的kind是否有效
    if (type->kind < TK_BASIC || type->kind > TK_STRUCT) {
        printf("[DEBUG] WARNING: Temp operand has invalid type kind=%d (expected 0-3), using int\n", 
               type->kind);
        op->type = new_basic_type(TYPE_INT);
        op->offset = 0;
        printf("[DEBUG] Created temp operand %s with corrected int type\n", op->name);
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
    
    gen_ir_program(node, ir_list, symtab);
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
    if (func_dec->ptr[0] && func_dec->ptr[0]->type_id) {
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
                if (param_name->kind == ID_NODE && param_name->type_id) {
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
            if (param_name->kind == ID_NODE && param_name->type_id) {
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
        case DEF:
            printf("[IR DEBUG] Processing DEF statement\n");
            // 处理变量定义
            if (node->ptr[0] && node->ptr[1]) {
                // ptr[0] 是类型节点，ptr[1] 是声明列表
                ASTNode *dec_list = node->ptr[1];
                if (dec_list && dec_list->kind == INIT_DEC) {
                    // 处理初始化的声明
                    gen_ir_init_dec(dec_list, ir_list, symtab);
                }
            }
            break;
            
        case COMP_ST:
            printf("[IR DEBUG] Processing COMP_ST statement\n");
            printf("[IR DEBUG] COMP_ST has child at ptr[0], kind = %d\n", 
                   node->ptr[0] ? node->ptr[0]->kind : -1);
            
            // 根据AST输出，语句在 ptr[0] 中（是 STMT_LIST）
            if (node->ptr[0]) {
                printf("[IR DEBUG] COMP_ST child is STMT_LIST, generating statements\n");
                gen_ir_stmt(node->ptr[0], ir_list, next_label, symtab);
            } else {
                printf("[IR DEBUG] COMP_ST has no child\n");
            }
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
            
        case WHILE_STMT:
            printf("[IR DEBUG] Processing WHILE_STMT\n");
            gen_ir_while_stmt(node, ir_list, next_label, symtab);
            break;
            
        case STMT_LIST:
            printf("[IR DEBUG] Processing STMT_LIST\n");
            printf("[IR DEBUG] STMT_LIST ptr[0] = %p, ptr[1] = %p\n", 
                   (void*)node->ptr[0], (void*)node->ptr[1]);
            
            // 处理第一个语句
            if (node->ptr[0]) {
                printf("[IR DEBUG] Processing first statement in STMT_LIST\n");
                gen_ir_stmt(node->ptr[0], ir_list, next_label, symtab);
            }
            
            // 递归处理剩余的语句列表
            if (node->ptr[1]) {
                printf("[IR DEBUG] Processing remaining statements in STMT_LIST\n");
                gen_ir_stmt(node->ptr[1], ir_list, next_label, symtab);
            } else {
                printf("[IR DEBUG] No more statements in STMT_LIST\n");
            }
            break;
            
        default:
            printf("[IR DEBUG] Unhandled statement kind: %d\n", node->kind);
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
                Operand *const_op = new_const_operand_float(node->type_float);
                if (const_op) {
                    IRCode *assign_code = new_ir_code(IR_ASSIGN, const_op, NULL, result);
                    if (assign_code) {
                        append_ir_code(ir_list, assign_code);
                    }
                }
            }
            break;
            
        case BINARY_EXP:
            gen_ir_binary_expr(node, ir_list, result, symtab);
            break;
            
        case ASSIGN_EXP:
            gen_ir_assign_expr(node, ir_list, result, symtab);
            break;
            
        case FUNC_CALL:
            gen_ir_func_call(node, ir_list, result, symtab);
            break;
            
        default:
            // 其他表达式类型
            break;
    }
}

/* 生成二元表达式的中间代码 */
static void gen_ir_binary_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list || !result) return;
    
    printf("[DEBUG] Generating binary expression, op: %s\n", node->type_id ? node->type_id : "unknown");
    
    ASTNode *left = node->ptr[0];
    ASTNode *right = node->ptr[1];
    
    if (!left || !right) return;
    
    // 为左右操作数生成临时变量
    Type *left_type = left->type_info;
    Type *right_type = right->type_info;
    
    /* 调试信息 */
    if (left_type) {
        printf("[DEBUG] Left type: kind=%d\n", left_type->kind);
    }
    if (right_type) {
        printf("[DEBUG] Right type: kind=%d\n", right_type->kind);
    }
    
    /* 确保类型不是函数类型 */
    if (!left_type) {
        left_type = new_basic_type(TYPE_INT);
        printf("[DEBUG] Using int as left type (was NULL)\n");
    } else if (left_type->kind == TK_FUNCTION) {
        printf("[WARNING] Left operand has function type, using int instead\n");
        left_type = new_basic_type(TYPE_INT);
    }
    
    if (!right_type) {
        right_type = new_basic_type(TYPE_INT);
        printf("[DEBUG] Using int as right type (was NULL)\n");
    } else if (right_type->kind == TK_FUNCTION) {
        printf("[WARNING] Right operand has function type, using int instead\n");
        right_type = new_basic_type(TYPE_INT);
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
        else {
            printf("[WARNING] Unknown binary operator: %s, using ADD\n", op_str);
            op_code = IR_ADD;
        }
    } else {
        printf("[WARNING] Binary operator is NULL, using ADD\n");
        op_code = IR_ADD;
    }
    
    IRCode *binary_code = new_ir_code(op_code, left_temp, right_temp, result);
    if (binary_code) {
        append_ir_code(ir_list, binary_code);
        printf("[DEBUG] Created binary IR code: %s\n", op_str ? op_str : "unknown");
    } else {
        printf("[ERROR] Failed to create binary IR code\n");
    }
}

/* 生成赋值表达式的中间代码 */
static void gen_ir_assign_expr(ASTNode *node, IRList *ir_list, Operand *result, SymbolTable *symtab) {
    if (!node || !ir_list || !result) return;
    
    ASTNode *left = node->ptr[0];
    ASTNode *right = node->ptr[1];
    
    if (!left || !right) return;
    
    // 生成右操作数的代码
    Type *right_type = right->type_info;
    if (!right_type) right_type = new_basic_type(TYPE_INT);
    
    Operand *right_temp = new_temp_operand(right_type);
    if (!right_temp) return;
    
    gen_ir_expr(right, ir_list, right_temp, symtab);
    
    // 如果是变量赋值
    if (left->symbol_ref) {
        Operand *left_op = new_var_operand(left->symbol_ref);
        if (left_op) {
            IRCode *assign_code = new_ir_code(IR_ASSIGN, right_temp, NULL, left_op);
            if (assign_code) {
                append_ir_code(ir_list, assign_code);
            }
            
            // 将结果赋给result
            IRCode *copy_code = new_ir_code(IR_ASSIGN, right_temp, NULL, result);
            if (copy_code) {
                append_ir_code(ir_list, copy_code);
            }
        }
    } else {
        // 直接赋值给result
        IRCode *assign_code = new_ir_code(IR_ASSIGN, right_temp, NULL, result);
        if (assign_code) {
            append_ir_code(ir_list, assign_code);
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

/* 生成while语句的中间代码 */
static void gen_ir_while_stmt(ASTNode *node, IRList *ir_list, char *next_label, SymbolTable *symtab) {
    if (!node || node->kind != WHILE_STMT || !ir_list) return;
    
    ASTNode *cond = node->ptr[0];
    ASTNode *body = node->ptr[1];
    
    if (!cond || !body) return;
    
    // 生成标签
    Operand *test_label = new_label_operand();
    Operand *body_label = new_label_operand();
    Operand *end_label = new_label_operand();
    
    if (!test_label || !body_label || !end_label) return;
    
    // 测试标签
    IRCode *label_test = new_ir_code(IR_LABEL, NULL, NULL, test_label);
    if (label_test) {
        append_ir_code(ir_list, label_test);
    }
    
    // 为条件表达式生成临时变量
    Type *cond_type = cond->type_info;
    if (!cond_type) cond_type = new_basic_type(TYPE_INT);
    
    Operand *cond_temp = new_temp_operand(cond_type);
    if (!cond_temp) return;
    
    gen_ir_expr(cond, ir_list, cond_temp, symtab);
    
    // 条件跳转到循环体
    IRCode *if_code = new_ir_code(IR_IF, cond_temp, NULL, body_label);
    if (if_code) {
        append_ir_code(ir_list, if_code);
    }
    
    // 跳转到结束
    IRCode *goto_end = new_ir_code(IR_GOTO, NULL, NULL, end_label);
    if (goto_end) {
        append_ir_code(ir_list, goto_end);
    }
    
    // 循环体标签
    IRCode *label_body = new_ir_code(IR_LABEL, NULL, NULL, body_label);
    if (label_body) {
        append_ir_code(ir_list, label_body);
    }
    
    // 生成循环体
    gen_ir_stmt(body, ir_list, NULL, symtab);
    
    // 跳回测试
    IRCode *goto_test = new_ir_code(IR_GOTO, NULL, NULL, test_label);
    if (goto_test) {
        append_ir_code(ir_list, goto_test);
    }
    
    // 结束标签
    IRCode *label_end = new_ir_code(IR_LABEL, NULL, NULL, end_label);
    if (label_end) {
        append_ir_code(ir_list, label_end);
    }
}