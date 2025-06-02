// viable_prefix_dfa.h - LR(0) 项集族 DFA 头文件
// 定义 LR(0) 活前缀 DFA 的数据结构、项集、转移和相关接口

#ifndef VIABLE_PREFIX_DFA_H
#define VIABLE_PREFIX_DFA_H

#include "grammar.h"
#include "arena.h"
#include <stdint.h>

// DFAItem 表示 LR(0) 项（产生式+点位置）
typedef struct DFAItem
{
    const char* restrict right_symbols; // 产生式右部符号串
    size_t right_len;                   // 右部长度
    char left_symbol;                   // 产生式左部非终结符
    uint8_t dot;                        // 点的位置
} DFAItem;

// ItemSet 表示一个项集（DFA 状态）
typedef struct ItemSet
{
    DFAItem* items;         // 项数组
    uint8_t item_count;     // 项数量
    uint8_t item_capacity;  // 分配的容量
} ItemSet;

// Transition 表示 DFA 的一条转移
typedef struct Transition{
    uint8_t from_state; // 起始状态编号
    char symbol;        // 转移符号
    uint8_t to_state;   // 目标状态编号
} Transition;

// DFA 结构体，表示 LR(0) 项集族自动机
typedef struct DFA{
    ItemSet* states;            // 所有状态（项集）
    uint8_t state_count;        // 状态数量
    uint8_t state_capacity;     // 状态容量
    Transition* transitions;    // 转移数组
    uint16_t transition_count;  // 转移数量
    uint16_t transition_capacity; // 转移容量
    Arena* arena;               // 内存池
} DFA;

// 构建 LR(0) 活前缀 DFA
void build_viable_prefix_dfa(const Grammar* grammar, DFA* dfa, Arena* arena);
// 释放 DFA 结构体（重置内容，实际内存由 arena 管理）
void dfa_free(DFA* dfa);
// 导出 DFA 到 dot 文件
void dfa_export_dot(const DFA* dfa, const char* filename);
// 打印 DFA 状态和转移
void print_dfa(const DFA* dfa);

#endif