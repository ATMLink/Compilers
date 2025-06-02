// tac_generator.c - 三地址码（TAC）生成器实现
// 负责将 AST 转换为三地址码

#include "tac_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAC 1000


TAC tacs[MAX_TAC];
int tac_count = 0;
static int temp_count = 0;

void init_TAC() {
    // 初始化 TAC 计数器和临时变量计数器
    tac_count = 0;
    temp_count = 0;
}

void gen_TAC(const char* op, const char* arg1, const char* arg2, const char* result) {
    // 生成三地址码并存储
    if (tac_count >= MAX_TAC) return;
    strcpy(tacs[tac_count].op, op);
    strcpy(tacs[tac_count].arg1, arg1);
    strcpy(tacs[tac_count].arg2, arg2);
    strcpy(tacs[tac_count].result, result);
    tac_count++;
}

char* new_temp() {
    // 生成新的临时变量名
    static char temp[10];
    sprintf(temp, "t%d", ++temp_count);
    return strdup(temp);
}

void print_TAC() {
    // 打印所有生成的三地址码
    for (int i = 0; i < tac_count; i++) {
        printf("%s = %s %s %s\n", tacs[i].result, tacs[i].arg1, tacs[i].op, tacs[i].arg2);
    }
}

void optimize_TAC() {
    // 优化三地址码，进行常量折叠
    for (int i = 0; i < tac_count; i++) {
        char* endptr1;
        char* endptr2;
        int v1 = strtol(tacs[i].arg1, &endptr1, 10);
        int v2 = strtol(tacs[i].arg2, &endptr2, 10);
        if (*endptr1 == '\0' && *endptr2 == '\0') {
            int result = 0;
            if (strcmp(tacs[i].op, "+") == 0) result = v1 + v2;
            else if (strcmp(tacs[i].op, "*") == 0) result = v1 * v2;
            // ...
            sprintf(tacs[i].arg1, "%d", result);
            tacs[i].op[0] = '\0'; // 直接赋值
            tacs[i].arg2[0] = '\0';
        }
    }
}
