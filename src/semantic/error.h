#ifndef ERROR_H
#define ERROR_H

#include "../utils/def.h"

#define MAX_ERRORS 100

// 错误信息结构
typedef struct SemanticError{
    int line;
    int column;
    ErrorType type;
    char message[256];
} SemanticError;

// 错误收集器
typedef struct ErrorCollector{
    SemanticError errors[MAX_ERRORS];
    int count;
} ErrorCollector;

// 函数声明
void init_error_collector(void);
void add_semantic_error(int line, int col, ErrorType type, const char *fmt, ...);
void semantic_error(int line, int col, ErrorType err_type, const char *msg);
void print_all_errors(void);
int has_errors(void);
int get_error_count(void);
void clear_all_errors(void);

#endif
