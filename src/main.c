#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/def.h"
#include "ast/ast.h"
#include "semantic/semantic.h"
#include "semantic/error.h"  // 添加这个头文件，包含 has_errors 声明
#include "ir/ir.h"
#include "mips/mips.h"

// 条件编译开关
#define SHOW_AST 1

// 声明全局根节点
extern ASTNode *root;
extern SymbolTable *symbol_table;

// 简化编译阶段标志
#define ENABLE_PHASE1  1
#define ENABLE_PHASE2  1
#define ENABLE_PHASE3  1
#define ENABLE_PHASE4  1

// 输出文件名
#define OUTPUT_MIPS_FILE "output.s"

// 函数声明
void cleanup_resources(IRList *ir_list, MipsCodeList *mips_list);
void free_ir_list(IRList *ir_list);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    
    printf("=== Mini-C++ Compiler (Phase 4) ===\n");
    printf("Source: %s\n\n", argv[1]);
    
    // 设置输入文件
    extern FILE *yyin;
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }
    
    // 进行语法分析
    extern int yyparse();
    printf("[PHASE 1] Parsing...\n");
    
    int result = yyparse();
    fclose(yyin);
    
    // 资源清理标志
    IRList *ir_list = NULL;
    MipsCodeList *mips_list = NULL;
    
    if (result != 0 || !root) {
        printf("\n✗ Parsing failed!\n");
        return 1;
    }
    
    printf("✓ Parsing completed successfully!\n");
    
    // 显示抽象语法树
    #if SHOW_AST
        printf("\n=== Abstract Syntax Tree ===\n");
        if (root) {
            display(root, 0);
        }
    #endif

    // ==================== 阶段二：语义分析 ====================
    #if ENABLE_PHASE2
        printf("\n=== Phase 2: Semantic Analysis ===\n");
        semantic_analysis(root);
        
        if (has_errors()) {
            printf("✗ Semantic analysis failed.\n");
            cleanup_resources(NULL, NULL);
            return 1;
        } else {
            printf("✓ Semantic analysis passed.\n");
        }
    #endif

    // ==================== 阶段三：中间代码生成 ====================
    #if ENABLE_PHASE3
        printf("\n=== Phase 3: Intermediate Code Generation ===\n");
        
        if (!symbol_table) {
            printf("[WARNING] symbol_table is NULL\n");
            // 创建临时符号表
            symbol_table = create_symbol_table();
            if (symbol_table) {
                init_symbol_table(symbol_table);
            }
        }
        
        if (symbol_table) {
            ir_list = gen_ir_from_ast(root, symbol_table);
            
            if (ir_list && ir_list->head) {
                print_ir_list(ir_list);
                printf("✓ Generated %d IR instructions.\n", ir_list->count);
                
                // ==================== 阶段四：目标代码生成 ====================
                #if ENABLE_PHASE4
                    printf("\n=== Phase 4: Target Code Generation ===\n");
                    
                    mips_list = gen_mips_from_ir(ir_list, symbol_table);
                    
                    if (mips_list && mips_list->head) {
                        print_mips_code(mips_list);
                        printf("✓ Generated %d MIPS instructions.\n", mips_list->count);
                        
                        save_mips_to_file(mips_list, OUTPUT_MIPS_FILE);
                        printf("✓ MIPS code saved to '%s'\n", OUTPUT_MIPS_FILE);
                        
                        printf("\n=== Testing Instructions ===\n");
                        printf("To run with MARS or QtSpim:\n");
                        printf("  1. Open %s in MARS/QtSpim\n", OUTPUT_MIPS_FILE);
                        printf("  2. Assemble and run\n");
                        printf("\nFor SPIM on Linux:\n");
                        printf("  spim -f %s\n", OUTPUT_MIPS_FILE);
                    } else {
                        printf("✗ Failed to generate MIPS code.\n");
                    }
                #else
                    printf("Phase 4 (Target Code Generation) is disabled.\n");
                #endif
                
            } else {
                printf("✗ No intermediate code generated.\n");
            }
        } else {
            printf("✗ Symbol table is NULL\n");
        }
    #endif

    // 清理资源
    cleanup_resources(ir_list, mips_list);
    
    // 清理符号表
    if (symbol_table) {
        destroy_symbol_table(symbol_table);
        symbol_table = NULL;
    }
    
    printf("\n=== Compilation finished ===\n");
    return 0;
}

/* ==================== 辅助函数 ==================== */

// 清理IR列表
void free_ir_list(IRList *ir_list) {
    if (!ir_list) return;
    
    IRCode *current = ir_list->head;
    while (current) {
        IRCode *next = current->next;
        
        // 安全地释放操作数
        if (current->op1) {
            if (current->op1->kind == OP_VAR || 
                current->op1->kind == OP_LABEL || 
                current->op1->kind == OP_FUNC) {
                if (current->op1->name) {
                    free(current->op1->name);
                }
            }
            // 不要释放类型，它们可能是共享的
            free(current->op1);
        }
        
        if (current->op2) {
            if (current->op2->kind == OP_VAR || 
                current->op2->kind == OP_LABEL || 
                current->op2->kind == OP_FUNC) {
                if (current->op2->name) {
                    free(current->op2->name);
                }
            }
            free(current->op2);
        }
        
        if (current->result) {
            if (current->result->kind == OP_VAR || 
                current->result->kind == OP_LABEL || 
                current->result->kind == OP_FUNC) {
                if (current->result->name) {
                    free(current->result->name);
                }
            }
            free(current->result);
        }
        
        free(current);
        current = next;
    }
    
    free(ir_list);
}

// 清理动态分配的资源
void cleanup_resources(IRList *ir_list, MipsCodeList *mips_list) {
    // 清理IR列表
    if (ir_list) {
        free_ir_list(ir_list);
    }
    
    // 清理MIPS列表
    if (mips_list) {
        MipsCode *current = mips_list->head;
        while (current) {
            MipsCode *next = current->next;
            if (current->instruction) {
                free(current->instruction);
            }
            free(current);
            current = next;
        }
        free(mips_list);
    }
}