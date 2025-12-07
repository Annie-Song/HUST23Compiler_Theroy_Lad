/* 测试7：错误测试（应该报错） */
int main() {
    // 错误1：未定义变量
    // undefined_var = 10;
    
    // 错误2：重复定义
    int x = 10;
    // int x = 20;          // 重复定义
    
    // 错误3：类型不匹配
    int a = 10;
    float b = 3.14;
    // a = b;               // 类型不匹配
    
    // 错误4：break不在循环中
    // break;
    
    // 错误5：continue不在循环中
    // continue;
    
    // 错误6：函数缺少返回语句
    // void no_return() {
    //     int x = 10;
    //     // 缺少return
    // }
    
    return 0;
}