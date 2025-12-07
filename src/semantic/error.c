#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "error.h"

/* 全局错误收集器 */
ErrorCollector error_collector;

/* 错误类型描述 */
static const char* get_error_type_string(ErrorType type) {
    switch (type) {
        case ERR_UNDEFINED_VAR: return "undefined variable";
        case ERR_UNDEFINED_FUNC: return "undefined function";
        case ERR_REDEFINE_VAR: return "redefined variable";
        case ERR_REDEFINE_FUNC: return "redefined function";
        case ERR_TYPE_MISMATCH: return "type mismatch";
        case ERR_ARRAY_SUBSCRIPT: return "array subscript not integer";
        case ERR_NOT_ARRAY: return "subscript on non-array";
        case ERR_PARAM_COUNT: return "parameter count mismatch";
        case ERR_PARAM_TYPE: return "parameter type mismatch";
        case ERR_RETURN_TYPE: return "return type mismatch";
        case ERR_BREAK_OUT_LOOP: return "break outside loop";
        case ERR_CONTINUE_OUT_LOOP: return "continue outside loop";
        case ERR_NOT_LVALUE: return "not an lvalue";
        case ERR_ARRAY_INIT: return "array initializer mismatch";
        case ERR_STRUCT_MEMBER: return "no such struct member";
        case ERR_ASSIGN_TYPE: return "assignment type mismatch";
        case ERR_CONDITION_TYPE: return "condition must be boolean";
        case ERR_DIVIDE_ZERO: return "division by zero";
        case ERR_ARRAY_DIM_MISMATCH: return "array dimension mismatch";
        case ERR_FUNC_NO_RETURN: return "function missing return";
        default: return "unknown error";
    }
}

/* 初始化错误收集器 */
void init_error_collector(void) {
    error_collector.count = 0;
}

/* 添加语义错误 */
void add_semantic_error(int line, int col, ErrorType type, const char *fmt, ...) {
    if (error_collector.count >= MAX_ERRORS) return;
    
    SemanticError *err = &error_collector.errors[error_collector.count];
    err->line = line;
    err->column = col;
    err->type = type;
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(err->message, sizeof(err->message), fmt, args);
    va_end(args);
    
    error_collector.count++;
}

/* 语义错误包装函数 */
void semantic_error(int line, int col, ErrorType err_type, const char *msg) {
    add_semantic_error(line, col, err_type, "%s", msg);
}

/* 打印所有错误 */
void print_all_errors(void) {
    if (error_collector.count == 0) {
        printf("No semantic errors found.\n");
        return;
    }
    
    printf("\n=== Semantic Errors (%d found) ===\n", error_collector.count);
    for (int i = 0; i < error_collector.count; i++) {
        SemanticError *err = &error_collector.errors[i];
        printf("Error %d at line %d, col %d: %s: %s\n", 
               i + 1, err->line, err->column, 
               get_error_type_string(err->type), err->message);
    }
}

/* 检查是否有错误 */
int has_errors(void) {
    return error_collector.count > 0;
}

/* 获取错误数量 */
int get_error_count(void) {
    return error_collector.count;
}

/* 清除所有错误 */
void clear_all_errors(void) {
    error_collector.count = 0;
}