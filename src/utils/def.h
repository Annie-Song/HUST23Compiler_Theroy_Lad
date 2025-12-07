#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ================== 常量定义 ================== */
#define MAX_SYMBOL_NAME 32
#define MAX_STRING_LEN 256
#define MAX_PARAMS 20
#define MAX_ERRORS 100
#define MAX_SCOPE_DEPTH 32
#define SYMBOL_TABLE_SIZE 211

/* 错误类型枚举 */
typedef enum {
    ERR_UNDEFINED_VAR = 1,
    ERR_UNDEFINED_FUNC,
    ERR_REDEFINE_VAR,
    ERR_REDEFINE_FUNC,
    ERR_TYPE_MISMATCH,
    ERR_ARRAY_SUBSCRIPT,
    ERR_NOT_ARRAY,
    ERR_PARAM_COUNT,
    ERR_PARAM_TYPE,
    ERR_RETURN_TYPE,
    ERR_BREAK_OUT_LOOP,
    ERR_CONTINUE_OUT_LOOP,
    ERR_NOT_LVALUE,
    ERR_ARRAY_INIT,
    ERR_STRUCT_MEMBER,
    ERR_ASSIGN_TYPE,
    ERR_CONDITION_TYPE,
    ERR_DIVIDE_ZERO,
    ERR_ARRAY_DIM_MISMATCH,
    ERR_FUNC_NO_RETURN
} ErrorType;

/* YYSTYPE 定义 */
typedef union {
    int type_int;
    float type_float;
    char type_id[MAX_SYMBOL_NAME];
    struct ASTNode *ptr;
} YYSTYPE;

/* ================== 前向声明 ================== */
typedef struct ASTNode ASTNode;
typedef struct Type Type;
typedef struct SymbolEntry SymbolEntry;
typedef struct SymbolTable SymbolTable;
typedef struct CodeNode CodeNode;
typedef struct SemanticError SemanticError;
typedef struct ErrorCollector ErrorCollector;

/* 抽象语法树结点类型定义 */
struct ASTNode {
    int kind;                    // 结点类型
    int pos;                     // 语法单位所在位置行号
    
    union {
        char type_id[MAX_SYMBOL_NAME];        // 标识符名字
        int type_int;            // 整型常量值
        float type_float;        // 浮点常量值
    };
    
    /* 阶段二新增字段 - 语义分析 */
    SymbolEntry *symbol_ref;     // 指向符号表中的项
    Type *type_info;             // 表达式类型信息
    int is_lvalue;               // 是否为左值
    int is_const;                // 是否为常量
    
    /* 代码生成字段 */
    struct ASTNode *ptr[4];      // 子树指针，最多4棵子树
    int place;                   // 存放（临时）变量在符号表的位置序号
    char Etrue[15], Efalse[15]; // 布尔表达式真假转移目标
    char Snext[15];              // 结点对应语句执行后的下一条语句位置
    struct CodeNode *code;       // 该结点中间代码链表头指针
    int type;                    // 表达式类型（兼容旧的type字段）
    int offset;                  // 偏移量
    int width;                   // 占数据字节数
    int num;                     // 计数器，可用于统计形参个数
    
    /* 数组/函数额外信息 */
    union {
        struct {
            int array_size;      // 数组大小
            Type *elem_type;     // 数组元素类型
        } array_info;
        struct {
            int param_count;     // 参数个数
            Type **param_types;  // 参数类型数组
        } func_info;
    };
};

/* 抽象语法树操作函数 */
ASTNode *mknode(int num, int kind, int pos, ...);
void display(ASTNode *T, int indent);
void free_ast(ASTNode *T);

/* 语义错误处理 */
void semantic_error(int line, int col, ErrorType err_type, const char *msg);

#endif