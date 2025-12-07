#!/bin/bash

# 测试脚本
COMPILER="../../bin/minicpp"
TEST_DIR="."
LOG_FILE="test_results.log"

echo "=== Mini-C++ Compiler Semantic Analysis Tests ===" > $LOG_FILE
echo "Test started at: $(date)" >> $LOG_FILE
echo "" >> $LOG_FILE

# 测试计数器
total_tests=0
passed_tests=0
failed_tests=0

# 运行测试
run_test() {
    local test_file=$1
    local test_name=$2
    local expect_error=$3
    
    echo "Testing: $test_name ($test_file)" | tee -a $LOG_FILE
    
    total_tests=$((total_tests + 1))
    
    # 运行编译器
    $COMPILER "$test_file" 2>&1 | tee -a $LOG_FILE
    local exit_code=${PIPESTATUS[0]}
    
    echo "" >> $LOG_FILE
    echo "Exit code: $exit_code" >> $LOG_FILE
    
    # 检查结果
    if [ $expect_error -eq 0 ]; then
        # 期望成功
        if [ $exit_code -eq 0 ]; then
            echo "✓ PASS: $test_name" | tee -a $LOG_FILE
            passed_tests=$((passed_tests + 1))
        else
            echo "✗ FAIL: $test_name (expected success)" | tee -a $LOG_FILE
            failed_tests=$((failed_tests + 1))
        fi
    else
        # 期望错误
        if [ $exit_code -ne 0 ]; then
            echo "✓ PASS: $test_name (correctly detected error)" | tee -a $LOG_FILE
            passed_tests=$((passed_tests + 1))
        else
            echo "✗ FAIL: $test_name (expected error but succeeded)" | tee -a $LOG_FILE
            failed_tests=$((failed_tests + 1))
        fi
    fi
    
    echo "----------------------------------------" >> $LOG_FILE
    echo ""
}

# 创建编译器（如果不存在）
cd ../..
make clean
make phase2
cd test/tests2

# 运行所有测试
echo "Running semantic analysis tests..."
echo ""

# 正常测试（期望成功）
run_test "test1_basic.c" "Basic variables and functions" 0
run_test "test2_types.c" "Type checking" 0
run_test "test3_scope.c" "Scope testing" 0
run_test "test4_array.c" "Array operations" 0
run_test "test5_control.c" "Control flow" 0
run_test "test6_function.c" "Functions and parameters" 0
run_test "test8_expressions.c" "Complex expressions" 0

# 错误测试（期望报错）
run_test "test7_errors.c" "Error detection" 1

# 显示总结
echo "=== Test Summary ===" | tee -a $LOG_FILE
echo "Total tests: $total_tests" | tee -a $LOG_FILE
echo "Passed: $passed_tests" | tee -a $LOG_FILE
echo "Failed: $failed_tests" | tee -a $LOG_FILE

if [ $failed_tests -eq 0 ]; then
    echo "✓ All tests passed!" | tee -a $LOG_FILE
    exit 0
else
    echo "✗ Some tests failed!" | tee -a $LOG_FILE
    exit 1
fi