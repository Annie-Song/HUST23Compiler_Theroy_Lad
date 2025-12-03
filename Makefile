# Mini-C++ Compiler Makefile
# Phase 1: Lexical & Syntax Analysis

CC = gcc
CFLAGS = -Wall -g
FLEX = flex
BISON = bison

PROJECT_DIR = /home/annie/compiler-lab
SRC_DIR = $(PROJECT_DIR)/src
OBJ_DIR = $(PROJECT_DIR)/build
BIN_DIR = $(PROJECT_DIR)/bin

TARGET = $(BIN_DIR)/minicpp

# 默认目标
all: $(TARGET)

# 主目标依赖
$(TARGET): $(OBJ_DIR)/main.o $(OBJ_DIR)/ast/ast.o $(OBJ_DIR)/lex.yy.o $(OBJ_DIR)/parser.tab.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 编译普通源文件
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ast/ast.o: $(SRC_DIR)/ast/ast.c
	@mkdir -p $(OBJ_DIR)/ast
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# lex.yy.o
$(OBJ_DIR)/lex.yy.o: $(OBJ_DIR)/lex.yy.c $(OBJ_DIR)/parser.tab.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(OBJ_DIR) -c $< -o $@

# parser.tab.o
$(OBJ_DIR)/parser.tab.o: $(OBJ_DIR)/parser.tab.c $(OBJ_DIR)/parser.tab.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(OBJ_DIR) -c $< -o $@

# 生成语法分析器
$(OBJ_DIR)/parser.tab.c $(OBJ_DIR)/parser.tab.h: $(SRC_DIR)/parser/parser.y
	@echo "生成语法分析器..."
	@mkdir -p $(OBJ_DIR)
	cd $(PROJECT_DIR) && $(BISON) -d -o $(OBJ_DIR)/parser.tab.c $(SRC_DIR)/parser/parser.y

# 生成词法分析器
$(OBJ_DIR)/lex.yy.c: $(SRC_DIR)/lexer/lex.l $(OBJ_DIR)/parser.tab.h
	@echo "生成词法分析器..."
	@mkdir -p $(OBJ_DIR)
	cd $(PROJECT_DIR) && $(FLEX) -o $(OBJ_DIR)/lex.yy.c $(SRC_DIR)/lexer/lex.l

# 清理
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) parser.output

# 强制重新构建
rebuild: clean all

.PHONY: all clean rebuild