/* 测试3：作用域测试 */
int global = 100;

void func1() {
    int local1 = 10;
    {
        int local2 = 20;      // 内层作用域
        local1 = local2;      // 正确：访问外层变量
    }
    // local2 = 30;           // 错误：local2不在作用域内
}

void func2() {
    int local1 = 40;          // 正确：与func1中的local1不同
    global = 200;             // 正确：访问全局变量
}

int main() {
    func1();
    func2();
    return 0;
}