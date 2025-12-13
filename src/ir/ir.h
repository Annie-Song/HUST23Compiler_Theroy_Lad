#ifndef IR_H
#define IR_H

#include "../utils/def.h"

/* ================== 中间代码类型定义 ================== */

/* 中间代码操作码 */
typedef enum {
    IR_LABEL,       // 标号定义
    IR_FUNCTION,    // 函数定义
    IR_ASSIGN,      // 赋值
    IR_ADD,         // 加法
    IR_SUB,         // 减法
    IR_MUL,         // 乘法
    IR_DIV,         // 除法
    IR_GOTO,        // 无条件跳转
    IR_IF,          // 条件跳转
    IR_RETURN,      // 返回
    IR_ARG,         // 实参传递
    IR_PARAM,       // 形参定义
    IR_READ,        // 读
    IR_WRITE,       // 写
    IR_CALL,        // 函数调用
    IR_CALL_VOID    // 无返回值函数调用
} IROpCode;

/* 关系运算符 */
typedef enum {
    RELOP_EQ,       // ==
    RELOP_NE,       // !=
    RELOP_LT,       // <
    RELOP_LE,       // <=
    RELOP_GT,       // >
    RELOP_GE        // >=
} RelOp;

/* 操作数类型 */
typedef enum {
    OP_CONST,       // 常量
    OP_VAR,         // 变量/临时变量
    OP_LABEL,       // 标号
    OP_FUNC,        // 函数
    OP_NONE         // 无操作数
} OpKind;

/* 操作数结构 */
typedef struct Operand {
    OpKind kind;
    union {
        int int_val;            // 整型常量
        float float_val;        // 浮点常量
        char *name;             // 变量/标号/函数名
    };
    struct Type *type;          // 操作数类型
    int offset;                 // 在活动记录中的偏移
} Operand;

/* 中间代码指令（四元式） */
typedef struct IRCode {
    IROpCode op;
    Operand *op1;
    Operand *op2;
    Operand *result;
    struct IRCode *prev;
    struct IRCode *next;
} IRCode;

/* 中间代码链表 */
typedef struct IRList {
    IRCode *head;
    IRCode *tail;
    int count;
} IRList;

/* ================== 中间代码生成函数 ================== */

/* 创建操作数 */
Operand *new_const_operand_int(int val);
Operand *new_const_operand_float(float val);
Operand *new_var_operand(SymbolEntry *sym);
Operand *new_temp_operand(Type *type);
Operand *new_label_operand(void);

/* 创建中间代码 */
IRCode *new_ir_code(IROpCode op, Operand *op1, Operand *op2, Operand *result);
void append_ir_code(IRList *list, IRCode *code);
IRList *new_ir_list(void);
void free_ir_list(IRList *list);

/* 代码序列合并 */
IRList *merge_ir_lists(int count, ...);

/* 生成中间代码 */
IRList *gen_ir_from_ast(ASTNode *node);
void gen_ir_program(ASTNode *node, IRList *ir_list);

/* 显示中间代码 */
void print_ir_code(IRCode *code);
void print_ir_list(IRList *list);
void print_operand(Operand *op);

#endif