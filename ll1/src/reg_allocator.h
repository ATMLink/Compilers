#ifndef REG_ALLOCATOR_H
#define REG_ALLOCATOR_H

// reg_allocator.h - 寄存器分配器头文件
// 定义寄存器分配相关的数据结构和接口

const char* allocate_reg(const char* temp_var);
// allocate_reg - 分配一个寄存器给临时变量
// temp_var: 临时变量的名称
// 返回: 分配的寄存器名称

void print_reg_allocations();
// print_reg_allocations - 打印当前的寄存器分配情况

#endif
