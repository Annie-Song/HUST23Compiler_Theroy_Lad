#include "def.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* 简单的工具函数实现 */

/* 分配内存并检查 */
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    return ptr;
}

/* 复制字符串 */
char *safe_strdup(const char *str) {
    if (!str) return NULL;
    char *new_str = (char *)safe_malloc(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}

/* 格式化字符串 */
char *format_string(const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    return safe_strdup(buffer);
}