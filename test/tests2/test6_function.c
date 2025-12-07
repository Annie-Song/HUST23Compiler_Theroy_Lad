/* 测试6：函数和参数测试 */
int max(int x, int y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);  // 递归调用
}

int main() {
    int a = 5;
    int b = 10;
    
    int m = max(a, b);           // 函数调用
    int fact = factorial(5);     // 递归调用
    
    return m + fact;
}