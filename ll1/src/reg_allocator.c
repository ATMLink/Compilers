// reg_allocator.c - 寄存器分配器实现
// 负责为中间代码分配物理寄存器

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

/**
 * allocate_reg - 为给定的临时变量分配寄存器
 * @temp_var: 临时变量的名称
 *
 * 如果临时变量已经有对应的寄存器映射，直接返回该寄存器；
 * 否则，分配一个新的寄存器并建立映射关系。
 *
 * 返回分配的寄存器名称，成功则返回寄存器字符串，失败则返回NULL。
 */
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

/**
 * print_reg_allocations - 打印当前的寄存器分配情况
 *
 * 遍历所有的寄存器映射关系，并打印每个临时变量及其对应的寄存器。
 */
void print_reg_allocations() {
    printf("\n;=== Register Allocation ===\n");
    for (int i = 0; i < reg_count; i++) {
        printf("; %s -> %s\n", reg_table[i].temp, reg_table[i].reg);
    }
}
