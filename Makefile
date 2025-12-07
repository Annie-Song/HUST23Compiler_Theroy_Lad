CC = gcc
CFLAGS = -Wall -g -I$(SRC_DIR)
FLEX = flex
BISON = bison

PROJECT_DIR = /home/annie/compiler-lab
SRC_DIR = $(PROJECT_DIR)/src
OBJ_DIR = $(PROJECT_DIR)/build
BIN_DIR = $(PROJECT_DIR)/bin

# 阶段二编译选项
PHASE2_FLAGS = -DPHASE2 -DSHOW_SYMBOL_TABLE

TARGET = $(BIN_DIR)/minicpp

# 源文件列表
AST_SRC = $(SRC_DIR)/ast/ast.c
LEX_SRC = $(OBJ_DIR)/lex.yy.c
PARSE_SRC = $(OBJ_DIR)/parser.tab.c
SEMANTIC_SRC = $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/semantic/error.c $(SRC_DIR)/semantic/type.c
SYMBOL_SRC = $(SRC_DIR)/symbol/symbol.c
UTILS_SRC = $(SRC_DIR)/utils/utils.c
MAIN_SRC = $(SRC_DIR)/main.c

# 目标文件列表
AST_OBJ = $(OBJ_DIR)/ast/ast.o
LEX_OBJ = $(OBJ_DIR)/lex.yy.o
PARSE_OBJ = $(OBJ_DIR)/parser.tab.o
SEMANTIC_OBJS = $(OBJ_DIR)/semantic/semantic.o $(OBJ_DIR)/semantic/error.o $(OBJ_DIR)/semantic/type.o
SYMBOL_OBJ = $(OBJ_DIR)/symbol/symbol.o
UTILS_OBJ = $(OBJ_DIR)/utils/utils.o
MAIN_OBJ = $(OBJ_DIR)/main.o

OBJS = $(MAIN_OBJ) $(AST_OBJ) $(LEX_OBJ) $(PARSE_OBJ) $(SEMANTIC_OBJS) $(SYMBOL_OBJ) $(UTILS_OBJ)

# 默认目标
all: $(TARGET)

# 主目标
$(TARGET): $(OBJS)
	@echo "Linking..."
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 主程序
$(MAIN_OBJ): $(MAIN_SRC)
	@echo "Compiling main..."
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

# AST
$(AST_OBJ): $(AST_SRC) $(SRC_DIR)/ast/ast.h
	@echo "Compiling AST..."
	@mkdir -p $(OBJ_DIR)/ast
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

# 语义分析
$(OBJ_DIR)/semantic/semantic.o: $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/semantic/semantic.h
	@echo "Compiling semantic analysis..."
	@mkdir -p $(OBJ_DIR)/semantic
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/error.o: $(SRC_DIR)/semantic/error.c $(SRC_DIR)/semantic/error.h
	@echo "Compiling error handling..."
	@mkdir -p $(OBJ_DIR)/semantic
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

$(OBJ_DIR)/semantic/type.o: $(SRC_DIR)/semantic/type.c $(SRC_DIR)/semantic/type.h
	@echo "Compiling type system..."
	@mkdir -p $(OBJ_DIR)/semantic
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

# 符号表
$(SYMBOL_OBJ): $(SYMBOL_SRC) $(SRC_DIR)/symbol/symbol.h
	@echo "Compiling symbol table..."
	@mkdir -p $(OBJ_DIR)/symbol
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

# 工具函数
$(UTILS_OBJ): $(UTILS_SRC) $(SRC_DIR)/utils/def.h
	@echo "Compiling utils..."
	@mkdir -p $(OBJ_DIR)/utils
	$(CC) $(CFLAGS) $(PHASE2_FLAGS) -c $< -o $@

# 词法分析器
$(LEX_OBJ): $(LEX_SRC) $(OBJ_DIR)/parser.tab.h
	@echo "Compiling lexer..."
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# 语法分析器
$(PARSE_OBJ): $(PARSE_SRC) $(OBJ_DIR)/parser.tab.h
	@echo "Compiling parser..."
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# 生成语法分析器
$(PARSE_SRC) $(OBJ_DIR)/parser.tab.h: $(SRC_DIR)/parser/parser.y
	@echo "Generating parser..."
	@mkdir -p $(OBJ_DIR)
	cd $(OBJ_DIR) && $(BISON) -d -o parser.tab.c $(SRC_DIR)/parser/parser.y

# 生成词法分析器
$(LEX_SRC): $(SRC_DIR)/lexer/lex.l $(OBJ_DIR)/parser.tab.h
	@echo "Generating lexer..."
	@mkdir -p $(OBJ_DIR)
	cd $(OBJ_DIR) && $(FLEX) -o lex.yy.c $(SRC_DIR)/lexer/lex.l

# 清理目标
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) parser.output

# 阶段一（无语义分析）
phase1:
	$(MAKE) clean
	$(MAKE) PHASE2_FLAGS=""

# 阶段二（有语义分析）
phase2:
	$(MAKE) clean
	$(MAKE) PHASE2_FLAGS="-DPHASE2 -DSHOW_SYMBOL_TABLE"

# 测试
test: $(TARGET)
	@echo "Testing..."
	$(TARGET) test/test1.c 2>&1 || true

# 调试
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# 发布
release: CFLAGS = -Wall -O2
release: $(TARGET)

# 依赖关系
depend:
	@echo "Generating dependencies..."
	@mkdir -p $(OBJ_DIR)
	$(CC) -MM $(CFLAGS) $(MAIN_SRC) $(AST_SRC) $(SEMANTIC_SRC) $(SYMBOL_SRC) $(UTILS_SRC) > $(OBJ_DIR)/dependencies

# 包含依赖
-include $(OBJ_DIR)/dependencies

.PHONY: all clean phase1 phase2 test debug release depend