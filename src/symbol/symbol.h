#ifndef SYMBOL_H
#define SYMBOL_H

#include "../semantic/type.h"

/* 符号种类 */
typedef enum {
    SK_VARIABLE,    // 变量
    SK_FUNCTION,    // 函数
    SK_PARAMETER,   // 参数
    SK_ARRAY,       // 数组
    SK_CONSTANT,    // 常量
    SK_TEMP,        // 临时变量
    SK_LABEL        // 标号
} SymbolKind;

/* 符号表项 */
typedef struct SymbolEntry {
    char name[32];              // 符号名
    char alias[32];             // 别名（用于中间代码）
    SymbolKind kind;            // 符号种类
    Type *type;                 // 符号类型
    int level;                  // 作用域层级
    int offset;                 // 在活动记录中的偏移
    int size;                   // 占用字节数
    int is_const;               // 是否为常量
    int is_ref;                 // 是否为引用
    
    /* 额外信息 */
    union {
        /* 常量值 */
        struct {
            union {
                int int_val;
                float float_val;
                char char_val;
                char *str_val;
            };
        } const_val;
        
        /* 函数信息 */
        struct {
            int defined;            // 是否已定义
            int param_count;        // 参数个数
            int local_var_size;     // 局部变量总大小
            int max_temp_size;      // 最大临时变量大小
            int has_return;         // 是否有return语句
        } func_info;
        
        /* 数组信息 */
        struct {
            int total_size;         // 数组总大小
            int *dim_sizes;         // 维度大小数组
            int dims;               // 维度数
        } array_info;
    };
    
    /* 链表指针 */
    struct SymbolEntry *next_hash;  // 哈希链
    struct SymbolEntry *prev_hash;  // 哈希链前驱
    struct SymbolEntry *next_scope; // 作用域链
} SymbolEntry;

/* 符号表 */
typedef struct SymbolTable {
    /* 哈希表 */
    SymbolEntry *hash_table[211];   // 哈希表
    int hash_size;                  // 哈希表大小
    
    /* 作用域管理 */
    SymbolEntry *scope_stack[32];   // 作用域栈
    int scope_top;                  // 作用域栈顶
    int current_level;              // 当前作用域层级
    
    /* 计数器 */
    int temp_var_count;             // 临时变量计数器
    int label_count;                // 标号计数器
    int error_count;                // 错误计数器
    
    /* 当前函数信息 */
    SymbolEntry *current_func;      // 当前处理的函数
    int in_loop;                    // 是否在循环中
    int in_switch;                  // 是否在switch中
} SymbolTable;

/* ========== 符号表操作函数 ========== */

/* 创建和销毁 */
SymbolTable *create_symbol_table(void);
void destroy_symbol_table(SymbolTable *table);
void init_symbol_table(SymbolTable *table);

/* 作用域管理 */
void enter_scope(SymbolTable *table, const char *name);
void exit_scope(SymbolTable *table);
int get_current_level(SymbolTable *table);

/* 符号查找 */
SymbolEntry *lookup_symbol(SymbolTable *table, const char *name);
SymbolEntry *lookup_current_scope(SymbolTable *table, const char *name);
SymbolEntry *lookup_global(SymbolTable *table, const char *name);
int check_redefinition(SymbolTable *table, const char *name);

/* 符号插入 */
SymbolEntry *insert_symbol(SymbolTable *table, const char *name, 
                          SymbolKind kind, Type *type);
SymbolEntry *insert_variable(SymbolTable *table, const char *name, 
                            Type *type, int is_const);
SymbolEntry *insert_array(SymbolTable *table, const char *name,
                         Type *elem_type, int dims, int *dim_sizes);
SymbolEntry *insert_function(SymbolTable *table, const char *name,
                           Type *return_type, int param_count);
SymbolEntry *insert_parameter(SymbolTable *table, const char *name,
                            Type *type);

/* 特殊符号创建 */
SymbolEntry *new_temp_var(SymbolTable *table, Type *type);
SymbolEntry *new_label(SymbolTable *table);

/* 函数相关 */
void set_current_function(SymbolTable *table, SymbolEntry *func);
SymbolEntry *get_current_function(SymbolTable *table);
void add_function_parameter(SymbolTable *table, SymbolEntry *func,
                           const char *param_name, Type *param_type);
void mark_function_defined(SymbolTable *table, SymbolEntry *func);

/* 调试和输出 */
void print_symbol_table(SymbolTable *table);
void print_symbol(SymbolEntry *entry);

#endif