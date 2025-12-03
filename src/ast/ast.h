#ifndef AST_H
#define AST_H

#include "../utils/def.h"

// 抽象语法树结点类型定义（需要和parser.y中的token对应）
enum NodeKind {
    // 程序结构
    EXT_DEF_LIST,
    EXT_VAR_DEF,
    FUNC_DEF,
    FUNC_DEC,
    
    // 类型节点（加后缀_NODE）
    TYPE_NODE_INT,      // 原 TYPE_INT
    TYPE_NODE_FLOAT,    // 原 TYPE_FLOAT  
    TYPE_NODE_CHAR,     // 原 TYPE_CHAR
    TYPE_NODE_VOID,     // 原 TYPE_VOID
    
    // 声明
    EXT_DEC_LIST,
    ARRAY_DEC,
    ID_NODE,            // 原 ID
    PARAM_DEC,
    VAR_LIST,
    
    // 语句
    COMP_ST,
    DEF_LIST,
    DEF,
    DEC_LIST,
    INIT_DEC,
    STMT_LIST,
    EXP_STMT,
    RETURN_STMT,
    IF_STMT,
    IF_ELSE_STMT,
    WHILE_STMT,
    FOR_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    
    // 表达式
    ASSIGN_EXP,
    BINARY_EXP,
    UNARY_EXP,
    POST_INC_EXP,
    POST_DEC_EXP,
    FUNC_CALL,
    ARGS,
    ARRAY_ACCESS,
    
    // 常量节点（加后缀_NODE）
    INT_NODE,           // 原 INT
    FLOAT_NODE,         // 原 FLOAT
    CHAR_NODE,          // 原 CHAR
    STRING_NODE         // 原 STRING
};

// 抽象语法树操作函数
ASTNode *mknode(int num, int kind, int pos, ...);
void display(ASTNode *T, int indent);
void free_ast(ASTNode *T);

// 辅助函数
const char *get_node_name(int kind);

#endif