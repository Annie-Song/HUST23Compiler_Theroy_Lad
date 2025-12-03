// 最终测试 - 包含所有要求的语言特性

int global_array[10];  // 全局数组
float global_float = 3.14;
char global_char = 'X';

// 测试所有特性的函数
int test_all_features() {
    // 1. 各种数据类型
    int a = 10;
    float b = 2.5;
    char c = 'A';
    
    // 2. 算术运算
    int sum = a + (int)b;
    float product = a * b;
    
    // 3. 自增自减
    a++;
    ++a;
    b -= 1.0;
    
    // 4. 复合赋值
    a += 5;
    sum *= 2;
    
    // 5. 比较运算
    if (a > sum && b != 0.0) {
        // 6. 数组操作
        int arr[3][4];
        arr[1][2] = a;
        
        // 7. for循环
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                arr[i][j] = i * 10 + j;
            }
        }
        
        // 8. while循环
        int k = 0;
        while (k < 10) {
            if (k == 5) {
                break;  // 测试break
            }
            if (k % 2 == 0) {
                k++;
                continue;  // 测试continue
            }
            k++;
        }
    }
    
    return a + sum;
}

// 主函数
int main() {
    int result = test_all_features();
    
    /* 
    块注释测试：
    这是一个多行注释
    可以跨越多行
    */
    
    return result;  // 行注释测试
}
