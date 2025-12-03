#include "ast.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>  // 添加这个头文件

//辅助函数：根据结点类型返回结点名称字符串
const char *get_node_name(int kind) {
    switch (kind) {
        case EXT_DEF_LIST: return "EXT_DEF_LIST";
        case EXT_VAR_DEF: return "EXT_VAR_DEF";
        case FUNC_DEF: return "FUNC_DEF";
        case FUNC_DEC: return "FUNC_DEC";
        case TYPE_NODE_INT: return "TYPE_INT";
        case TYPE_NODE_FLOAT: return "TYPE_FLOAT";
        case TYPE_NODE_CHAR: return "TYPE_CHAR";
        case TYPE_NODE_VOID: return "TYPE_VOID";
        case EXT_DEC_LIST: return "EXT_DEC_LIST";
        case ARRAY_DEC: return "ARRAY_DEC";
        case ID_NODE: return "ID";
        case PARAM_DEC: return "PARAM_DEC";
        case VAR_LIST: return "VAR_LIST";
        case COMP_ST: return "COMP_ST";
        case DEF_LIST: return "DEF_LIST";
        case DEF: return "DEF";
        case DEC_LIST: return "DEC_LIST";
        case INIT_DEC: return "INIT_DEC";
        case STMT_LIST: return "STMT_LIST";
        case EXP_STMT: return "EXP_STMT";
        case RETURN_STMT: return "RETURN_STMT";
        case IF_STMT: return "IF_STMT";
        case IF_ELSE_STMT: return "IF_ELSE_STMT";
        case WHILE_STMT: return "WHILE_STMT";
        case FOR_STMT: return "FOR_STMT";
        case BREAK_STMT: return "BREAK_STMT";
        case CONTINUE_STMT: return "CONTINUE_STMT";
        case ASSIGN_EXP: return "ASSIGN_EXP";
        case BINARY_EXP: return "BINARY_EXP";
        case UNARY_EXP: return "UNARY_EXP";
        case POST_INC_EXP: return "POST_INC_EXP";
        case POST_DEC_EXP: return "POST_DEC_EXP";
        case FUNC_CALL: return "FUNC_CALL";
        case ARGS: return "ARGS";
        case ARRAY_ACCESS: return "ARRAY_ACCESS";
        case INT_NODE: return "INT";
        case FLOAT_NODE: return "FLOAT";
        case CHAR_NODE: return "CHAR";
        case STRING_NODE: return "STRING";
        default: return "UNKNOWN_NODE";
    }
}

ASTNode *mknode(int num, int kind, int pos, ...) {
    ASTNode *T = (ASTNode *)malloc(sizeof(ASTNode));
    if (!T) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    T->kind = kind;
    T->pos = pos;
    T->ptr[0] = T->ptr[1] = T->ptr[2] = T->ptr[3] = NULL;
    T->type = 0;
    T->place = 0;
    T->Etrue[0] = T->Efalse[0] = T->Snext[0] = '\0';
    T->code = NULL;
    T->offset = 0;
    T->width = 0;
    T->num = 0;
    
    va_list args;
    va_start(args, pos);
    
    for (int i = 0; i < num; i++) {
        T->ptr[i] = va_arg(args, ASTNode *);
    }
    
    va_end(args);
    return T;
}

void display(ASTNode *T, int indent) {
    if (!T) return;
    
    // 缩进
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    // 显示行号信息
    printf("[Line %d] ", T->pos);

    // 根据结点类型显示不同信息
    switch (T->kind) {
        case EXT_DEF_LIST:
            printf("External Definition List\n");
            break;
            
        case EXT_VAR_DEF:
            printf("External Variable Definition\n");
            break;
            
        case FUNC_DEF:
            printf("Function Definition\n");
            break;
            
        case FUNC_DEC:
            printf("Function Declaration: %s\n", T->ptr[0] ? T->ptr[0]->type_id : "NULL");
            break;
            
        case TYPE_NODE_INT:
            printf("Type: int\n");
            break;
            
        case TYPE_NODE_FLOAT:
            printf("Type: float\n");
            break;
            
        case TYPE_NODE_CHAR:
            printf("Type: char\n");
            break;
            
        case TYPE_NODE_VOID:
            printf("Type: void\n");
            break;
            
        case ID_NODE:
            printf("Identifier: %s\n", T->type_id);
            break;
            
        case INT_NODE:
            printf("Integer Constant: %d\n", T->type_int);
            break;
            
        case FLOAT_NODE:
            printf("Float Constant: %f\n", T->type_float);
            break;
            
        case CHAR_NODE:
            printf("Char Constant: %c\n", (char)T->type_int);
            break;
            
        case STRING_NODE:  // 添加 STRING 的处理
            printf("String Constant: %s\n", T->type_id);
            break;
            
        case ASSIGN_EXP:
            printf("Assignment: %s\n", T->type_id);
            display(T->ptr[0], indent + 2);
            display(T->ptr[1], indent + 2);
            break;
            
        case BINARY_EXP:
            printf("Binary Expression: %s\n", T->type_id);
            display(T->ptr[0], indent + 2);
            display(T->ptr[1], indent + 2);
            break;
            
        case UNARY_EXP:
            printf("Unary Expression: %s\n", T->type_id);
            display(T->ptr[0], indent + 2);
            break;
            
        case POST_INC_EXP:  // 添加后置++处理
        case POST_DEC_EXP:  // 添加后置--处理
            printf("Postfix Expression: %s\n", T->type_id);
            display(T->ptr[0], indent + 2);
            break;
            
        case FUNC_CALL:
            printf("Function Call: %s\n", T->ptr[0] ? T->ptr[0]->type_id : "NULL");
            if (T->ptr[1]) display(T->ptr[1], indent + 2);
            break;
            
        case ARRAY_ACCESS:
            printf("Array Access\n");
            display(T->ptr[0], indent + 2);  // 数组名
            display(T->ptr[1], indent + 2);  // 下标表达式
            break;
            
        case IF_STMT:
            printf("If Statement\n");
            printf("  Condition:\n");
            display(T->ptr[0], indent + 4);
            printf("  Then:\n");
            display(T->ptr[1], indent + 4);
            break;
            
        case IF_ELSE_STMT:
            printf("If-Else Statement\n");
            printf("  Condition:\n");
            display(T->ptr[0], indent + 4);
            printf("  Then:\n");
            display(T->ptr[1], indent + 4);
            printf("  Else:\n");
            display(T->ptr[2], indent + 4);
            break;
            
        case WHILE_STMT:
            printf("While Statement\n");
            printf("  Condition:\n");
            display(T->ptr[0], indent + 4);
            printf("  Body:\n");
            display(T->ptr[1], indent + 4);
            break;
            
        case FOR_STMT:
            printf("For Statement\n");
            printf("  Init:\n");
            display(T->ptr[0], indent + 4);
            printf("  Condition:\n");
            display(T->ptr[1], indent + 4);
            printf("  Update:\n");
            display(T->ptr[2], indent + 4);
            printf("  Body:\n");
            display(T->ptr[3], indent + 4);
            break;
            
        case BREAK_STMT:
            printf("Break Statement\n");
            break;
            
        case CONTINUE_STMT:
            printf("Continue Statement\n");
            break;
            
        case RETURN_STMT:
            printf("Return Statement\n");
            if (T->ptr[0]) display(T->ptr[0], indent + 2);
            break;
            
        case COMP_ST:
            printf("Compound Statement\n");
            if (T->ptr[0]) {
                printf("  Declarations:\n");
                display(T->ptr[0], indent + 4);
            }
            if (T->ptr[1]) {
                printf("  Statements:\n");
                display(T->ptr[1], indent + 4);
            }
            break;
            
        // 添加其他节点的处理
        case EXT_DEC_LIST:
            printf("External Declaration List\n");
            break;
            
        case ARRAY_DEC:
            printf("Array Declaration\n");
            break;
            
        case PARAM_DEC:
            printf("Parameter Declaration\n");
            break;
            
        case VAR_LIST:
            printf("Variable List\n");
            break;
            
        case DEF_LIST:
            printf("Definition List\n");
            break;
            
        case DEF:
            printf("Definition\n");
            break;
            
        case DEC_LIST:
            printf("Declaration List\n");
            break;
            
        case INIT_DEC:
            printf("Initialized Declaration\n");
            break;
            
        case STMT_LIST:
            printf("Statement List\n");
            break;
            
        case EXP_STMT:
            printf("Expression Statement\n");
            if (T->ptr[0]) display(T->ptr[0], indent + 2);
            break;
            
        case ARGS:
            printf("Arguments\n");
            break;
            
        default:
            printf("Unknown Node (kind=%d)\n", T->kind);
    }
    
    // 递归显示子树 - 简化条件判断
    for (int i = 0; i < 4; i++) {
        if (T->ptr[i]) {
            // 如果当前节点已经在上面处理过了，就不再递归处理
            int handled_in_switch = 0;
            switch (T->kind) {
                case ASSIGN_EXP:
                case BINARY_EXP:
                case UNARY_EXP:
                case POST_INC_EXP:
                case POST_DEC_EXP:
                case IF_STMT:
                case IF_ELSE_STMT:
                case WHILE_STMT:
                case FOR_STMT:
                case RETURN_STMT:
                case COMP_ST:
                case FUNC_CALL:
                case ARRAY_ACCESS:
                case EXP_STMT:
                    handled_in_switch = 1;
                    break;
            }
            if (!handled_in_switch) {
                display(T->ptr[i], indent + 2);
            }
        }
    }
}

void free_ast(ASTNode *T) {
    if (!T) return;
    
    for (int i = 0; i < 4; i++) {
        free_ast(T->ptr[i]);
    }
    
    if (T->code) {
        free(T->code);
    }
    
    free(T);
}