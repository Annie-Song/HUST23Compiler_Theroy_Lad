/* 测试6：简单函数测试（修复参数问题） */
int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

int main() {
    int a = 5;
    int b = 10;
    int m = max(a, b);
    return m;
}