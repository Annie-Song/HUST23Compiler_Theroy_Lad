#include "type.h"
#include <stdlib.h>
#include <string.h>

/* 创建基本类型 */
Type *new_basic_type(BasicType basic) {
    Type *type = (Type *)malloc(sizeof(Type));
    if (!type) return NULL;
    
    type->kind = TK_BASIC;
    type->basic = basic;
    
    /* 设置大小和对齐 */
    switch (basic) {
        case TYPE_INT:
            type->size = 4;
            type->align = 4;
            break;
        case TYPE_FLOAT:
            type->size = 4;
            type->align = 4;
            break;
        case TYPE_CHAR:
            type->size = 1;
            type->align = 1;
            break;
        case TYPE_VOID:
            type->size = 0;
            type->align = 0;
            break;
    }
    
    return type;
}

/* 创建数组类型 */
Type *new_array_type(Type *elem, int length) {
    if (!elem) return NULL;
    
    Type *type = (Type *)malloc(sizeof(Type));
    if (!type) return NULL;
    
    type->kind = TK_ARRAY;
    type->array.elem = elem;
    type->array.length = length;
    
    /* 数组大小 = 元素大小 × 长度 */
    type->size = elem->size * length;
    type->align = elem->align;
    
    return type;
}

/* 创建函数类型 */
Type *new_func_type(Type *return_type, Type **param_types, int param_count) {
    Type *type = (Type *)malloc(sizeof(Type));
    if (!type) return NULL;
    
    type->kind = TK_FUNCTION;
    type->func.return_type = return_type;
    type->func.param_types = param_types;
    type->func.param_count = param_count;
    type->size = 0;        /* 函数类型没有大小 */
    type->align = 0;
    
    return type;
}

/* 创建结构体类型 */
Type *new_struct_type(const char *name, FieldList *fields) {
    Type *type = (Type *)malloc(sizeof(Type));
    if (!type) return NULL;
    
    type->kind = TK_STRUCT;
    type->structure.name = name ? strdup(name) : NULL;
    type->structure.fields = fields;
    
    /* 计算结构体大小和对齐 */
    int size = 0;
    int align = 1;
    FieldList *field = fields;
    
    while (field) {
        if (field->type) {
            /* 对齐当前字段 */
            int field_align = field->type->align;
            if (align < field_align) align = field_align;
            
            /* 计算偏移量 */
            if (size % field_align != 0) {
                size += field_align - (size % field_align);
            }
            field->offset = size;
            size += field->type->size;
            
            /* 统计字段数量 */
            type->structure.field_count++;
        }
        field = field->next;
    }
    
    /* 最终结构体大小需要对齐 */
    if (size % align != 0) {
        size += align - (size % align);
    }
    
    type->size = size;
    type->align = align;
    
    return type;
}

/* 复制类型 */
Type *copy_type(Type *src) {
    if (!src) return NULL;
    
    Type *dst = (Type *)malloc(sizeof(Type));
    if (!dst) return NULL;
    
    memcpy(dst, src, sizeof(Type));
    
    /* 深度复制需要递归处理的部分 */
    if (src->kind == TK_ARRAY && src->array.elem) {
        dst->array.elem = copy_type(src->array.elem);
    } else if (src->kind == TK_FUNCTION) {
        /* 函数类型需要深度复制参数类型 */
        if (src->func.param_types && src->func.param_count > 0) {
            dst->func.param_types = (Type **)malloc(src->func.param_count * sizeof(Type *));
            for (int i = 0; i < src->func.param_count; i++) {
                dst->func.param_types[i] = copy_type(src->func.param_types[i]);
            }
        }
        dst->func.return_type = copy_type(src->func.return_type);
    }
    
    return dst;
}

/* 释放类型 */
void free_type(Type *type) {
    if (!type) return;
    
    if (type->kind == TK_ARRAY) {
        free_type(type->array.elem);
    } else if (type->kind == TK_FUNCTION) {
        free_type(type->func.return_type);
        if (type->func.param_types) {
            for (int i = 0; i < type->func.param_count; i++) {
                free_type(type->func.param_types[i]);
            }
            free(type->func.param_types);
        }
    } else if (type->kind == TK_STRUCT) {
        if (type->structure.name) free(type->structure.name);
        /* 释放域列表 */
        FieldList *field = type->structure.fields;
        while (field) {
            FieldList *next = field->next;
            if (field->name) free(field->name);
            free_type(field->type);
            free(field);
            field = next;
        }
    }
    
    free(type);
}

/* 类型相等检查 */
int type_equal(Type *t1, Type *t2) {
    if (!t1 || !t2) return t1 == t2;
    if (t1->kind != t2->kind) return 0;
    
    switch (t1->kind) {
        case TK_BASIC:
            return t1->basic == t2->basic;
            
        case TK_ARRAY:
            if (t1->array.length != t2->array.length) return 0;
            return type_equal(t1->array.elem, t2->array.elem);
            
        case TK_FUNCTION:
            if (!type_equal(t1->func.return_type, t2->func.return_type))
                return 0;
            if (t1->func.param_count != t2->func.param_count)
                return 0;
            for (int i = 0; i < t1->func.param_count; i++) {
                if (!type_equal(t1->func.param_types[i], t2->func.param_types[i]))
                    return 0;
            }
            return 1;
            
        case TK_STRUCT:
            /* 结构体按名比较 */
            return t1->structure.name && t2->structure.name &&
                   strcmp(t1->structure.name, t2->structure.name) == 0;
    }
    
    return 0;
}

static int type_compatible_internal(Type *t1, Type *t2) {
    if (!t1 || !t2) return 0;
    return type_equal(t1, t2);
}

/* 获取类型大小 */
int get_type_size(Type *type) {
    return type ? type->size : 0;
}

/* 获取类型对齐要求 */
int get_type_align(Type *type) {
    return type ? type->align : 1;
}

/* 类型转字符串 */
const char *type_to_string(Type *type) {
    static char buffer[256];
    
    if (!type) return "null";
    
    switch (type->kind) {
        case TK_BASIC:
            switch (type->basic) {
                case TYPE_VOID: return "void";
                case TYPE_INT: return "int";
                case TYPE_FLOAT: return "float";
                case TYPE_CHAR: return "char";
            }
            break;
            
        case TK_ARRAY: {
            const char *elem_str = type_to_string(type->array.elem);
            snprintf(buffer, sizeof(buffer), "%s[%d]", elem_str, type->array.length);
            return buffer;
        }
            
        case TK_FUNCTION: {
            buffer[0] = '\0';
            strcat(buffer, "func(");
            for (int i = 0; i < type->func.param_count; i++) {
                if (i > 0) strcat(buffer, ", ");
                strcat(buffer, type_to_string(type->func.param_types[i]));
            }
            strcat(buffer, "):");
            strcat(buffer, type_to_string(type->func.return_type));
            return buffer;
        }
            
        case TK_STRUCT:
            if (type->structure.name) {
                snprintf(buffer, sizeof(buffer), "struct %s", type->structure.name);
            } else {
                strcpy(buffer, "struct");
            }
            return buffer;
    }
    
    return "unknown";
}