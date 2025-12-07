#include "semantic.h"
#include "../ast/ast.h"
#include "../symbol/symbol.h"
#include "type.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
static int get_param_count(ASTNode *node);
static Type **get_param_types(ASTNode *node, int param_count);
static void get_param_types_recursive(ASTNode *node, Type **types, int *index);
static void check_param_list(ASTNode *node, SymbolEntry *func);
static void check_param_dec(ASTNode *node, SymbolEntry *func);
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
static Type *get_expr_type(ASTNode *node, SemanticContext *ctx);  /* 添加这行 */
static Type *get_type_from_ast(ASTNode *node);
static int type_compatible(Type *t1, Type *t2);  /* 确保这个声明在文件开头 */

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
    if (!t1 || !t2) return 0;
    
    /* 简单实现：类型相同或都是基本类型 */
    if (t1->kind == TK_BASIC && t2->kind == TK_BASIC) {
        return 1;
    }
    
    return type_equal(t1, t2);
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

/* 获取表达式类型 - 移到工具函数部分 */
static Type *get_expr_type(ASTNode *node, SemanticContext *ctx) {
    if (!node) return new_basic_type(TYPE_VOID);
    
    /* 如果还没检查过，先检查 */
    if (!node->type_info) {
        check_expr(node, ctx);
    }
    
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
    
    if (node->kind == EXT_VAR_DEF) {
        check_global_var_def(node);
    } else if (node->kind == FUNC_DEF) {
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
    check_ext_dec_list(decl, type, 1);
    
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
        
        /* 检查是否重定义 */
        if (lookup_current_scope(symbol_table, name)) {
            local_semantic_error(node->pos, 0, ERR_REDEFINE_VAR, name);
            return;
        }
        
        /* 插入符号表 */
        SymbolEntry *entry = insert_symbol(symbol_table, name, SK_VARIABLE, type);
        if (entry) {
            node->symbol_ref = entry;
            node->type_info = type;
            node->is_lvalue = 1;
        }
        
    } else if (node->kind == ARRAY_DEC) {
        /* 数组声明 */
        check_array_dec(node, type, is_global);
    }
}

/* 检查数组声明 */
static void check_array_dec(ASTNode *node, Type *type, int is_global) {
    if (!node || node->kind != ARRAY_DEC) return;
    
    ASTNode *array_name = node->ptr[0];
    ASTNode *array_size = node->ptr[1];
    
    if (!array_name || !array_size) return;
    
    if (array_name->kind != ID_NODE) return;
    
    char *name = array_name->type_id;
    
    /* 检查数组大小 */
    if (array_size->kind != INT_NODE || array_size->type_int <= 0) {
        local_semantic_error(array_size->pos, 0, ERR_ARRAY_SUBSCRIPT, 
                           "Array size must be positive integer");
        return;
    }
    
    int size = array_size->type_int;
    
    /* 创建数组类型 */
    Type *array_type = new_array_type(copy_type(type), size);
    
    /* 插入符号表 */
    int dim_sizes[1] = {size};
    SymbolEntry *entry = insert_array(symbol_table, name, type, 1, dim_sizes);
    if (entry) {
        array_name->symbol_ref = entry;
        array_name->type_info = array_type;
        array_name->is_lvalue = 1;
        /* 设置数组额外信息 */
        array_name->array_info.array_size = size;
        array_name->array_info.elem_type = copy_type(type);
    } else {
        free_type(array_type);
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

/* 检查函数定义 */
static void check_func_def(ASTNode *node) {
    if (!node || node->kind != FUNC_DEF) return;
    
    ASTNode *spec = node->ptr[0];
    ASTNode *func_dec = node->ptr[1];
    ASTNode *body = node->ptr[2];
    
    if (!spec || !func_dec) return;
    
    /* 获取函数名 */
    char func_name[32] = "";
    if (func_dec->kind == FUNC_DEC && func_dec->ptr[0]) {
        strcpy(func_name, func_dec->ptr[0]->type_id);
    }
    
    /* 获取返回类型 */
    Type *return_type = get_type_from_ast(spec);
    
    /* 获取参数信息 */
    int param_count = 0;
    Type **param_types = NULL;
    
    if (func_dec->ptr[1]) {
        /* 有参数列表 */
        param_count = get_param_count(func_dec->ptr[1]);
        if (param_count > 0) {
            param_types = get_param_types(func_dec->ptr[1], param_count);
        }
    }
    
    /* 插入函数符号 */
    SymbolEntry *func_sym = insert_function(symbol_table, func_name, return_type, param_count);
    if (!func_sym) {
        local_semantic_error(node->pos, 0, ERR_REDEFINE_FUNC, func_name);
        free_type(return_type);
        if (param_types) {
            for (int i = 0; i < param_count; i++) free_type(param_types[i]);
            free(param_types);
        }
        return;
    }
    
    /* 设置当前函数 */
    set_current_function(symbol_table, func_sym);
    
    /* 进入函数作用域 */
    enter_scope(symbol_table, func_name);
    
    /* 处理参数 */
    if (func_dec->ptr[1]) {
        check_param_list(func_dec->ptr[1], func_sym);
    }
    
    /* 设置上下文 */
    SemanticContext func_ctx = {0};
    func_ctx.in_function = 1;
    func_ctx.return_type = return_type;
    func_ctx.has_return = 0;
    func_ctx.symbol_table = symbol_table;
    
    /* 检查函数体 */
    if (body && body->kind == COMP_ST) {
        check_compound_stmt(body, &func_ctx);
    }
    
    /* 标记函数已定义 */
    mark_function_defined(symbol_table, func_sym);
    
    /* 检查非void函数是否有return */
    if (return_type->kind == TK_BASIC && return_type->basic != TYPE_VOID && !func_ctx.has_return) {
        local_semantic_error(node->pos, 0, ERR_FUNC_NO_RETURN, func_name);
    }
    
    /* 退出函数作用域 */
    exit_scope(symbol_table);
    
    /* 清理 */
    free_type(return_type);
    if (param_types) {
        for (int i = 0; i < param_count; i++) {
            free_type(param_types[i]);
        }
        free(param_types);
    }
}

/* 获取参数个数 */
static int get_param_count(ASTNode *node) {
    if (!node) return 0;
    
    int count = 0;
    if (node->kind == VAR_LIST) {
        count = 1;
        if (node->ptr[1]) {
            count += get_param_count(node->ptr[1]);
        }
    } else if (node->kind == PARAM_DEC) {
        count = 1;
    }
    
    return count;
}

/* 获取参数类型数组 */
static Type **get_param_types(ASTNode *node, int param_count) {
    if (!node || param_count <= 0) return NULL;
    
    Type **types = (Type **)malloc(param_count * sizeof(Type *));
    if (!types) return NULL;
    
    int index = 0;
    get_param_types_recursive(node, types, &index);
    
    return types;
}

/* 递归获取参数类型 */
static void get_param_types_recursive(ASTNode *node, Type **types, int *index) {
    if (!node || !types || !index) return;
    
    if (node->kind == VAR_LIST) {
        if (node->ptr[0]) get_param_types_recursive(node->ptr[0], types, index);
        if (node->ptr[1]) get_param_types_recursive(node->ptr[1], types, index);
    } else if (node->kind == PARAM_DEC) {
        if (*index < MAX_PARAMS && node->ptr[0]) {
            types[*index] = get_type_from_ast(node->ptr[0]);
            (*index)++;
        }
    }
}

/* 检查参数列表 */
static void check_param_list(ASTNode *node, SymbolEntry *func) {
    if (!node || !func) return;
    
    if (node->kind == VAR_LIST) {
        if (node->ptr[0]) check_param_dec(node->ptr[0], func);
        if (node->ptr[1]) check_param_list(node->ptr[1], func);
    } else if (node->kind == PARAM_DEC) {
        check_param_dec(node, func);
    }
}

/* 检查参数声明 */
static void check_param_dec(ASTNode *node, SymbolEntry *func) {
    if (!node || node->kind != PARAM_DEC) return;
    
    ASTNode *spec = node->ptr[0];
    ASTNode *var_dec = node->ptr[1];
    
    if (!spec || !var_dec) return;
    
    /* 获取参数类型 */
    Type *type = get_type_from_ast(spec);
    
    /* 获取参数名 */
    char param_name[32] = "";
    if (var_dec->kind == ID_NODE) {
        strcpy(param_name, var_dec->type_id);
    } else if (var_dec->kind == ARRAY_DEC && var_dec->ptr[0]) {
        strcpy(param_name, var_dec->ptr[0]->type_id);
    }
    
    /* 添加参数到函数 */
    add_function_parameter(symbol_table, func, param_name, type);
    
    /* 检查变量声明 */
    check_var_dec(var_dec, type, 0);
    
    free_type(type);
}

/* ================== 语句检查 ================== */

/* 检查复合语句 */
static void check_compound_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || node->kind != COMP_ST) return;
    
    /* 进入新的作用域 */
    enter_scope(symbol_table, "block");
    
    /* 检查声明部分 */
    if (node->ptr[0]) {
        check_local_defs(node->ptr[0], ctx);
    }
    
    /* 检查语句部分 */
    if (node->ptr[1]) {
        check_stmt_list(node->ptr[1], ctx);
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
    } else {
        check_var_dec(node, type, is_global);
    }
}

/* 检查语句列表 */
static void check_stmt_list(ASTNode *node, SemanticContext *ctx) {
    if (!node) return;
    
    if (node->kind == STMT_LIST) {
        if (node->ptr[0]) check_stmt(node->ptr[0], ctx);
        if (node->ptr[1]) check_stmt_list(node->ptr[1], ctx);
    }
}

/* 检查语句 */
static void check_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    switch (node->kind) {
        case EXP_STMT:
            if (node->ptr[0]) {
                check_expr(node->ptr[0], ctx);
            }
            break;
            
        case COMP_ST:
            check_compound_stmt(node, ctx);
            break;
            
        case RETURN_STMT:
            check_return_stmt(node, ctx);
            break;
            
        case IF_STMT:
            check_if_stmt(node, ctx);
            break;
            
        case IF_ELSE_STMT:
            check_if_else_stmt(node, ctx);
            break;
            
        case WHILE_STMT:
            check_while_stmt(node, ctx);
            break;
            
        case FOR_STMT:
            check_for_stmt(node, ctx);
            break;
            
        case BREAK_STMT:
            check_break_stmt(node, ctx);
            break;
            
        case CONTINUE_STMT:
            check_continue_stmt(node, ctx);
            break;
            
        default:
            break;
    }
}

/* 检查return语句 */
static void check_return_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    ctx->has_return = 1;
    
    if (node->ptr[0]) {
        Type *expr_type = get_expr_type(node->ptr[0], ctx);
        if (!type_compatible(ctx->return_type, expr_type)) {
            local_semantic_error(node->pos, 0, ERR_RETURN_TYPE, "return type mismatch");
        }
        free_type(expr_type);
    } else if (ctx->return_type->kind == TK_BASIC && ctx->return_type->basic != TYPE_VOID) {
        local_semantic_error(node->pos, 0, ERR_RETURN_TYPE, "void return in non-void function");
    }
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

/* 检查for语句 */
static void check_for_stmt(ASTNode *node, SemanticContext *ctx) {
    if (!node || !ctx) return;
    
    int saved_in_loop = ctx->in_loop;
    ctx->in_loop = 1;
    
    for (int i = 0; i < 4; i++) {
        if (node->ptr[i]) {
            if (i == 3) {
                check_stmt(node->ptr[i], ctx);
            } else {
                check_expr(node->ptr[i], ctx);
            }
        }
    }
    
    ctx->in_loop = saved_in_loop;
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
    
    /* 查找符号 */
    SymbolEntry *sym = lookup_symbol(symbol_table, name);
    if (!sym) {
        local_semantic_error(node->pos, 0, ERR_UNDEFINED_VAR, name);
        node->type_info = new_basic_type(TYPE_INT);  /* 默认类型 */
        node->is_lvalue = 0;
        return;
    }
    
    /* 设置节点信息 */
    node->symbol_ref = sym;
    node->type_info = copy_type(sym->type);
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
    
    /* 左值检查 */
    if (node->ptr[0] && !node->ptr[0]->is_lvalue) {
        local_semantic_error(node->ptr[0]->pos, 0, ERR_NOT_LVALUE, 
                           "left side of assignment must be lvalue");
    }
    
    /* 类型检查 */
    if (node->ptr[0] && node->ptr[0]->type_info && 
        node->ptr[1] && node->ptr[1]->type_info) {
        if (!type_compatible(node->ptr[0]->type_info, node->ptr[1]->type_info)) {
            local_semantic_error(node->pos, 0, ERR_ASSIGN_TYPE, 
                               "type mismatch in assignment");
        }
        node->type_info = copy_type(node->ptr[0]->type_info);
    } else {
        node->type_info = new_basic_type(TYPE_INT);
    }
    node->is_lvalue = 0;  /* 赋值表达式本身不是左值 */
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
    
    /* 查找函数符号 */
    SymbolEntry *func_sym = lookup_symbol(symbol_table, func_name);
    if (!func_sym || func_sym->kind != SK_FUNCTION) {
        local_semantic_error(node->pos, 0, ERR_UNDEFINED_FUNC, func_name);
        node->type_info = new_basic_type(TYPE_INT);
        node->is_lvalue = 0;
        return;
    }
    
    /* 如果有参数，检查参数（暂时简化处理） */
    if (args_node) {
        /* 这里可以添加参数检查逻辑 */
        /* 例如：check_args(args_node, ctx, func_sym); */
    }
    
    /* 设置返回类型 */
    if (func_sym->type && func_sym->type->kind == TK_FUNCTION) {
        node->type_info = copy_type(func_sym->type->func.return_type);
    } else {
        node->type_info = new_basic_type(TYPE_INT);
    }
    node->is_lvalue = 0;
}

/* 检查数组访问 */
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
    if (array_node->type_info && array_node->type_info->kind != TK_ARRAY) {
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
    if (array_node->type_info && array_node->type_info->kind == TK_ARRAY) {
        node->type_info = copy_type(array_node->type_info->array.elem);
    } else {
        node->type_info = new_basic_type(TYPE_INT);
    }
    node->is_lvalue = 1;  /* 数组访问是左值 */
}