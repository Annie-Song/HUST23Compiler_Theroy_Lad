#!/bin/bash

# 简单测试脚本
COMPILER="../../bin/minicpp"
TEST_DIR="."

echo "=== Mini-C++ Compiler Simple Semantic Tests ==="
echo ""

# 测试1：基础测试
echo "=== Test 1: Basic variable and function ==="
cat > test_basic.c << 'EOF'
/* 基础测试 */
int main() {
    int a = 10;
    int b = 20;
    int sum = a + b;
    return sum;
}
EOF
$COMPILER test_basic.c
echo ""

# 测试2：函数测试
echo "=== Test 2: Simple function ==="
cat > test_func.c << 'EOF'
/* 简单函数测试 */
int add(int x, int y) {
    return x + y;
}

int main() {
    int result = add(5, 10);
    return result;
}
EOF
$COMPILER test_func.c
echo ""

# 测试3：控制流测试
echo "=== Test 3: Control flow ==="
cat > test_control.c << 'EOF'
/* 控制流测试 */
int main() {
    int i;
    int sum = 0;
    
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + i;
    }
    
    if (sum > 0) {
        return sum;
    } else {
        return 0;
    }
}
EOF
$COMPILER test_control.c
echo ""

# 测试4：错误检测
echo "=== Test 4: Error detection ==="
cat > test_error.c << 'EOF'
/* 错误检测测试 */
int main() {
    int x = 10;
    int x = 20;  // 重复定义
    
    return 0;
}
EOF
$COMPILER test_error.c
echo ""

# 清理
rm -f test_basic.c test_func.c test_control.c test_error.c

echo "=== Tests completed ==="