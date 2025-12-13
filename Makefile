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
# 新增IR和MIPS源文件
IR_SRC = $(SRC_DIR)/ir/ir.c
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
# 新增IR和MIPS目标文件
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
       $(OBJ_DIR)/utils $(OBJ_DIR)/ir $(OBJ_DIR)/mips \
       $(SRC_DIR)/ir $(SRC_DIR)/mips

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
$(MAIN_OBJ): $(MAIN_SRC)
	@echo "Compiling main program..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# AST
$(AST_OBJ): $(AST_SRC) $(SRC_DIR)/ast/ast.h
	@echo "Compiling AST..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# 语义分析
$(OBJ_DIR)/semantic/semantic.o: $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/semantic/semantic.h
	@echo "Compiling semantic analysis..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/error.o: $(SRC_DIR)/semantic/error.c $(SRC_DIR)/semantic/error.h
	@echo "Compiling error handling..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/type.o: $(SRC_DIR)/semantic/type.c $(SRC_DIR)/semantic/type.h
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

# ================== 新增：中间代码生成 ==================
$(IR_OBJ): $(IR_SRC) $(SRC_DIR)/ir/ir.h
	@echo "Compiling intermediate code generator..."
	$(CC) $(CFLAGS) $(CURRENT_PHASE_FLAGS) -c $< -o $@

# ================== 新增：目标代码生成 ==================
$(MIPS_OBJ): $(MIPS_SRC) $(SRC_DIR)/mips/mips.h $(SRC_DIR)/ir/ir.h
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
	rm -rf $(OBJ_DIR) $(BIN_DIR) parser.output *.ir *.s
	@echo "Clean complete."

# 清理阶段三、四相关文件
clean-phase34:
	@echo "Cleaning phase 3/4 files..."
	rm -rf $(OBJ_DIR)/ir $(OBJ_DIR)/mips $(SRC_DIR)/ir $(SRC_DIR)/mips
	rm -f *.ir *.s
	@echo "Phase 3/4 clean complete."

# 清理中间代码文件
clean-ir:
	@echo "Cleaning IR files..."
	rm -f *.ir
	@echo "IR files cleaned."

# 清理目标代码文件
clean-mips:
	@echo "Cleaning MIPS files..."
	rm -f *.s
	@echo "MIPS files cleaned."

# ================== 测试目标 ==================
# 测试所有阶段
test-all:
	@echo "Testing all phases..."
	$(MAKE) phase1 && $(TARGET) test/test1.c 2>&1 | head -20
	@echo "---"
	$(MAKE) phase2 && $(TARGET) test/test1.c 2>&1 | tail -20
	@echo "---"
	$(MAKE) phase3 && $(TARGET) test/test1.c 2>&1 | tail -20
	@echo "---"
	$(MAKE) phase4 && $(TARGET) test/test1.c 2>&1 | tail -20

# 测试阶段三
test-phase3: phase3
	@echo "Testing Phase 3 (Intermediate Code)..."
	$(TARGET) test/test1.c 2>&1 | grep -A 50 "Intermediate Code" || true

# 测试阶段四
test-phase4: phase4
	@echo "Testing Phase 4 (MIPS Code)..."
	$(TARGET) test/test1.c 2>&1 | grep -A 50 "MIPS Assembly Code" || true
	@echo "MIPS code saved to output.s"

# 快速测试（使用阶段四）
test: phase4
	@echo "Running test with Phase 4..."
	$(TARGET) test/test1.c
	@echo "Check output.s for MIPS assembly"

# ================== 调试目标 ==================
debug: CFLAGS += -DDEBUG -O0 -g3
debug: phase4
	@echo "Debug version built with full debug symbols"

# 只调试阶段三
debug-phase3: CFLAGS += -DDEBUG -O0 -g3
debug-phase3: phase3
	@echo "Phase 3 debug version built"

# ================== 发布目标 ==================
release: CFLAGS = -Wall -O2
release: phase4
	@echo "Release version built with optimizations"

# ================== 依赖生成 ==================
depend:
	@echo "Generating dependencies..."
	$(CC) -MM $(CFLAGS) $(MAIN_SRC) $(AST_SRC) $(SEMANTIC_SRC) \
		$(SYMBOL_SRC) $(UTILS_SRC) $(IR_SRC) $(MIPS_SRC) \
		> $(OBJ_DIR)/dependencies 2>/dev/null || true

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
	@echo ""
	@echo "Cleaning:"
	@echo "  clean        - Clean all build files"
	@echo "  clean-phase34- Clean phase 3/4 specific files"
	@echo "  clean-ir     - Clean intermediate code files"
	@echo "  clean-mips   - Clean MIPS assembly files"
	@echo ""
	@echo "Other:"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  help         - Show this help message"

# 包含依赖
-include $(OBJ_DIR)/dependencies

.PHONY: all clean clean-phase34 clean-ir clean-mips phase1 phase2 phase3 phase4 \
        test test-phase3 test-phase4 test-all debug debug-phase3 release depend help