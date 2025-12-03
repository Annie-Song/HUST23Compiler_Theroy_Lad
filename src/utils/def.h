#ifndef DEF_H
#define DEF_H

typedef union {
    int type_int;
    float type_float;
    char type_id[32];
    struct ASTNode *ptr;
} YYSTYPE;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define MAXLENGTH 200
#define DX 3 * sizeof(int)  // 活动记录控制信息需要的单元数

// 抽象语法树结点类型定义
typedef struct ASTNode {
    int kind;                    // 结点类型
    union {
        char type_id[32];        // 标识符名字
        int type_int;            // 整型常量值
        float type_float;        // 浮点常量值
    };
    struct ASTNode *ptr[4];      // 子树指针，最多4棵子树
    int place;                   // 存放（临时）变量在符号表的位置序号
    char Etrue[15], Efalse[15]; // 布尔表达式真假转移目标
    char Snext[15];              // 结点对应语句执行后的下一条语句位置
    struct CodeNode *code;       // 该结点中间代码链表头指针
    int type;                    // 表达式类型
    int pos;                     // 语法单位所在位置行号
    int offset;                  // 偏移量
    int width;                   // 占数据字节数
    int num;                     // 计数器，可用于统计形参个数
} ASTNode;

// 符号表项定义
typedef struct Symbol {
    char name[33];              // 名字
    int level;                  // 层号
    int type;                   // 类型
    int paramnum;               // 对函数：形参个数
    char alias[10];             // 别名
    char flag;                  // 符号标记：函数'F' 变量'V' 参数'P' 临时变量'T'
    int offset;                 // 偏移量
} Symbol;

// 符号表
typedef struct {
    Symbol symbols[MAXLENGTH];
    int index;
} SymbolTable;

// 作用域栈
typedef struct {
    int TX[30];
    int top;
} SymbolScopeTX;

// 三地址码定义
typedef struct Opn {
    int kind;                   // 操作数类型
    int type;                   // 数据类型
    union {
        int const_int;          // 整型常量
        float const_float;      // 浮点常量
        char const_char;        // 字符常量
        char id[33];            // 变量/临时变量别名或标号
    };
    int level;                  // 层号
    int offset;                 // 偏移量
} Opn;

typedef struct CodeNode {
    int op;                     // 操作码
    Opn opn1, opn2, result;    // 操作数和结果
    struct CodeNode *next, *prior;
} CodeNode;

// 抽象语法树创建函数
ASTNode *mknode(int num, int kind, int pos, ...);

// 显示抽象语法树函数
void display(ASTNode *T, int indent);

// 错误处理函数
void semantic_error(int line, char *msg1, char *msg2);

#endif