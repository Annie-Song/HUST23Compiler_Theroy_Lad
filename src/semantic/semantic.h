#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../utils/def.h"
#include "../ast/ast.h"

/* 语义分析上下文结构体定义在.c文件中，不需要在这里暴露 */

/* 主要接口函数 - 只暴露这个 */
void semantic_analysis(ASTNode *root);

/* 错误处理函数 - 需要在def.h中声明，但这里也需要供其他文件使用 */
void semantic_error(int line, int col, ErrorType err_type, const char *msg);

/* 全局符号表声明 - 添加这行 */
extern SymbolTable *symbol_table;

#endif