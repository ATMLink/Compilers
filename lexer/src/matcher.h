#ifndef MATCHER_H
#define MATCHER_H
#define ANY_CHAR -1

#include "nfa.h"

// matcher.h - 正则表达式匹配器头文件
// 定义正则匹配相关的数据结构和接口

typedef struct StateList {
    State* state;
    struct StateList* next;
} StateList;

/**
 * @brief 模拟 NFA 的状态转移
 * 
 * @param start NFA 的起始状态
 * @param input 输入的字符串
 * @param arena 内存池，用于分配临时状态
 * @return int 如果成功匹配返回 1，失败返回 0
 */
int simulate_nfa(State* start, const char* input, struct Arena* arena);

#endif