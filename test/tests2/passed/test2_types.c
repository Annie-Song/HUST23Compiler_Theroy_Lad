/* 测试2：类型检查和错误 */
int main() {
    int a = 10;
    float b = 3.14;
    char c = 'A';
    
    // 类型正确的赋值
    a = 100;
    b = 2.718;
    c = 'B';
    
    // 这些应该产生类型错误（测试时注释掉）
    // a = b;          // 错误：float赋值给int
    // b = c;          // 错误：char赋值给float
    // c = a;          // 警告：int赋值给char
    
    return 0;
}