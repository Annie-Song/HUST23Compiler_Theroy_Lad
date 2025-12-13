#include <stdio.h>
#include <stdlib.h>
#include "utils/def.h"
#include "ast/ast.h"
#include "semantic/semantic.h"
#include "ir/ir.h"  // 添加IR头文件

// 条件编译开关
#define SHOW_AST 1

// 声明全局根节点（在parser.y中定义）
extern ASTNode *root;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    
    printf("=== Mini-C++ Compiler ===\n");
    printf("File: %s\n\n", argv[1]);
    
    // 设置输入文件
    extern FILE *yyin;
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }
    
    // 进行语法分析
    extern int yyparse();
    printf("Starting parsing...\n");
    
    int result = yyparse();
    
    fclose(yyin);
    
    if (result == 0) {
        printf("\n✓ Parsing completed successfully!\n");
        
        // 显示抽象语法树
        #if SHOW_AST
            printf("\n=== Abstract Syntax Tree ===\n");
             if (root) {
                display(root, 0);
            }
        #endif

        // ==================== 阶段二：语义分析 ====================
        #ifdef PHASE2
            printf("\n=== Phase 2: Semantic Analysis ===\n");
            semantic_analysis(root);
            
            // 如果语义分析有错误，停止后续阶段
            if (has_errors()) {
                printf("\n✗ Semantic analysis failed. Stopping.\n");
                return 1;
            }
        #endif

        // ==================== 阶段三：中间代码生成 ====================
        #ifdef PHASE3
            printf("\n=== Phase 3: Intermediate Code Generation ===\n");
            
            if (root) {
                // 生成中间代码
                IRList *ir_list = gen_ir_from_ast(root);
                
                if (ir_list && ir_list->head) {
                    // 显示中间代码
                    print_ir_list(ir_list);
                    
                    // 保存中间代码到文件（可选）
                    // FILE *ir_file = fopen("output.ir", "w");
                    // if (ir_file) {
                    //     // 遍历并写入文件
                    //     IRCode *current = ir_list->head;
                    //     while (current) {
                    //         // 这里需要实现写入函数
                    //         current = current->next;
                    //     }
                    //     fclose(ir_file);
                    // }
                } else {
                    printf("No intermediate code generated.\n");
                }
            } else {
                printf("AST is empty, cannot generate intermediate code.\n");
            }
        #endif

        // ==================== 阶段四：目标代码生成 ====================
        #ifdef PHASE4
            printf("\n=== Phase 4: Target Code Generation ===\n");
            // 这里将添加MIPS代码生成
        #endif

    } else {
        printf("\n✗ Parsing failed!\n");
    }
    
    return result;
}