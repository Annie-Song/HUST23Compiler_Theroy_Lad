#include "semantic.h"
#include "../ast/ast.h"
#include "../symbol/symbol.h"
#include "type.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define DEBUG_SEMANTIC 1  /* 设置为0关闭调试，1打开调试 */

#if SEMANTIC_DEBUG
    #define SEM_DEBUG(...) printf("[SEM] " __VA_ARGS__)
#else
    #define SEM_DEBUG(...)
#endif

/* ================== 数据结构定义 ================== */

/* 语义分析上下文 */
typedef struct {
    int in_loop;
    int in_function;
    Type *return_type;
    int has_return;
    SymbolTable *symbol_table;
} SemanticContext;

/* ================== 全局变量 ================== */

static SymbolTable *symbol_table = NULL;

/* ================== 内部函数声明 ================== */

static void local_semantic_error(int line, int col, ErrorType err_type, const char *fmt, ...);

static void init_semantic_analysis(void);
static void check_program(ASTNode *node);
static void check_ext_def(ASTNode *node);
static void check_global_var_def(ASTNode *node);
static void check_ext_dec_list(ASTNode *node, Type *type, int is_global);
static void check_var_dec(ASTNode *node, Type *type, int is_global);
static void check_array_dec(ASTNode *node, Type *type, int is_global);
static void check_init_dec(ASTNode *node, Type *type, int is_global);
static void check_func_def(ASTNode *node);
static void check_compound_stmt(ASTNode *node, SemanticContext *ctx);
static void check_local_defs(ASTNode *node, SemanticContext *ctx);
static void check_def(ASTNode *node, SemanticContext *ctx);
static void check_dec_list(ASTNode *node, Type *type, int is_global, SemanticContext *ctx);
static void check_dec(ASTNode *node, Type *type, int is_global, SemanticContext *ctx);
static void check_stmt_list(ASTNode *node, SemanticContext *ctx);
static void check_stmt(ASTNode *node, SemanticContext *ctx);
static void check_expr(ASTNode *node, SemanticContext *ctx);
static void check_id_expr(ASTNode *node, SemanticContext *ctx);
static void check_binary_expr(ASTNode *node, SemanticContext *ctx);
static void check_unary_expr(ASTNode *node, SemanticContext *ctx);
static void check_assign_expr(ASTNode *node, SemanticContext *ctx);
static void check_func_call(ASTNode *node, SemanticContext *ctx);
static void check_array_access(ASTNode *node, SemanticContext *ctx);
static void check_return_stmt(ASTNode *node, SemanticContext *ctx);
static void check_if_stmt(ASTNode *node, SemanticContext *ctx);
static void check_if_else_stmt(ASTNode *node, SemanticContext *ctx);
static void check_while_stmt(ASTNode *node, SemanticContext *ctx);
static void check_for_stmt(ASTNode *node, SemanticContext *ctx);
static void check_break_stmt(ASTNode *node, SemanticContext *ctx);
static void check_continue_stmt(ASTNode *node, SemanticContext *ctx);
static Type *get_expr_type(ASTNode *node, SemanticContext *ctx);
static Type *get_type_from_ast(ASTNode *node);
static int type_compatible(Type *t1, Type *t2);

/* 新增函数声明 */
static void check_func_params(ASTNode *node, Type *return_type);
static void check_func_param(ASTNode *node);
static int check_assignment_compatible(Type *target, Type *source);
/* ================== 工具函数 ================== */

/* 本地错误处理 */
static void local_semantic_error(int line, int col, ErrorType err_type, const char *fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    semantic_error(line, col, err_type, buffer);
}

/* 类型兼容性检查 */
static int type_compatible(Type *t1, Type *t2) {
    if (!t1 || !t2) {
        printf("[DEBUG] type_compatible: one or both types are NULL\n");
        return 0;
    }
    
    printf("[DEBUG] type_compatible: t1->kind=%d, t2->kind=%d\n", t1->kind, t2->kind);
    
    /* 如果类型完全相同 */
    if (type_equal(t1, t2)) {
        printf("[DEBUG] type_compatible: types are equal\n");
        return 1;
    }
    
    /* 如果都是基本类型，检查基本类型是否相同 */
    if (t1->kind == TK_BASIC && t2->kind == TK_BASIC) {
        printf("[DEBUG] type_compatible: both basic, t1->basic=%d, t2->basic=%d\n", 
               t1->basic, t2->basic);
        return t1->basic == t2->basic;
    }
    
    printf("[DEBUG] type_compatible: types not compatible\n");
    return 0;
}

/* 初始化语义分析 */
static void init_semantic_analysis(void) {
    symbol_table = create_symbol_table();
    if (symbol_table) {
        init_symbol_table(symbol_table);
    }
}

/* 从AST结点获取类型 */
static Type *get_type_from_ast(ASTNode *node) {
    if (!node) return new_basic_type(TYPE_VOID);
    
    switch (node->kind) {
        case TYPE_NODE_INT:
            return new_basic_type(TYPE_INT);
        case TYPE_NODE_FLOAT:
            return new_basic_type(TYPE_FLOAT);
        case TYPE_NODE_CHAR:
            return new_basic_type(TYPE_CHAR);
        case TYPE_NODE_VOID:
            return new_basic_type(TYPE_VOID);
        default:
            return new_basic_type(TYPE_INT);
    }
}

/* 获取表达式类型 */
static Type *get_expr_type(ASTNode *node, SemanticContext *ctx) {
    if (!node) {
        printf("[DEBUG] get_expr_type: node is NULL\n");
        return new_basic_type(TYPE_VOID);
    }

    /* 如果还没检查过，先检查 */
    if (!node->type_info) {
        printf("[DEBUG] get_expr_type: calling check_expr for node kind=%d\n", node->kind);
        check_expr(node, ctx);
    }
    
     if (!node->type_info) {
        printf("[DEBUG] get_expr_type: type_info still NULL after check_expr\n");
        return new_basic_type(TYPE_INT);
    }
    
    printf("[DEBUG] get_expr_type: returning type kind=%d\n", node->type_info->kind);
    return node->type_info;
}

/* ================== 主入口函数 ================== */

/* 语义分析主函数 */
void semantic_analysis(ASTNode *root) {
    if (!root) return;
    
    printf("\n=== Starting Semantic Analysis ===\n");
    
    init_semantic_analysis();
    
    /* 检查程序 */
    check_program(root);
    
    /* 显示符号表（调试） */
    #ifdef SHOW_SYMBOL_TABLE
    if (symbol_table) {
        print_symbol_table(symbol_table);
    }
    #endif
    
    /* 打印所有错误 */
    print_all_errors();
    
    /* 清理 */
    if (symbol_table) {
        destroy_symbol_table(symbol_table);
        symbol_table = NULL;
    }
    
    if (!has_errors()) {
        printf("\n✓ Semantic analysis passed!\n");
    } else {
        printf("\n✗ Semantic analysis failed with %d errors.\n", get_error_count());
    }
}

/* ================== 程序结构检查 ================== */

/* 检查程序 */
static void check_program(ASTNode *node) {
    if (!node) return;
    
    printf("[DEBUG] check_program: node kind = %d\n", node->kind);

    switch (node->kind) {
        case EXT_DEF_LIST:
            /* 处理外部定义列表 */
            if (node->ptr[0]) check_ext_def(node->ptr[0]);
            if (node->ptr[1]) check_program(node->ptr[1]);
            break;
            
        default:
            /* 递归检查所有子树 */
            for (int i = 0; i < 4; i++) {
                if (node->ptr[i]) check_program(node->ptr[i]);
            }
            break;
    }
}

/* 检查外部定义 */
static void check_ext_def(ASTNode *node) {
    if (!node) return;
    
    printf("[DEBUG] check_ext_def: node kind = %d\n", node->kind);

    if (node->kind == EXT_VAR_DEF) {
        printf("[DEBUG] Found EXT_VAR_DEF\n");
        check_global_var_def(node);
    } else if (node->kind == FUNC_DEF) {
        printf("[DEBUG] Found FUNC_DEF\n");
        check_func_def(node);
    }
}

/* 检查全局变量定义 */
static void check_global_var_def(ASTNode *node) {
    if (!node || node->kind != EXT_VAR_DEF) return;
    
    ASTNode *spec = node->ptr[0];
    ASTNode *decl = node->ptr[1];
    
    if (!spec || !decl) return;
    
    /* 获取类型 */
    Type *type = get_type_from_ast(spec);
    
    /* 处理声明列表 */
    if (decl->kind == INIT_DEC) {
        check_init_dec(decl, type, 1);
    } else {
        check_ext_dec_list(decl, type, 1);
    }
    
    free_type(type);
}

/* 检查外部声明列表 */
static void check_ext_dec_list(ASTNode *node, Type *type, int is_global) {
    if (!node) return;
    
    switch (node->kind) {
        case EXT_DEC_LIST:
            if (node->ptr[0]) check_var_dec(node->ptr[0], type, is_global);
            if (node->ptr[1]) check_ext_dec_list(node->ptr[1], type, is_global);
            break;
            
        case INIT_DEC:
            check_init_dec(node, type, is_global);
            break;
            
        default:
            check_var_dec(node, type, is_global);
            break;
    }
}

/* ================== 变量和数组声明检查 ================== */

/* 检查变量声明 */
static void check_var_dec(ASTNode *node, Type *type, int is_global) {
    if (!node || !type) return;
    
    if (node->kind == ID_NODE) {
        /* 普通变量 */
        char *name = node->type_id;
        
        /* 检查是否重定义 - 在当前作用域查找 */
        SymbolEntry *existing = lookup_current_scope(symbol_table, name);
        if (existing) {
            local_semantic_error(node->pos, 0, ERR_REDEFINE_VAR, 
                               "variable '%s' redefined", name);
            return;
        }
        
        /* 复制类型给符号表 */
        Type *type_for_symbol = copy_type(type);

        /* 插入符号表 */
        SymbolEntry *entry = insert_symbol(symbol_table, name, SK_VARIABLE, type);
        if (entry) {
            node->symbol_ref = entry;
            node->type_info = copy_type(type);
            node->is_lvalue = 1;
            node->is_const = 0;
        }
        
    } else if (node->kind == ARRAY_DEC) {
        /* 数组声明 */
        check_array_dec(node, type, is_global);
    }
}

/* 检查数组声明 - 支持多维数组 */
static void check_array_dec(ASTNode *node, Type *type, int is_global) {
    if (!node || node->kind != ARRAY_DEC) return;
    
    ASTNode *array_name = node->ptr[0];
    ASTNode *array_size = node->ptr[1];
    
    if (!array_name || !array_size) return;
    
    /* 检查数组大小 */
    if (array_size->kind != INT_NODE || array_size->type_int <= 0) {
        local_semantic_error(array_size->pos, 0, ERR_ARRAY_SUBSCRIPT, 
                           "Array size must be positive integer");
        return;
    }
    
    int size = array_size->type_int;
    
    /* 创建数组类型 */
    Type *array_type = new_array_type(copy_type(type), size);
    
    /* 如果数组名还是数组声明，说明是多维数组 */
    if (array_name->kind == ARRAY_DEC) {
        /* 递归处理多维数组 */
        check_array_dec(array_name, array_type, is_global);
        /* 释放临时创建的类型 */
        free_type(array_type);
    } else if (array_name->kind == ID_NODE) {
        /* 到达变量名，插入符号表 */
        char *name = array_name->type_id;
        
        /* 检查是否重定义 */
        SymbolEntry *existing = lookup_current_scope(symbol_table, name);
        if (existing) {
            local_semantic_error(node->pos, 0, ERR_REDEFINE_VAR, 
                               "variable '%s' redefined", name);
            free_type(array_type);
            return;
        }
        
        /* 计算多维数组的总维度和各维度大小 */
        /* 我们需要收集所有的维度信息 */
        int dims = 1;
        ASTNode *temp = node;
        while (temp->kind == ARRAY_DEC && temp->ptr[0] && temp->ptr[0]->kind == ARRAY_DEC) {
            dims++;
            temp = temp->ptr[0];
        }
        
        /* 分配维度数组 */
        int *dim_sizes = (int *)malloc(dims * sizeof(int));
        if (!dim_sizes) {
            free_type(array_type);
            return;
        }
        
        /* 收集维度大小（从外层到内层） */
        temp = node;
        for (int i = 0; i < dims; i++) {
            if (temp->kind == ARRAY_DEC && temp->ptr[1]) {
                dim_sizes[i] = temp->ptr[1]->type_int;
                temp = temp->ptr[0];
            }
        }
        
        /* 创建最终的数组类型 */
        Type *final_array_type = array_type;
        /* 从内层到外层创建类型 */
        for (int i = dims - 2; i >= 0; i--) {
            Type *temp_type = final_array_type;
            final_array_type = new_array_type(temp_type, dim_sizes[i]);
            free_type(temp_type);
        }
        
        /* 插入符号表 */
        SymbolEntry *entry = insert_array(symbol_table, name, type, dims, dim_sizes);
        if (entry) {
            array_name->symbol_ref = entry;
            array_name->type_info = final_array_type;
            array_name->is_lvalue = 1;
            
            /* 设置数组额外信息 */
            array_name->array_info.array_size = dim_sizes[dims-1];  /* 最内层大小 */
            array_name->array_info.elem_type = copy_type(type);
        }
        
        free(dim_sizes);
        /* 注意：final_array_type 现在被符号表管理，不要在这里释放 */
    }
}

/* 检查初始化声明 */
static void check_init_dec(ASTNode *node, Type *type, int is_global) {
    if (!node || node->kind != INIT_DEC) return;
    
    ASTNode *var_dec = node->ptr[0];
    ASTNode *init_exp = node->ptr[1];
    
    if (!var_dec) return;
    
    /* 首先检查变量声明 */
    check_var_dec(var_dec, type, is_global);
    
    /* 然后检查初始化表达式 */
    if (init_exp) {
        SemanticContext ctx = {0};
        ctx.symbol_table = symbol_table;
        
        Type *exp_type = get_expr_type(init_exp, &ctx);
        
        /* 检查类型兼容性 */
        if (!type_compatible(type, exp_type)) {
            local_semantic_error(init_exp->pos, 0, ERR_TYPE_MISMATCH,
                               "Initializer type mismatch");
        }
        
        free_type(exp_type);
    }
}

/* ================== 函数定义检查 ================== */

/* 检查函数定义 - 修复版本 */
static void check_func_def(ASTNode *node) {
    if (!node || node->kind != FUNC_DEF) return;
    
    printf("[DEBUG] === ENTER check_func_def ===\n");
    printf("[DEBUG] Function definition at line %d\n", node->pos);
    
    ASTNode *spec = node->ptr[0];
    ASTNode *func_dec = node->ptr[1];
    ASTNode *body = node->ptr[2];
    
    if (!spec || !func_dec) {
        printf("[DEBUG] Missing spec or func_dec\n");
        return;
    }
    
    /* 获取函数名 */
    char func_name[32] = "";
    if (func_dec->kind == FUNC_DEC && func_dec->ptr[0]) {
        strcpy(func_name, func_dec->ptr[0]->type_id);
    }
    
    printf("[DEBUG] Function name: %s\n", func_name);
    
    /* 获取返回类型 */
Type *return_type = get_type_from_ast(spec);
printf("[DEBUG] check_func_def: return_type kind=%d\n", return_type->kind);

/* 创建函数类型 */
Type *func_type = new_func_type(return_type, NULL, 0);
printf("[DEBUG] check_func_def: created func_type, func_type->func.return_type kind=%d\n", 
       func_type->func.return_type ? func_type->func.return_type->kind : -1);
    
    /* 插入函数符号 - 传递返回类型，不是函数类型 */
    SymbolEntry *func_sym = insert_function(symbol_table, func_name, return_type, 0);
    if (!func_sym) {
        printf("[DEBUG] Failed to insert function symbol\n");
        local_semantic_error(node->pos, 0, ERR_REDEFINE_FUNC, func_name);
        free_type(return_type);
        return;
    }
    
    printf("[DEBUG] Function symbol inserted successfully\n");
    
    /* 进入函数作用域 */
    enter_scope(symbol_table, func_name);
    printf("[DEBUG] Entered function scope\n");
    
    /* 设置上下文 */
    SemanticContext func_ctx = {0};
    func_ctx.in_function = 1;
    func_ctx.return_type = copy_type(return_type);  // 复制返回类型给上下文
    func_ctx.has_return = 0;
    func_ctx.symbol_table = symbol_table;
    
    printf("[DEBUG] Context created: has_return = %d\n", func_ctx.has_return);
    
    /* ===== 处理函数参数 ===== */
    if (func_dec->kind == FUNC_DEC && func_dec->ptr[1]) {
        printf("[DEBUG] Processing function parameters\n");
        check_func_params(func_dec->ptr[1], return_type);
    }
    /* ===== 结束处理参数 ===== */
    
    /* 检查函数体 */
    if (body && body->kind == COMP_ST) {
        printf("[DEBUG] Checking function body (COMP_ST)\n");
        check_compound_stmt(body, &func_ctx);
    } else {
        printf("[DEBUG] No function body or not COMP_ST\n");
    }
    
    printf("[DEBUG] After checking body: has_return = %d\n", func_ctx.has_return);
    
    /* 检查非void函数是否有return */
    if (return_type->kind == TK_BASIC && return_type->basic != TYPE_VOID) {
        if (!func_ctx.has_return) {
            printf("[DEBUG] ERROR: Function %s missing return statement\n", func_name);
            local_semantic_error(node->pos, 0, ERR_FUNC_NO_RETURN, func_name);
        } else {
            printf("[DEBUG] OK: Function %s has return statement\n", func_name);
        }
    }
    
    /* 标记函数已定义 */
    mark_function_defined(symbol_table, func_sym);
    
    /* 退出函数作用域 */
    exit_scope(symbol_table);
    printf("[DEBUG] Exited function scope\n");
    
    /* 清理 - 注意：func_type 现在由符号表管理，不要释放它 */
    free_type(return_type);
    free_type(func_ctx.return_type);
   
    printf("[DEBUG] === EXIT check_func_def ===\n\n");
}

/* 检查复合语句 */
static void check_compound_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || node->kind != COMP_ST) return;
    
    printf("[DEBUG] check_compound_stmt at line %d\n", node->pos);
    
    /* 打印所有孩子节点信息用于调试 */
    for (int i = 0; i < 4; i++) {
        if (node->ptr[i]) {
            printf("[DEBUG] ptr[%d] = %p, kind = %d\n", 
                   i, (void*)node->ptr[i], node->ptr[i]->kind);
        } else {
            printf("[DEBUG] ptr[%d] = NULL\n", i);
        }
    }
    
    /* 进入新的作用域 */
    enter_scope(symbol_table, "block");
    
    /* 简化逻辑：直接检查所有可能的孩子 */
    /* 先检查声明部分（如果有） */
    if (node->ptr[0] && node->ptr[0]->kind == DEF_LIST) {
        printf("[DEBUG] Found DEF_LIST at ptr[0]\n");
        check_local_defs(node->ptr[0], ctx);
    }
    
    /* 然后检查语句部分 */
    /* 语句可能在 ptr[0]（如果没有声明）或 ptr[1]（如果有声明） */
    ASTNode *stmt_node = NULL;
    if (node->ptr[0] && node->ptr[0]->kind != DEF_LIST) {
        /* 第一个孩子不是声明，那么它应该是语句 */
        stmt_node = node->ptr[0];
        printf("[DEBUG] Statements at ptr[0], kind = %d\n", stmt_node->kind);
    } else if (node->ptr[1]) {
        /* 第二个孩子是语句 */
        stmt_node = node->ptr[1];
        printf("[DEBUG] Statements at ptr[1], kind = %d\n", stmt_node->kind);
    }
    
    if (stmt_node) {
        printf("[DEBUG] Checking statements...\n");
        check_stmt_list(stmt_node, ctx);
    } else {
        printf("[DEBUG] No statements found in COMP_ST\n");
    }
    
    /* 退出作用域 */
    exit_scope(symbol_table);
}

/* 检查局部定义 */
static void check_local_defs(ASTNode *node, SemanticContext *ctx) {
    if (!node) return;
    
    if (node->kind == DEF_LIST) {
        if (node->ptr[0]) check_def(node->ptr[0], ctx);
        if (node->ptr[1]) check_local_defs(node->ptr[1], ctx);
    }
}

/* 检查定义 */
static void check_def(ASTNode *node, SemanticContext *ctx) {
    if (!node || node->kind != DEF) return;
    
    ASTNode *spec = node->ptr[0];
    ASTNode *dec_list = node->ptr[1];
    
    if (!spec || !dec_list) return;
    
    Type *type = get_type_from_ast(spec);
    check_dec_list(dec_list, type, 0, ctx);
    free_type(type);
}

/* 检查声明列表 */
static void check_dec_list(ASTNode *node, Type *type, int is_global, SemanticContext *ctx) {
    if (!node) return;
    
    if (node->kind == DEC_LIST) {
        if (node->ptr[0]) check_dec(node->ptr[0], type, is_global, ctx);
        if (node->ptr[1]) check_dec_list(node->ptr[1], type, is_global, ctx);
    } else {
        check_dec(node, type, is_global, ctx);
    }
}

/* 检查声明 */
static void check_dec(ASTNode *node, Type *type, int is_global, SemanticContext *ctx) {
    if (!node) return;
    
    if (node->kind == INIT_DEC) {
        check_init_dec(node, type, is_global);
    } else if (node->kind == ARRAY_DEC) {
        /* 直接处理数组声明 */
        check_array_dec(node, type, is_global);
    } else {
        check_var_dec(node, type, is_global);
    }
}

/* 检查语句列表 - 修复版本 */
static void check_stmt_list(ASTNode *node, SemanticContext *ctx) {
    if (!node) {
        printf("[DEBUG] check_stmt_list: node is NULL\n");
        return;
    }
    
    printf("[DEBUG] check_stmt_list: node kind = %d at line %d\n", node->kind, node->pos);
    
    if (node->kind == STMT_LIST) {
        /* 检查第一个语句 */
        if (node->ptr[0]) {
            printf("[DEBUG] Checking first statement in list\n");
            check_stmt(node->ptr[0], ctx);
        } else {
            printf("[DEBUG] First statement is NULL\n");
        }
        
        /* 递归检查剩余语句列表 */
        if (node->ptr[1]) {
            printf("[DEBUG] Checking remaining statements\n");
            check_stmt_list(node->ptr[1], ctx);
        }
    } else {
        /* 如果node不是STMT_LIST，直接检查它 */
        printf("[DEBUG] Node is not STMT_LIST, checking as statement\n");
        check_stmt(node, ctx);
        
        /* 关键修复：确保return语句的设置被传播 */
        if (node->kind == RETURN_STMT && ctx) {
            printf("[DEBUG] check_stmt_list: processed RETURN_STMT, ensuring has_return flag is set\n");
            ctx->has_return = 1;
        }
    }
}

/* 检查语句 */
static void check_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) {
        printf("[DEBUG] check_stmt: node or ctx is NULL\n");
        return;
    }
    
    printf("[DEBUG] check_stmt: kind = %d at line %d, ctx pointer = %p\n", 
           node->kind, node->pos, (void*)ctx);
    
    switch (node->kind) {
        case EXP_STMT:
            printf("[DEBUG] Found EXP_STMT\n");
             if (node->ptr[0]) {
                // 首先检查是否为 DEF 节点
                if (node->ptr[0]->kind == DEF) {
                    // 如果是 DEF 节点，调用 check_def
                    check_def(node->ptr[0], ctx);
                } else {
                    // 否则，作为普通表达式检查
                    check_expr(node->ptr[0], ctx);
                }
            }
            break;
            
        case COMP_ST:
            printf("[DEBUG] Found COMP_ST\n");
            check_compound_stmt(node, ctx);
            break;
            
        case RETURN_STMT:
            printf("[DEBUG] Found RETURN_STMT - calling check_return_stmt\n");
            printf("[DEBUG] check_stmt: ctx->has_return before = %d\n", ctx->has_return);
            check_return_stmt(node, ctx);
            printf("[DEBUG] check_stmt: ctx->has_return after = %d\n", ctx->has_return);
            break;
            
        case IF_STMT:
            printf("[DEBUG] Found IF_STMT\n");
            check_if_stmt(node, ctx);
            break;
            
        case IF_ELSE_STMT:
            printf("[DEBUG] Found IF_ELSE_STMT\n");
            check_if_else_stmt(node, ctx);
            break;
            
        case WHILE_STMT:
            printf("[DEBUG] Found WHILE_STMT\n");
            check_while_stmt(node, ctx);
            break;
            
        case FOR_STMT:
            printf("[DEBUG] Found FOR_STMT\n");
            check_for_stmt(node, ctx);
            break;
            
        case BREAK_STMT:
            printf("[DEBUG] Found BREAK_STMT\n");
            check_break_stmt(node, ctx);
            break;
            
        case CONTINUE_STMT:
            printf("[DEBUG] Found CONTINUE_STMT\n");
            check_continue_stmt(node, ctx);
            break;
            
        case DEF:
            printf("[DEBUG] Found DEF as statement\n");
            check_def(node, ctx);
            break;
            
        default:
            printf("[DEBUG] Unknown statement kind: %d\n", node->kind);
            break;
    }
}

/* 检查return语句 */
static void check_return_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    printf("[DEBUG] ===== ENTER check_return_stmt at line %d =====\n", node->pos);
    printf("[DEBUG] Before: ctx->has_return = %d\n", ctx->has_return);
    
    /* 重要：设置has_return标志 */
    ctx->has_return = 1;
    
    printf("[DEBUG] After: ctx->has_return = %d\n", ctx->has_return);
    
    if (node->ptr[0]) {
        printf("[DEBUG] Return has expression\n");
        Type *expr_type = get_expr_type(node->ptr[0], ctx);
        
        /* 新增调试信息 */
        printf("[DEBUG] Return type: kind = %d, basic = %d\n", 
               ctx->return_type->kind, 
               ctx->return_type->kind == TK_BASIC ? ctx->return_type->basic : -1);
        printf("[DEBUG] Expression type: kind = %d, basic = %d\n", 
               expr_type->kind, 
               expr_type->kind == TK_BASIC ? expr_type->basic : -1);
        
        if (!type_compatible(ctx->return_type, expr_type)) {
            local_semantic_error(node->pos, 0, ERR_RETURN_TYPE, "return type mismatch");
        }
        free_type(expr_type);
    } else {
        printf("[DEBUG] Return has no expression\n");
        if (ctx->return_type->kind == TK_BASIC && ctx->return_type->basic != TYPE_VOID) {
            local_semantic_error(node->pos, 0, ERR_RETURN_TYPE, "void return in non-void function");
        }
    }
    
    printf("[DEBUG] ===== EXIT check_return_stmt =====\n");
}

/* 检查if语句 */
static void check_if_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    if (node->ptr[1]) check_stmt(node->ptr[1], ctx);
}

/* 检查if-else语句 */
static void check_if_else_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    if (node->ptr[1]) check_stmt(node->ptr[1], ctx);
    if (node->ptr[2]) check_stmt(node->ptr[2], ctx);
}

/* 检查while语句 */
static void check_while_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    int saved_in_loop = ctx->in_loop;
    ctx->in_loop = 1;
    
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    if (node->ptr[1]) check_stmt(node->ptr[1], ctx);
    
    ctx->in_loop = saved_in_loop;
}

/* 检查for语句 - 修复版本 */
static void check_for_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    int saved_in_loop = ctx->in_loop;
    ctx->in_loop = 1;
    
    printf("[DEBUG] check_for_stmt: entering FOR loop at line %d\n", node->pos);
    
    /* ===== 重要修复：为for循环创建新的作用域 ===== */
    /* for循环的初始化部分可能包含变量声明（如 int j = 0）*/
    enter_scope(symbol_table, "for_loop");
    
    /* 处理初始化部分（可能包含变量声明）*/
    if (node->ptr[0]) {
        printf("[DEBUG] check_for_stmt: checking init part\n");
        /* 检查初始化表达式 - 可能是DEF或EXP_STMT */
        check_expr(node->ptr[0], ctx);
    }
    
    /* 处理条件部分 */
    if (node->ptr[1]) {
        printf("[DEBUG] check_for_stmt: checking condition part\n");
        Type *cond_type = get_expr_type(node->ptr[1], ctx);
        /* 条件应为布尔类型 */
        if (cond_type && cond_type->kind != TK_BASIC) {
            local_semantic_error(node->ptr[1]->pos, 0, ERR_TYPE_MISMATCH,
                               "Condition must be boolean expression");
        }
        free_type(cond_type);
    }
    
    /* 处理更新部分 */
    if (node->ptr[2]) {
        printf("[DEBUG] check_for_stmt: checking update part\n");
        check_expr(node->ptr[2], ctx);
    }
    
    /* 处理循环体 */
    if (node->ptr[3]) {
        printf("[DEBUG] check_for_stmt: checking loop body\n");
        check_stmt(node->ptr[3], ctx);
    }
    
    /* ===== 重要修复：退出for循环作用域 ===== */
    exit_scope(symbol_table);
    
    ctx->in_loop = saved_in_loop;
    printf("[DEBUG] check_for_stmt: exiting FOR loop\n");
}

/* 检查break语句 */
static void check_break_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (!ctx->in_loop) {
        local_semantic_error(node->pos, 0, ERR_BREAK_OUT_LOOP, "break statement");
    }
}

/* 检查continue语句 */
static void check_continue_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (!ctx->in_loop) {
        local_semantic_error(node->pos, 0, ERR_CONTINUE_OUT_LOOP, "continue statement");
    }
}

/* ================== 表达式检查 ================== */

/* 检查表达式 */
static void check_expr(ASTNode *node, SemanticContext *ctx) {
    if (!node) return;
    
    /* 如果已经检查过，直接返回 */
    if (node->type_info) return;
    
    switch (node->kind) {
        case DEF:  /* 新增：处理定义表达式 */
            printf("[DEBUG] check_expr: found DEF node, calling check_def\n");
            check_def(node, ctx);
            node->type_info = new_basic_type(TYPE_INT);  /* 定义表达式返回int */
            node->is_lvalue = 0;
            break;

        case ID_NODE:
            check_id_expr(node, ctx);
            break;
            
        case INT_NODE:
            node->type_info = new_basic_type(TYPE_INT);
            node->is_lvalue = 0;
            node->is_const = 1;
            break;
            
        case FLOAT_NODE:
            node->type_info = new_basic_type(TYPE_FLOAT);
            node->is_lvalue = 0;
            node->is_const = 1;
            break;
            
        case CHAR_NODE:
            node->type_info = new_basic_type(TYPE_CHAR);
            node->is_lvalue = 0;
            node->is_const = 1;
            break;
            
        case BINARY_EXP:
            check_binary_expr(node, ctx);
            break;
            
        case UNARY_EXP:
            check_unary_expr(node, ctx);
            break;
            
        case ASSIGN_EXP:
            check_assign_expr(node, ctx);
            break;
            
        case FUNC_CALL:
            check_func_call(node, ctx);
            break;
            
        case ARRAY_ACCESS:
            check_array_access(node, ctx);
            break;
            
        default:
            /* 递归检查子树 */
            for (int i = 0; i < 4; i++) {
                if (node->ptr[i]) check_expr(node->ptr[i], ctx);
            }
            break;
    }
}

/* 检查标识符表达式 */
static void check_id_expr(ASTNode *node, SemanticContext *ctx) {
    if (!node || node->kind != ID_NODE) return;
    
    char *name = node->type_id;
    printf("[DEBUG] check_id_expr: looking up %s\n", name);
    
    /* 查找符号 */
    SymbolEntry *sym = lookup_symbol(symbol_table, name);
    if (!sym) {
        printf("[DEBUG] check_id_expr: symbol not found for %s\n", name);
        local_semantic_error(node->pos, 0, ERR_UNDEFINED_VAR, name);
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    printf("[DEBUG] check_id_expr: found symbol, kind=%d, type=%p\n", 
           sym->kind, (void*)sym->type);
    
    /* 详细检查类型结构 */
    if (sym->type) {
        printf("[DEBUG] check_id_expr: sym->type address: %p\n", (void*)sym->type);
        printf("[DEBUG] check_id_expr: sym->type->kind: %d\n", sym->type->kind);
        printf("[DEBUG] check_id_expr: sym->type->size: %d\n", sym->type->size);
        printf("[DEBUG] check_id_expr: sym->type->align: %d\n", sym->type->align);
        
        if (sym->type->kind == TK_BASIC) {
            printf("[DEBUG] check_id_expr: sym->type->basic: %d\n", sym->type->basic);
        }
    } else {
        printf("[DEBUG] check_id_expr: WARNING: sym->type is NULL!\n");
    }
    
    /* 检查类型指针是否有效 */
    if (sym->type == NULL) {
        printf("[DEBUG] check_id_expr: sym->type is NULL, using default int type\n");
        node->type_info = new_basic_type(TYPE_INT);
    } else if (sym->type->kind < 0 || sym->type->kind > 3) {
        /* 无效的 type->kind 值 */
        printf("[DEBUG] check_id_expr: WARNING: sym->type has invalid kind=%d, using default int type\n", 
               sym->type->kind);
        node->type_info = new_basic_type(TYPE_INT);
    } else {
        node->type_info = copy_type(sym->type);
        if (node->type_info) {
            printf("[DEBUG] check_id_expr: set type_info, kind=%d\n", node->type_info->kind);
        } else {
            printf("[DEBUG] check_id_expr: copy_type returned NULL!\n");
            node->type_info = new_basic_type(TYPE_INT);
        }
    }
    
    node->symbol_ref = sym;
    node->is_lvalue = (sym->kind == SK_VARIABLE || 
                       sym->kind == SK_ARRAY || 
                       sym->kind == SK_PARAMETER);
    node->is_const = sym->is_const;
}

/* 检查二进制表达式 */
static void check_binary_expr(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    /* 检查左右操作数 */
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    if (node->ptr[1]) check_expr(node->ptr[1], ctx);
    
    /* 简单类型推断：取左操作数的类型 */
    if (node->ptr[0] && node->ptr[0]->type_info) {
        node->type_info = copy_type(node->ptr[0]->type_info);
    } else {
        node->type_info = new_basic_type(TYPE_INT);
    }
    node->is_lvalue = 0;
}

/* 检查一元表达式 */
static void check_unary_expr(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    
    /* 取操作数的类型 */
    if (node->ptr[0] && node->ptr[0]->type_info) {
        node->type_info = copy_type(node->ptr[0]->type_info);
    } else {
        node->type_info = new_basic_type(TYPE_INT);
    }
    node->is_lvalue = 0;
}

/* 检查赋值表达式 */
static void check_assign_expr(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    if (node->ptr[0]) check_expr(node->ptr[0], ctx);
    if (node->ptr[1]) check_expr(node->ptr[1], ctx);
    
     /* 获取左右操作数的类型 */
    Type *left_type = node->ptr[0] ? node->ptr[0]->type_info : NULL;
    Type *right_type = node->ptr[1] ? node->ptr[1]->type_info : NULL;

    /* 左值检查 */
    if (node->ptr[0] && !node->ptr[0]->is_lvalue) {
        local_semantic_error(node->ptr[0]->pos, 0, ERR_NOT_LVALUE, 
                           "left side of assignment must be lvalue");
    }
    
    /* 类型兼容性检查 - 使用专门的赋值兼容性检查 */
    if (!check_assignment_compatible(left_type, right_type)) {
        /* 转换为字符串以便显示错误信息 */
        const char *left_type_str = type_to_string(left_type);
        const char *right_type_str = type_to_string(right_type);
        
        local_semantic_error(node->pos, 0, ERR_ASSIGN_TYPE,
                           "cannot assign %s to %s", 
                           right_type_str, left_type_str);
    }
    
    /* 赋值表达式的类型是左操作数的类型 */
    node->type_info = copy_type(left_type);
    node->is_lvalue = 0;
}

/* 检查函数调用 */
static void check_func_call(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    ASTNode *func_name_node = node->ptr[0];
    ASTNode *args_node = node->ptr[1];
    
    if (!func_name_node || func_name_node->kind != ID_NODE) {
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    char *func_name = func_name_node->type_id;
    printf("[DEBUG] check_func_call: looking up function %s\n", func_name);
    
    /* 查找函数符号 */
    SymbolEntry *func_sym = lookup_symbol(symbol_table, func_name);
    if (!func_sym || func_sym->kind != SK_FUNCTION) {
        printf("[DEBUG] check_func_call: function %s not found or not a function\n", func_name);
        local_semantic_error(node->pos, 0, ERR_UNDEFINED_FUNC, func_name);
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    printf("[DEBUG] check_func_call: found function symbol, type=%p\n", (void*)func_sym->type);
    
    Type *return_type = NULL;
    
    /* 确定返回类型 */
    if (func_sym->type) {
        if (func_sym->type->kind == TK_FUNCTION) {
            /* 函数类型：获取返回类型 */
            printf("[DEBUG] check_func_call: function type found\n");
            return_type = func_sym->type->func.return_type;
        } else if (func_sym->type->kind == TK_BASIC) {
            /* 基本类型：直接使用（旧式函数定义） */
            printf("[DEBUG] check_func_call: basic type found (legacy)\n");
            return_type = func_sym->type;
        }
    }
    
    if (return_type) {
        printf("[DEBUG] check_func_call: returning type kind=%d\n", return_type->kind);
        node->type_info = copy_type(return_type);
    } else {
        printf("[DEBUG] check_func_call: no return type found, using int\n");
        node->type_info = new_basic_type(TYPE_INT);
    }
    
    node->is_lvalue = 0;
}

/* 检查数组访问 - 支持多维数组 */
static void check_array_access(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    ASTNode *array_node = node->ptr[0];
    ASTNode *index_node = node->ptr[1];
    
    if (!array_node || !index_node) {
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    /* 检查数组表达式 */
    check_expr(array_node, ctx);
    
    /* 检查下标表达式 */
    check_expr(index_node, ctx);
    
    /* 检查是否为数组 */
    if (!array_node->type_info || array_node->type_info->kind != TK_ARRAY) {
        local_semantic_error(array_node->pos, 0, ERR_NOT_ARRAY, 
                           "subscript on non-array type");
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    /* 检查下标是否为整数类型 */
    if (index_node->type_info && 
        (index_node->type_info->kind != TK_BASIC || 
         index_node->type_info->basic != TYPE_INT)) {
        local_semantic_error(index_node->pos, 0, ERR_ARRAY_SUBSCRIPT, 
                           "array subscript must be integer");
    }
    
    /* 数组访问的结果类型是数组元素类型 */
    Type *result_type = NULL;
    if (array_node->type_info->kind == TK_ARRAY) {
        result_type = copy_type(array_node->type_info->array.elem);
    } else {
        result_type = new_basic_type(TYPE_INT);
    }
    
    /* 如果结果还是数组，设置is_lvalue为1，否则为0 */
    if (result_type && result_type->kind == TK_ARRAY) {
        node->is_lvalue = 1;  /* 多维数组访问仍然是左值 */
        node->type_info = result_type;
    } else {
        node->is_lvalue = 1;  /* 数组元素访问也是左值 */
        node->type_info = result_type;
    }
}

/* 检查函数参数 - 新增函数 */
static void check_func_params(ASTNode *node, Type *return_type) {
    if (!node) return;
    
    printf("[DEBUG] check_func_params: node kind = %d\n", node->kind);
    
    if (node->kind == VAR_LIST) {
        /* 参数列表 */
        if (node->ptr[0]) {
            check_func_param(node->ptr[0]);
        }
        if (node->ptr[1]) {
            check_func_params(node->ptr[1], return_type);
        }
    } else if (node->kind == PARAM_DEC) {
        /* 单个参数声明 */
        check_func_param(node);
    }
}

/* 检查单个函数参数 - 修复版本 */
static void check_func_param(ASTNode *node) {
    if (!node || node->kind != PARAM_DEC) return;
    
    ASTNode *spec = node->ptr[0];
    ASTNode *var_dec = node->ptr[1];
    
    if (!spec || !var_dec) return;
    
    /* 获取参数类型 */
    Type *param_type = get_type_from_ast(spec);
    
    /* 获取参数名 */
    if (var_dec->kind == ID_NODE) {
        char *param_name = var_dec->type_id;
        printf("[DEBUG] Processing parameter: %s\n", param_name);
        
        /* 插入参数到符号表 */
        SymbolEntry *param_sym = insert_symbol(symbol_table, param_name, SK_PARAMETER, param_type);
        if (param_sym) {
            var_dec->symbol_ref = param_sym;
            var_dec->type_info = copy_type(param_type);  // 修复：复制类型
            var_dec->is_lvalue = 1;
            var_dec->is_const = 0;
        }
    }
    
    free_type(param_type);  // 释放原始类型
}

/* 检查赋值兼容性 */
static int check_assignment_compatible(Type *target, Type *source) {
    if (!target || !source) return 0;
    
    printf("[DEBUG] check_assignment_compatible: target kind=%d, source kind=%d\n", 
           target->kind, source->kind);
    
    /* 如果类型完全相同 */
    if (type_equal(target, source)) {
        printf("[DEBUG] check_assignment_compatible: types are equal\n");
        return 1;
    }
    
    /* 如果都是基本类型 */
    if (target->kind == TK_BASIC && source->kind == TK_BASIC) {
        printf("[DEBUG] check_assignment_compatible: target basic=%d, source basic=%d\n", 
               target->basic, source->basic);
        
        /* 严格的C语言规则：int和float不能隐式转换 */
        if (target->basic == source->basic) {
            return 1;  // 相同类型，兼容
        }
        
        /* 可以添加一些隐式转换规则，例如：
         * - int可以赋值给float
         * - char可以赋值给int
         * 但float不能赋值给int（需要显式转换）
         */
        
        /* 这里使用严格规则：必须类型相同 */
        return 0;
    }
    
    /* 其他类型的兼容性规则... */
    
    return 0;
}