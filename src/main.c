#include <stdio.h>
#include <stdlib.h>
#include "utils/def.h"
#include "ast/ast.h"

// 条件编译开关：是否显示AST
#define SHOW_AST 1

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    
    printf("=== Mini-C++ Compiler (Phase 1: Lexical & Syntax Analysis) ===\n");
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
        
        // 条件编译：显示抽象语法树
        #if SHOW_AST
            printf("\n=== Abstract Syntax Tree ===\n");
            // AST根节点通过全局变量传递，这里需要根据实际实现调整
        #endif
    } else {
        printf("\n✗ Parsing failed!\n");
    }
    
    return result;
}