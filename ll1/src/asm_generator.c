#include <stdio.h>
#include <string.h>
#include "tac_generator.h"
#include "reg_allocator.h"

void gen_asm() {
    for (int i = 0; i < tac_count; i++) {
        const char* arg1 = tacs[i].arg1;
        const char* arg2 = tacs[i].arg2;
        const char* result = tacs[i].result;

        // 处理常量折叠后的赋值
        if (tacs[i].op[0] == '\0') {
            const char* reg = allocate_reg(result);
            printf("MOV %s, %s\n", reg, arg1);
            continue;
        }

        const char* reg_result = allocate_reg(result);
        const char* reg1 = (arg1[0] == 't') ? allocate_reg(arg1) : "R100"; // 假设 R100 为立即数加载寄存器
        const char* reg2 = (arg2[0] == 't') ? allocate_reg(arg2) : "R101"; // R101 同理

        if (arg1[0] != 't') printf("MOV %s, %s\n", reg1, arg1);
        if (arg2[0] != 't') printf("MOV %s, %s\n", reg2, arg2);

        if (strcmp(tacs[i].op, "+") == 0) {
            printf("MOV %s, %s\n", reg_result, reg1);
            printf("ADD %s, %s\n", reg_result, reg2);
        } else if (strcmp(tacs[i].op, "-") == 0) {
            printf("MOV %s, %s\n", reg_result, reg1);
            printf("SUB %s, %s\n", reg_result, reg2);
        } else if (strcmp(tacs[i].op, "*") == 0) {
            printf("MOV %s, %s\n", reg_result, reg1);
            printf("MUL %s, %s\n", reg_result, reg2);
        } else if (strcmp(tacs[i].op, "/") == 0) {
            printf("MOV %s, %s\n", reg_result, reg1);
            printf("DIV %s, %s\n", reg_result, reg2);
        } else {
            printf("; Unknown op: %s\n", tacs[i].op);
        }
    }

    print_reg_allocations();
}
