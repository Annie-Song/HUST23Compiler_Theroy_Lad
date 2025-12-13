#ifndef IR_H
#define IR_H

#include "../semantic/type.h"
#include "../symbol/symbol.h"
#include "../ast/ast.h"  // 添加这行

/* 操作数种类 */
typedef enum {
    OP_CONST,       /* 常量 */
    OP_VAR,         /* 变量 */
    OP_TEMP,        /* 临时变量 */
    OP_LABEL,       /* 标签 */
    OP_FUNC,        /* 函数 */
    OP_NONE         /* 无操作数 */
} OperandKind;

/* 操作数结构体 */
typedef struct Operand {
    OperandKind kind;           /* 操作数种类 */
    union {
        int int_val;           /* 整型常量值 */
        float float_val;       /* 浮点常量值 */
        char *name;            /* 变量名、标签名、函数名 */
    };
    Type *type;                /* 类型信息 */
    int offset;                /* 偏移量（用于栈帧分配） */
} Operand;

/* 中间代码操作码 */
typedef enum {
    IR_LABEL,        /* 标签定义 */
    IR_FUNCTION,     /* 函数定义 */
    IR_ASSIGN,       /* 赋值 */
    IR_ADD,          /* 加法 */
    IR_SUB,          /* 减法 */
    IR_MUL,          /* 乘法 */
    IR_DIV,          /* 除法 */
    IR_LT,           /* 小于 */       // 新增
    IR_LE,           /* 小于等于 */   // 新增
    IR_GT,           /* 大于 */       // 新增
    IR_GE,           /* 大于等于 */   // 新增
    IR_EQ,           /* 等于 */       // 新增
    IR_NE,           /* 不等于 */     // 新增
    IR_GOTO,         /* 无条件跳转 */
    IR_IF,           /* 条件跳转 */
    IR_RETURN,       /* 返回 */
    IR_ARG,          /* 实参 */
    IR_PARAM,        /* 形参 */
    IR_CALL,         /* 函数调用（有返回值） */
    IR_CALL_VOID,    /* 函数调用（无返回值） */
    IR_READ,         /* 读 */
    IR_WRITE         /* 写 */
} IROpCode;

/* 中间代码指令 */
typedef struct IRCode {
    IROpCode op;             /* 操作码 */
    Operand *op1;            /* 操作数1 */
    Operand *op2;            /* 操作数2 */
    Operand *result;         /* 结果操作数 */
    struct IRCode *prev;     /* 前一条指令 */
    struct IRCode *next;     /* 后一条指令 */
} IRCode;

/* 中间代码链表 */
typedef struct IRList {
    IRCode *head;           /* 链表头 */
    IRCode *tail;           /* 链表尾 */
    int count;              /* 指令数量 */
} IRList;

/* ================== 操作数创建函数 ================== */

/* 创建整型常量操作数 */
Operand *new_const_operand_int(int val);

/* 创建浮点常量操作数 */
Operand *new_const_operand_float(float val);

/* 创建变量操作数 */
Operand *new_var_operand(SymbolEntry *sym);

/* 创建临时变量操作数 */
Operand *new_temp_operand(Type *type);

/* 创建标签操作数 */
Operand *new_label_operand(void);

/* ================== 中间代码链表操作 ================== */

/* 创建新的IR链表 */
IRList *new_ir_list(void);

/* 创建新的IR指令 */
IRCode *new_ir_code(IROpCode op, Operand *op1, Operand *op2, Operand *result);

/* 向链表追加指令 */
void append_ir_code(IRList *list, IRCode *code);

/* 合并多个IR链表 */
IRList *merge_ir_lists(int count, ...);

/* ================== 显示函数 ================== */

/* 打印操作数 */
void print_operand(Operand *op);

/* 打印单条IR指令 */
void print_ir_code(IRCode *code);

/* 打印整个IR链表 */
void print_ir_list(IRList *list);

/* ================== 中间代码生成主函数 ================== */

/* 从AST生成中间代码 */
IRList *gen_ir_from_ast(ASTNode *node, SymbolTable *symtab);

/* 生成程序的中间代码 */
void gen_ir_program(ASTNode *node, IRList *ir_list, SymbolTable *symtab);

#endif