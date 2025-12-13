// 测试阶段三的简单程序
int main() {
    // 基本表达式测试
    int a = 5;
    int b = 3;
    int c = a + b;
    
    // 条件语句测试
    if (c > 7) {
        c = c - 2;
    }
    
    // 循环测试
    int i = 0;
    int sum = 0;
    while (i < 10) {
        sum = sum + i;
        i = i + 1;
    }
    
    // 函数调用测试（假设有read/write函数）
    int x = read();
    write(x + sum);
    
    return c;
}

// 测试函数定义
int add(int x, int y) {
    int result = x + y;
    return result;
}

// 测试嵌套函数调用
int multiply(int x, int y) {
    int temp = 0;
    int i = 0;
    while (i < y) {
        temp = temp + x;
        i = i + 1;
    }
    return temp;
}