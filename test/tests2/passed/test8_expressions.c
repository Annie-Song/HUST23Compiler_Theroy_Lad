/* 测试8：复杂表达式测试 */
int main() {
    int a = 10;
    int b = 20;
    int c = 30;
    
    // 算术表达式
    int result1 = a + b * c / 2 - 5;
    
    // 关系表达式
    int result2 = (a < b) && (b < c);
    
    // 赋值表达式
    a += b;           // a = a + b
    b -= c;           // b = b - c
    c *= a;           // c = c * a
    
    // 位运算（如果支持）
    // int result4 = a & b;
    // int result5 = a | b;
    
    return result1 + result2 ;
}