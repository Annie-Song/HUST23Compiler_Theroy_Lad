#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 基本数据类型枚举 */
typedef enum {
    TYPE_VOID = 0,
    TYPE_INT = 1,
    TYPE_FLOAT = 2,
    TYPE_CHAR = 3
} BasicType;

/* 类型种类 */
typedef enum {
    TK_BASIC,       // 基本类型
    TK_ARRAY,       // 数组类型
    TK_FUNCTION,    // 函数类型
    TK_STRUCT       // 结构体类型
} TypeKind;

/* 类型结构 */
typedef struct Type {
    TypeKind kind;
    int size;                   // 类型大小（字节）
    int align;                  // 对齐要求
    
    union {
        /* 基本类型 */
        struct {
            BasicType basic;
        };
        
        /* 数组类型 */
        struct {
            struct Type *elem;  // 元素类型
            int length;         // 数组长度
        } array;
        
        /* 函数类型 */
        struct {
            struct Type *return_type;    // 返回类型
            struct Type **param_types;   // 参数类型数组
            int param_count;             // 参数个数
        } func;
        
        /* 结构体类型 */
        struct {
            char *name;         // 结构体名
            struct FieldList *fields;  // 域列表
            int field_count;    // 域数量
        } structure;
    };
} Type;

/* 域列表（用于结构体） */
typedef struct FieldList {
    char *name;         // 域名
    Type *type;         // 域类型
    int offset;         // 偏移量
    struct FieldList *next;
} FieldList;

/* 类型操作函数 */
Type *new_basic_type(BasicType basic);
Type *new_array_type(Type *elem, int length);
Type *new_func_type(Type *return_type, Type **param_types, int param_count);
Type *new_struct_type(const char *name, FieldList *fields);
Type *copy_type(Type *src);
void free_type(Type *type);

/* 类型比较和计算 */
int type_equal(Type *t1, Type *t2);
int get_type_size(Type *type);
int get_type_align(Type *type);
const char *type_to_string(Type *type);

#endif