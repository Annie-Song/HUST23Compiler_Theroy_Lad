/* 测试4：数组测试 */
int main() {
    int arr[10];              // 数组声明
    int i;
    
    // 数组赋值
    for (i = 0; i < 10; i++) {
        arr[i] = i * i;       // 数组访问
    }
    
    // 多维数组
    int matrix[3][3];
    matrix[0][0] = 1;
    matrix[1][1] = 2;
    matrix[2][2] = 3;
    
    return arr[5];
}