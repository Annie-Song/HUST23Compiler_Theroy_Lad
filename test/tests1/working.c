int global_int = 42;

int factorial(int n) {
    int result;
    
    if (n <= 1) {
        result = 1;
    } else {
        result = n * factorial(n - 1);
    }
    
    return result;
}

int main() {
    int n;
    int sum;
    int i;
    
    n = 10;
    sum = 0;
    i = 1;
    
    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }
    
    return sum;
}
