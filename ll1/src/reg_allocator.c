#include "reg_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP_VARS 100

typedef struct {
    char temp[10];
    char reg[10];
} RegMap;

static RegMap reg_table[MAX_TEMP_VARS];
static int reg_count = 0;

const char* allocate_reg(const char* temp_var) {
    // 如果已有映射，直接返回
    for (int i = 0; i < reg_count; i++) {
        if (strcmp(reg_table[i].temp, temp_var) == 0)
            return reg_table[i].reg;
    }

    // 分配新寄存器
    if (reg_count >= MAX_TEMP_VARS) return NULL; // 超出上限
    sprintf(reg_table[reg_count].temp, "%s", temp_var);
    sprintf(reg_table[reg_count].reg, "R%d", reg_count + 1);
    return reg_table[reg_count++].reg;
}

void print_reg_allocations() {
    printf("\n;=== Register Allocation ===\n");
    for (int i = 0; i < reg_count; i++) {
        printf("; %s -> %s\n", reg_table[i].temp, reg_table[i].reg);
    }
}
