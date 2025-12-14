CC = gcc
CFLAGS = -Wall -g -I$(SRC_DIR)
FLEX = flex
BISON = bison

PROJECT_DIR = /home/annie/compiler-lab
SRC_DIR = $(PROJECT_DIR)/src
OBJ_DIR = $(PROJECT_DIR)/build
BIN_DIR = $(PROJECT_DIR)/bin

# 各阶段编译选项
PHASE1_FLAGS = 
PHASE2_FLAGS = -DPHASE2 -DSHOW_SYMBOL_TABLE
PHASE3_FLAGS = -DPHASE2 -DPHASE3 -DINTERMEDIATE_CODE
PHASE4_FLAGS = -DPHASE2 -DPHASE3 -DPHASE4 -DTARGET_CODE

# 默认使用阶段四
CURRENT_PHASE_FLAGS = $(PHASE4_FLAGS)

TARGET = $(BIN_DIR)/minicpp

# ================== 源文件列表 ==================
# IR源文件
IR_SRC = $(SRC_DIR)/ir/ir.c

# MIPS源文件
MIPS_SRC = $(SRC_DIR)/mips/mips.c

# 现有源文件
AST_SRC = $(SRC_DIR)/ast/ast.c
LEX_SRC = $(OBJ_DIR)/lex.yy.c
PARSE_SRC = $(OBJ_DIR)/parser.tab.c
SEMANTIC_SRC = $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/semantic/error.c $(SRC_DIR)/semantic/type.c
SYMBOL_SRC = $(SRC_DIR)/symbol/symbol.c
UTILS_SRC = $(SRC_DIR)/utils/utils.c
MAIN_SRC = $(SRC_DIR)/main.c

# ================== 目标文件列表 ==================
# IR和MIPS目标文件
IR_OBJ = $(OBJ_DIR)/ir/ir.o
MIPS_OBJ = $(OBJ_DIR)/mips/mips.o

# 现有目标文件
AST_OBJ = $(OBJ_DIR)/ast/ast.o
LEX_OBJ = $(OBJ_DIR)/lex.yy.o
PARSE_OBJ = $(OBJ_DIR)/parser.tab.o
SEMANTIC_OBJS = $(OBJ_DIR)/semantic/semantic.o $(OBJ_DIR)/semantic/error.o $(OBJ_DIR)/semantic/type.o
SYMBOL_OBJ = $(OBJ_DIR)/symbol/symbol.o
UTILS_OBJ = $(OBJ_DIR)/utils/utils.o
MAIN_OBJ = $(OBJ_DIR)/main.o

# 阶段一目标文件（无IR/MIPS）
OBJS_PHASE1 = $(MAIN_OBJ) $(AST_OBJ) $(LEX_OBJ) $(PARSE_OBJ) $(SEMANTIC_OBJS) $(SYMBOL_OBJ) $(UTILS_OBJ)

# 阶段三目标文件（有IR）
OBJS_PHASE3 = $(OBJS_PHASE1) $(IR_OBJ)

# 阶段四目标文件（有IR和MIPS）
OBJS_PHASE4 = $(OBJS_PHASE3) $(MIPS_OBJ)

# 默认使用阶段四目标文件
OBJS = $(OBJS_PHASE4)

# ================== 目录创建 ==================
DIRS = $(OBJ_DIR) $(BIN_DIR) \
       $(OBJ_DIR)/ast $(OBJ_DIR)/semantic $(OBJ_DIR)/symbol \
       $(OBJ_DIR)/utils $(OBJ_DIR)/ir $(OBJ_DIR)/mips

# 创建所需目录
$(shell mkdir -p $(DIRS))

# ================== 主目标 ==================
all: phase4
	@echo "All phases built successfully!"

# 主目标链接
$(TARGET): $(OBJS)
	@echo "Linking compiler..."
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -o $@ $^
	@echo "Compiler built: $(TARGET)"

# ================== 各阶段的编译规则 ==================
phase1: CURRENT_PHASE_FLAGS = $(PHASE1_FLAGS)
phase1: OBJS = $(OBJS_PHASE1)
phase1: $(TARGET)
	@echo "Phase 1 (Lex & Parse) built successfully!"

phase2: CURRENT_PHASE_FLAGS = $(PHASE2_FLAGS)
phase2: OBJS = $(OBJS_PHASE1)
phase2: $(TARGET)
	@echo "Phase 2 (Semantic Analysis) built successfully!"

phase3: CURRENT_PHASE_FLAGS = $(PHASE3_FLAGS)
phase3: OBJS = $(OBJS_PHASE3)
phase3: $(TARGET)
	@echo "Phase 3 (Intermediate Code) built successfully!"

phase4: CURRENT_PHASE_FLAGS = $(PHASE4_FLAGS)
phase4: OBJS = $(OBJS_PHASE4)
phase4: $(TARGET)
	@echo "Phase 4 (Target Code) built successfully!"

# ================== 各源文件的编译规则 ==================
# 主程序
$(MAIN_OBJ): $(MAIN_SRC) \
             $(SRC_DIR)/utils/def.h $(SRC_DIR)/ast/ast.h \
             $(SRC_DIR)/semantic/semantic.h $(SRC_DIR)/semantic/error.h \
             $(SRC_DIR)/ir/ir.h $(SRC_DIR)/mips/mips.h
	@echo "Compiling main program..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# AST
$(AST_OBJ): $(AST_SRC) $(SRC_DIR)/ast/ast.h
	@echo "Compiling AST..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# 语义分析
$(OBJ_DIR)/semantic/semantic.o: $(SRC_DIR)/semantic/semantic.c \
                                 $(SRC_DIR)/semantic/semantic.h \
                                 $(SRC_DIR)/semantic/type.h \
                                 $(SRC_DIR)/semantic/error.h
	@echo "Compiling semantic analysis..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/error.o: $(SRC_DIR)/semantic/error.c \
                             $(SRC_DIR)/semantic/error.h
	@echo "Compiling error handling..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/type.o: $(SRC_DIR)/semantic/type.c \
                           $(SRC_DIR)/semantic/type.h
	@echo "Compiling type system..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# 符号表
$(SYMBOL_OBJ): $(SYMBOL_SRC) $(SRC_DIR)/symbol/symbol.h
	@echo "Compiling symbol table..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# 工具函数
$(UTILS_OBJ): $(UTILS_SRC) $(SRC_DIR)/utils/def.h
	@echo "Compiling utils..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# ================== 中间代码生成 ==================
$(IR_OBJ): $(IR_SRC) $(SRC_DIR)/ir/ir.h \
           $(SRC_DIR)/ast/ast.h $(SRC_DIR)/symbol/symbol.h \
           $(SRC_DIR)/semantic/type.h
	@echo "Compiling intermediate code generator..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# ================== 目标代码生成 ==================
$(MIPS_OBJ): $(MIPS_SRC) $(SRC_DIR)/mips/mips.h \
             $(SRC_DIR)/ir/ir.h $(SRC_DIR)/symbol/symbol.h
	@echo "Compiling MIPS code generator..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# ================== 词法和语法分析器生成 ==================
# 词法分析器
$(LEX_OBJ): $(LEX_SRC) $(OBJ_DIR)/parser.tab.h
	@echo "Compiling lexer..."
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# 语法分析器
$(PARSE_OBJ): $(PARSE_SRC) $(OBJ_DIR)/parser.tab.h
	@echo "Compiling parser..."
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# 生成语法分析器
$(PARSE_SRC) $(OBJ_DIR)/parser.tab.h: $(SRC_DIR)/parser/parser.y
	@echo "Generating parser..."
	cd $(OBJ_DIR) && $(BISON) -d -v -o parser.tab.c $(SRC_DIR)/parser/parser.y

# 生成词法分析器
$(LEX_SRC): $(SRC_DIR)/lexer/lex.l $(OBJ_DIR)/parser.tab.h
	@echo "Generating lexer..."
	cd $(OBJ_DIR) && $(FLEX) -o lex.yy.c $(SRC_DIR)/lexer/lex.l

# ================== 清理和目标 ==================
# 清理所有
clean:
	@echo "Cleaning up all build files..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) parser.output *.ir *.s output.s
	@echo "Clean complete."

# 清理中间代码和目标代码生成器
clean-phase34:
	@echo "Cleaning phase 3/4 files..."
	rm -rf $(OBJ_DIR)/ir $(OBJ_DIR)/mips 
	rm -f *.ir *.s output.s
	@echo "Phase 3/4 clean complete."

# 清理中间代码文件
clean-ir:
	@echo "Cleaning IR files..."
	rm -f *.ir
	@echo "IR files cleaned."

# 清理目标代码文件
clean-mips:
	@echo "Cleaning MIPS files..."
	rm -f $(OBJ_DIR)/mips/*.o
	rm -f *.s output.s
	@echo "MIPS files cleaned."

# 清理分析器生成的文件
clean-parser:
	@echo "Cleaning parser files..."
	rm -f $(OBJ_DIR)/lex.yy.c $(OBJ_DIR)/lex.yy.o
	rm -f $(OBJ_DIR)/parser.tab.c $(OBJ_DIR)/parser.tab.h $(OBJ_DIR)/parser.tab.o
	rm -f parser.output
	@echo "Parser files cleaned."

# ================== 测试目标 ==================
# 测试所有阶段
test-all:
	@echo "Testing all phases..."
	@echo "=== Phase 1 ==="
	@$(MAKE) phase1 >/dev/null 2>&1
	@$(TARGET) test/test1.c 2>&1 | grep -A5 "Parsing" || true
	@echo ""
	@echo "=== Phase 2 ==="
	@$(MAKE) phase2 >/dev/null 2>&1
	@$(TARGET) test/test1.c 2>&1 | grep -A5 "Semantic" || true
	@echo ""
	@echo "=== Phase 3 ==="
	@$(MAKE) phase3 >/dev/null 2>&1
	@$(TARGET) test/test1.c 2>&1 | grep -A5 "Intermediate" || true
	@echo ""
	@echo "=== Phase 4 ==="
	@$(MAKE) phase4 >/dev/null 2>&1
	@$(TARGET) test/test1.c 2>&1 | grep -A5 "Target" || true

# 测试阶段三
test-phase3: phase3
	@echo "Testing Phase 3 (Intermediate Code)..."
	@$(TARGET) test/test1.c 2>&1 | grep -A 20 "=== Intermediate Code ===" || true

# 测试阶段四
test-phase4: phase4
	@echo "Testing Phase 4 (MIPS Code)..."
	@$(TARGET) test/test1.c 2>&1 | grep -A 30 "=== MIPS Assembly Code ===" || true
	@if [ -f output.s ]; then \
		echo ""; \
		echo "MIPS code saved to output.s"; \
		echo "First 15 lines:"; \
		head -15 output.s; \
	else \
		echo "No output.s generated"; \
	fi

# 快速测试（使用阶段四）
test: phase4
	@echo "Running test with Phase 4..."
	@$(TARGET) test/test1.c 2>&1
	@if [ -f output.s ]; then \
		echo ""; \
		echo "Check output.s for MIPS assembly"; \
		echo "First 10 lines:"; \
		head -10 output.s; \
	else \
		echo "No MIPS output generated"; \
	fi

# 创建测试目录和示例文件
create-test:
	@echo "Creating test directory and sample test file..."
	@mkdir -p test
	@echo "Creating test/test1.c..."
	@echo 'int main() {' > test/test1.c
	@echo '    int a = 10;' >> test/test1.c
	@echo '    int b = 20;' >> test/test1.c
	@echo '    int c = a + b;' >> test/test1.c
	@echo '    return c;' >> test/test1.c
	@echo '}' >> test/test1.c
	@echo "Test file created: test/test1.c"

# ================== 调试目标 ==================
debug: CFLAGS += -DDEBUG -O0 -g3
debug: phase4
	@echo "Debug version built with full debug symbols"

# 只调试阶段三
debug-phase3: CFLAGS += -DDEBUG -O0 -g3
debug-phase3: phase3
	@echo "Phase 3 debug version built"

# 只调试阶段四
debug-phase4: CFLAGS += -DDEBUG -O0 -g3
debug-phase4: phase4
	@echo "Phase 4 debug version built"

# ================== 发布目标 ==================
release: CFLAGS = -Wall -O2
release: phase4
	@echo "Release version built with optimizations"

# ================== 文件检查 ==================
check-files:
	@echo "Checking required files..."
	@echo ""
	@echo "MIPS source: $(MIPS_SRC)"
	@if [ -f $(MIPS_SRC) ]; then \
		echo "✓ MIPS source file exists"; \
	else \
		echo "✗ MIPS source file missing: $(MIPS_SRC)"; \
		echo "Creating minimal mips.c..."; \
		mkdir -p $(SRC_DIR)/mips; \
		echo '#include "mips.h"' > $(MIPS_SRC); \
		echo '#include <stdio.h>' >> $(MIPS_SRC); \
		echo '#include <stdlib.h>' >> $(MIPS_SRC); \
		echo '#include <string.h>' >> $(MIPS_SRC); \
		echo '// MIPS code generator' >> $(MIPS_SRC); \
		echo 'MipsCodeList *gen_mips_from_ir(IRList *ir_list, SymbolTable *symtab) {' >> $(MIPS_SRC); \
		echo '    printf("MIPS generation not yet implemented\\n");' >> $(MIPS_SRC); \
		echo '    return NULL;' >> $(MIPS_SRC); \
		echo '}' >> $(MIPS_SRC); \
		echo 'void print_mips_code(MipsCodeList *list) {}' >> $(MIPS_SRC); \
		echo 'void save_mips_to_file(MipsCodeList *list, const char *filename) {}' >> $(MIPS_SRC); \
	fi
	@echo ""
	@echo "MIPS header: $(SRC_DIR)/mips/mips.h"
	@if [ -f $(SRC_DIR)/mips/mips.h ]; then \
		echo "✓ MIPS header file exists"; \
	else \
		echo "✗ MIPS header file missing"; \
		echo "Creating minimal mips.h..."; \
		mkdir -p $(SRC_DIR)/mips; \
		echo '#ifndef MIPS_H' > $(SRC_DIR)/mips/mips.h; \
		echo '#define MIPS_H' >> $(SRC_DIR)/mips/mips.h; \
		echo '#include "../ir/ir.h"' >> $(SRC_DIR)/mips/mips.h); \
		echo 'typedef struct MipsCodeList MipsCodeList;' >> $(SRC_DIR)/mips/mips.h; \
		echo 'MipsCodeList *gen_mips_from_ir(IRList *ir_list, SymbolTable *symtab);' >> $(SRC_DIR)/mips/mips.h; \
		echo 'void print_mips_code(MipsCodeList *list);' >> $(SRC_DIR)/mips/mips.h; \
		echo 'void save_mips_to_file(MipsCodeList *list, const char *filename);' >> $(SRC_DIR)/mips/mips.h; \
		echo '#endif' >> $(SRC_DIR)/mips/mips.h; \
	fi
	@echo ""
	@echo "IR files:"
	@if [ -f $(SRC_DIR)/ir/ir.c ]; then \
		echo "✓ IR source file exists"; \
	else \
		echo "✗ IR source file missing"; \
	fi
	@if [ -f $(SRC_DIR)/ir/ir.h ]; then \
		echo "✓ IR header file exists"; \
	else \
		echo "✗ IR header file missing"; \
	fi
	@echo ""
	@echo "All required directories:"
	@for dir in $(DIRS); do \
		if [ -d $$dir ]; then \
			echo "✓ $$dir"; \
		else \
			echo "✗ $$dir (will be created during build)"; \
		fi; \
	done

# ================== 帮助信息 ==================
help:
	@echo "Mini-C++ Compiler Makefile Help"
	@echo "================================"
	@echo "Targets:"
	@echo "  all          - Build all phases (default: phase4)"
	@echo "  phase1       - Build phase 1 (Lexical & Syntax Analysis)"
	@echo "  phase2       - Build phase 2 (Semantic Analysis)"
	@echo "  phase3       - Build phase 3 (Intermediate Code Generation)"
	@echo "  phase4       - Build phase 4 (MIPS Code Generation)"
	@echo ""
	@echo "Testing:"
	@echo "  test         - Test with default test file"
	@echo "  test-phase3  - Test only phase 3"
	@echo "  test-phase4  - Test only phase 4"
	@echo "  test-all     - Test all phases sequentially"
	@echo "  create-test  - Create test directory and sample test file"
	@echo ""
	@echo "Cleaning:"
	@echo "  clean        - Clean all build files"
	@echo "  clean-phase34- Clean phase 3/4 specific files"
	@echo "  clean-ir     - Clean intermediate code files"
	@echo "  clean-mips   - Clean MIPS assembly files"
	@echo "  clean-parser - Clean parser generated files"
	@echo ""
	@echo "Development:"
	@echo "  check-files  - Check if required files exist"
	@echo "  debug        - Build with debug symbols"
	@echo "  debug-phase3 - Debug build for phase 3"
	@echo "  debug-phase4 - Debug build for phase 4"
	@echo "  release      - Build optimized release version"
	@echo ""
	@echo "Misc:"
	@echo "  help         - Show this help message"

# 包含依赖（可选）
#-include $(OBJ_DIR)/dependencies

.PHONY: all clean clean-phase34 clean-ir clean-mips clean-parser \
        phase1 phase2 phase3 phase4 \
        test test-phase3 test-phase4 test-all create-test \
        debug debug-phase3 debug-phase4 release \
        check-files help