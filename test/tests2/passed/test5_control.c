/* 测试5：控制流测试 */
int main() {
    int a = 10;
    int b = 20;
    int result = 0;
    
    // if语句
    if (a < b) {
        result = 1;
    } else {
        result = 2;
    }
    
    // while循环
    int i = 0;
    while (i < 10) {
        i = i + 1;
    }
    
    // for循环
    for (int j = 0; j < 10; j = j + 1) {
        result = result + j;
    }
    
    // break和continue
    for (int k = 0; k < 10; k = k + 1) {
        if (k == 5) {
            break;            // 跳出循环
        }
        if (k % 2 == 0) {
            continue;         // 跳过本次循环
        }
        result = result + k;
    }
    
    return result;
}