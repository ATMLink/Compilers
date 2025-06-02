// tac_generator.h - 三地址码（TAC）生成器头文件
// 定义三地址码生成相关的数据结构和接口

#ifndef TAC_GENERATOR_H
#define TAC_GENERATOR_H

// 三地址码结构体定义
typedef struct {
    char op[10];     // 操作符
    char arg1[50];  // 操作数1
    char arg2[50];  // 操作数2
    char result[50]; // 结果
} TAC;

extern TAC tacs[];      // TAC 数组，存储生成的三地址码
extern int tac_count;   // 当前 TAC 数量
// extern int temp_count; // 临时变量计数（如果需要的话）

/**
 * @brief 生成三地址码
 * 
 * @param op 操作符
 * @param arg1 操作数1
 * @param arg2 操作数2
 * @param result 结果
 */
void gen_TAC(const char* op, const char* arg1, const char* arg2, const char* result);

/**
 * @brief 打印所有生成的三地址码
 */
void print_TAC();

/**
 * @brief 生成新的临时变量
 * 
 * @return 新的临时变量名
 */
char* new_temp();

/**
 * @brief 优化三地址码
 * 
 * 对生成的三地址码进行优化，以减少冗余和提高效率
 */
void optimize_TAC();

/**
 * @brief 初始化 TAC 生成器
 * 
 * 包括清空现有 TAC、重置计数等
 */
void init_TAC();

#endif
