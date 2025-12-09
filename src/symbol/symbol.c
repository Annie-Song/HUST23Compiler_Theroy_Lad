#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* PJW哈希函数 */
static unsigned int hash_pjw(const char *name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if ((i = val & ~0x3fff) != 0)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val % 211;
}

/* 创建符号表 */
SymbolTable *create_symbol_table(void) {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!table) return NULL;
    
    /* 初始化哈希表 */
    for (int i = 0; i < 211; i++) {
        table->hash_table[i] = NULL;
    }
    table->hash_size = 211;
    
    /* 初始化作用域栈 */
    for (int i = 0; i < 32; i++) {
        table->scope_stack[i] = NULL;
    }
    table->scope_top = -1;
    table->current_level = 0;
    
    /* 初始化计数器 */
    table->temp_var_count = 0;
    table->label_count = 0;
    table->error_count = 0;
    
    /* 初始化状态 */
    table->current_func = NULL;
    table->in_loop = 0;
    table->in_switch = 0;
    
    return table;
}

/* 初始化符号表 */
void init_symbol_table(SymbolTable *table) {
    if (!table) return;
    
    /* 进入全局作用域 */
    enter_scope(table, "global");
    
    /* 插入预定义函数 */
    Type *int_type = new_basic_type(TYPE_INT);
    Type *void_type = new_basic_type(TYPE_VOID);
    
    /* read() -> int */
    insert_function(table, "read", int_type, 0);
    
    /* write(int) -> void */
    Type *write_params[1] = {int_type};
    Type *write_type = new_func_type(void_type, write_params, 1);
    SymbolEntry *write_func = insert_symbol(table, "write", SK_FUNCTION, write_type);
    if (write_func) {
        write_func->func_info.defined = 1;
    }
}

/* 销毁符号表 */
void destroy_symbol_table(SymbolTable *table) {
    if (!table) return;
    
    /* 释放所有符号 */
    for (int i = 0; i < table->hash_size; i++) {
        SymbolEntry *entry = table->hash_table[i];
        while (entry) {
            SymbolEntry *next = entry->next_hash;
            
            /* 释放类型 */
            if (entry->type) {
                free_type(entry->type);
            }
            
            /* 释放数组维度信息 */
            if (entry->kind == SK_ARRAY && entry->array_info.dim_sizes) {
                free(entry->array_info.dim_sizes);
            }
            
            free(entry);
            entry = next;
        }
    }
    
    /* 释放作用域栈 */
    for (int i = 0; i <= table->scope_top; i++) {
        if (table->scope_stack[i]) {
            free(table->scope_stack[i]);
        }
    }
    
    free(table);
}

/* 进入新作用域 */
void enter_scope(SymbolTable *table, const char *name) {
    if (!table) return;
    
    table->current_level++;
    table->scope_top++;
    
    /* 创建新的作用域头节点 */
    SymbolEntry *scope_head = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    if (scope_head) {
        strncpy(scope_head->name, name ? name : "", 31);
        scope_head->name[31] = '\0';
        scope_head->kind = SK_LABEL;
        scope_head->level = table->current_level;
        scope_head->next_scope = NULL;
        table->scope_stack[table->scope_top] = scope_head;
    }
}

/* 退出作用域 */
void exit_scope(SymbolTable *table) {
    if (!table || table->scope_top < 0) return;
    
    /* 获取当前作用域头节点 */
    SymbolEntry *scope_head = table->scope_stack[table->scope_top];
    
    /* 遍历作用域中的所有符号，从哈希表中移除 */
    SymbolEntry *entry = scope_head->next_scope;
    while (entry) {
        unsigned int idx = hash_pjw(entry->name);
        SymbolEntry **head = &table->hash_table[idx];
        SymbolEntry *current = *head;
        SymbolEntry *prev = NULL;
        
        while (current) {
            if (current == entry) {
                if (prev) {
                    prev->next_hash = current->next_hash;
                } else {
                    *head = current->next_hash;
                }
                break;
            }
            prev = current;
            current = current->next_hash;
        }
        
        entry = entry->next_scope;
    }
    
    /* 释放作用域头节点 */
    free(scope_head);
    table->scope_stack[table->scope_top] = NULL;
    table->scope_top--;
    table->current_level--;
}

/* 获取当前作用域层级 */
int get_current_level(SymbolTable *table) {
    return table ? table->current_level : 0;
}

/* 查找符号（从当前作用域向上查找） */
SymbolEntry *lookup_symbol(SymbolTable *table, const char *name) {
    if (!table || !name) return NULL;
    
    unsigned int idx = hash_pjw(name);
    SymbolEntry *entry = table->hash_table[idx];
    
    /* 在哈希链中查找 */
    while (entry) {
        if (strcmp(entry->name, name) == 0 && entry->level <= table->current_level) {
            return entry;
        }
        entry = entry->next_hash;
    }
    
    return NULL;
}

/* 只在当前作用域查找 */
SymbolEntry *lookup_current_scope(SymbolTable *table, const char *name) {
    if (!table || !name || table->scope_top < 0) return NULL;
    
    SymbolEntry *scope_head = table->scope_stack[table->scope_top];
    SymbolEntry *entry = scope_head->next_scope;
    
    while (entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry;
        }
        entry = entry->next_scope;
    }
    
    return NULL;
}

/* 全局查找 */
SymbolEntry *lookup_global(SymbolTable *table, const char *name) {
    if (!table || !name) return NULL;
    
    unsigned int idx = hash_pjw(name);
    SymbolEntry *entry = table->hash_table[idx];
    
    while (entry) {
        if (strcmp(entry->name, name) == 0 && entry->level == 0) {
            return entry;
        }
        entry = entry->next_hash;
    }
    
    return NULL;
}

/* 检查是否重定义 */
int check_redefinition(SymbolTable *table, const char *name) {
    return lookup_current_scope(table, name) != NULL;
}

/* 插入符号到符号表 */
SymbolEntry *insert_symbol(SymbolTable *table, const char *name, 
                          SymbolKind kind, Type *type) {
    if (!table || !name) return NULL;  /* 注意：type可以为NULL（如标签）*/
    
    /* 检查重定义 */
    if (check_redefinition(table, name)) {
        table->error_count++;
        return NULL;
    }
    
    /* 创建新符号 */
    SymbolEntry *entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    if (!entry) return NULL;
    
    /* 初始化符号 */
    strncpy(entry->name, name, 31);
    entry->name[31] = '\0';
    
    /* 生成别名 */
    if (kind == SK_TEMP) {
        snprintf(entry->alias, 32, "t%d", ++table->temp_var_count);
    } else if (kind == SK_LABEL) {
        snprintf(entry->alias, 32, "L%d", ++table->label_count);
    } else {
        strncpy(entry->alias, name, 31);
        entry->alias[31] = '\0';
    }
    
    entry->kind = kind;
    entry->type = type ? copy_type(type) : NULL;  /* 修改：复制类型 */
    entry->level = table->current_level;
    entry->offset = 0;
    entry->size = type ? get_type_size(type) : 0;
    entry->is_const = 0;
    entry->is_ref = 0;
    
    /* 初始化额外信息 */
    memset(&entry->const_val, 0, sizeof(entry->const_val));
    memset(&entry->func_info, 0, sizeof(entry->func_info));
    memset(&entry->array_info, 0, sizeof(entry->array_info));
    
    /* 插入哈希表 */
    unsigned int idx = hash_pjw(name);
    entry->next_hash = table->hash_table[idx];
    if (table->hash_table[idx]) {
        table->hash_table[idx]->prev_hash = entry;
    }
    table->hash_table[idx] = entry;
    entry->prev_hash = NULL;
    
    /* 添加到当前作用域链 */
    if (table->scope_top >= 0) {
        SymbolEntry *scope_head = table->scope_stack[table->scope_top];
        entry->next_scope = scope_head->next_scope;
        scope_head->next_scope = entry;
    }
    
    return entry;
}

/* 插入变量 */
SymbolEntry *insert_variable(SymbolTable *table, const char *name, 
                            Type *type, int is_const) {
    SymbolEntry *entry = insert_symbol(table, name, 
                                      is_const ? SK_CONSTANT : SK_VARIABLE, type);
    if (entry) {
        entry->is_const = is_const;
        if (is_const) {
            /* 设置默认值 */
            if (type->kind == TK_BASIC) {
                switch (type->basic) {
                    case TYPE_INT: entry->const_val.int_val = 0; break;
                    case TYPE_FLOAT: entry->const_val.float_val = 0.0f; break;
                    case TYPE_CHAR: entry->const_val.char_val = '\0'; break;
                    default: break;
                }
            }
        }
    }
    return entry;
}

/* 插入数组 */
SymbolEntry *insert_array(SymbolTable *table, const char *name,
                         Type *elem_type, int dims, int *dim_sizes) {
    if (!elem_type || dims <= 0 || !dim_sizes) return NULL;
    
    /* 创建数组类型 */
    Type *array_type = new_array_type(elem_type, dim_sizes[0]);
    for (int i = 1; i < dims; i++) {
        Type *temp = array_type;
        array_type = new_array_type(temp, dim_sizes[i]);
        free_type(temp);
    }
    
    SymbolEntry *entry = insert_symbol(table, name, SK_ARRAY, array_type);
    if (entry) {
        entry->array_info.dims = dims;
        entry->array_info.dim_sizes = (int *)malloc(dims * sizeof(int));
        memcpy(entry->array_info.dim_sizes, dim_sizes, dims * sizeof(int));
        
        /* 计算总大小 */
        int total_size = get_type_size(elem_type);
        for (int i = 0; i < dims; i++) {
            total_size *= dim_sizes[i];
        }
        entry->array_info.total_size = total_size;
        entry->size = total_size;
    }
    
    return entry;
}

/* 插入函数 */
SymbolEntry *insert_function(SymbolTable *table, const char *name,
                           Type *return_type, int param_count) {
    /* 创建函数类型 */
    Type *func_type = new_func_type(return_type, NULL, param_count);
    SymbolEntry *entry = insert_symbol(table, name, SK_FUNCTION, func_type);
    
    if (entry) {
        entry->func_info.param_count = param_count;
        entry->func_info.defined = 0;
        entry->func_info.local_var_size = 0;
        entry->func_info.max_temp_size = 0;
        entry->func_info.has_return = 0;
    }
    
    return entry;
}

/* 插入参数 */
SymbolEntry *insert_parameter(SymbolTable *table, const char *name,
                            Type *type) {
    SymbolEntry *entry = insert_symbol(table, name, SK_PARAMETER, type);
    if (entry) {
        entry->is_ref = 1;  /* 参数是引用传递 */
    }
    return entry;
}

/* 创建临时变量 */
SymbolEntry *new_temp_var(SymbolTable *table, Type *type) {
    char temp_name[32];
    snprintf(temp_name, 32, "_temp%d", ++table->temp_var_count);
    return insert_symbol(table, temp_name, SK_TEMP, type);
}

/* 创建标号 */
SymbolEntry *new_label(SymbolTable *table) {
    char label_name[32];
    snprintf(label_name, 32, "_label%d", ++table->label_count);
    return insert_symbol(table, label_name, SK_LABEL, NULL);
}

/* 设置当前函数 */
void set_current_function(SymbolTable *table, SymbolEntry *func) {
    if (!table) return;
    table->current_func = func;
}

/* 获取当前函数 */
SymbolEntry *get_current_function(SymbolTable *table) {
    return table ? table->current_func : NULL;
}

/* 添加函数参数 */
void add_function_parameter(SymbolTable *table, SymbolEntry *func,
                           const char *param_name, Type *param_type) {
    if (!table || !func || !param_name || !param_type) return;
    if (func->kind != SK_FUNCTION) return;
    
    /* 创建参数符号 */
    SymbolEntry *param = insert_parameter(table, param_name, param_type);
    if (!param) return;
    
    /* 更新函数类型的参数信息 */
    if (!func->type->func.param_types) {
        func->type->func.param_types = (Type **)malloc(func->func_info.param_count * sizeof(Type *));
    }
    
    int param_index = 0;
    /* 查找参数插入位置 */
    SymbolEntry *entry = func->next_scope;
    while (entry) {
        if (entry->kind == SK_PARAMETER && entry == param) {
            func->type->func.param_types[param_index] = param_type;  /* 不复制，使用原指针 */
            break;
        }
        if (entry->kind == SK_PARAMETER) {
            param_index++;
        }
        entry = entry->next_scope;
    }
}

/* 标记函数已定义 */
void mark_function_defined(SymbolTable *table, SymbolEntry *func) {
    if (!table || !func || func->kind != SK_FUNCTION) return;
    func->func_info.defined = 1;
}

/* 打印符号表 */
void print_symbol_table(SymbolTable *table) {
    if (!table) {
        printf("Symbol table is NULL\n");
        return;
    }
    
    printf("\n=== Symbol Table ===\n");
    printf("Current Level: %d\n", table->current_level);
    printf("Errors: %d\n", table->error_count);
    printf("Temp Variables: %d\n", table->temp_var_count);
    printf("Labels: %d\n", table->label_count);
    printf("\n");
    
    printf("%-5s %-20s %-12s %-15s %-6s %-8s %-6s\n",
           "Level", "Name", "Kind", "Type", "Size", "Offset", "Const");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < table->hash_size; i++) {
        SymbolEntry *entry = table->hash_table[i];
        while (entry) {
            printf("%-5d %-20s ", entry->level, entry->name);
            
            /* 种类 */
            switch (entry->kind) {
                case SK_VARIABLE: printf("%-12s ", "variable"); break;
                case SK_FUNCTION: printf("%-12s ", "function"); break;
                case SK_PARAMETER: printf("%-12s ", "parameter"); break;
                case SK_ARRAY: printf("%-12s ", "array"); break;
                case SK_CONSTANT: printf("%-12s ", "constant"); break;
                case SK_TEMP: printf("%-12s ", "temp"); break;
                case SK_LABEL: printf("%-12s ", "label"); break;
                default: printf("%-12s ", "unknown");
            }
            
            /* 类型 - 安全处理 */
            if (entry->type) {
                const char *type_str = type_to_string(entry->type);
                printf("%-15s ", type_str);
            } else {
                printf("%-15s ", "null");
            }
            
            /* 其他信息 */
            printf("%-6d %-8d %-6s\n",
                   entry->size, entry->offset,
                   entry->is_const ? "yes" : "no");
            
            entry = entry->next_hash;
        }
    }
    printf("=== Total symbols in hash table ===\n\n");
}

/* 打印单个符号 */
void print_symbol(SymbolEntry *entry) {
    if (!entry) return;
    
    printf("  %s [%s] ", entry->name, entry->alias);
    
    switch (entry->kind) {
        case SK_VARIABLE: printf("variable"); break;
        case SK_FUNCTION: printf("function"); break;
        case SK_PARAMETER: printf("parameter"); break;
        case SK_ARRAY: printf("array"); break;
        case SK_CONSTANT: printf("constant"); break;
        case SK_TEMP: printf("temp"); break;
        default: printf("unknown");
    }
    
    printf(" type:%s", entry->type ? type_to_string(entry->type) : "none");
    printf(" level:%d size:%d offset:%d",
           entry->level, entry->size, entry->offset);
    
    if (entry->is_const) printf(" const");
    if (entry->is_ref) printf(" ref");
    
    printf("\n");
}