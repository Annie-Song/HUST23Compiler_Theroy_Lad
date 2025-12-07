# 测试Makefile
CC = ../../bin/minicpp
TESTS = test1_basic.c test2_types.c test3_scope.c test4_array.c \
        test5_control.c test6_function.c test7_errors.c test8_expressions.c

all: run

run: $(TESTS)
	@echo "Running all tests..."
	@./run_tests.sh

clean:
	@echo "Cleaning test outputs..."
	@rm -f *.log
	@rm -f *.out
	@rm -f test_results.log

.PHONY: all run clean