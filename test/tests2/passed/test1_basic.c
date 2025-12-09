/* 测试1：基础变量和函数定义 */
int global_var;                // 全局变量

int add(int a, int b) {        // 函数定义
    return a + b;
}

int main() {
    int x = 10;                // 局部变量
    int y = 20;
    int result = add(x, y);    // 函数调用
    return result;
}