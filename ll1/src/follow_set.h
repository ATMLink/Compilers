#ifndef FOLLOW_SET_H
#define FOLLOW_SET_H

#include "arena.h"
#include "grammar.h"
#include "first_follow.h"

#include <string.h>
#include <stdbool.h>

// follow_set.h - Follow 集合头文件
// 定义 Follow 集合相关接口

/**
 * @brief 计算文法中每个非终结符的 Follow 集合
 *
 * @param grammar 文法
 * @param sets 存储 Follow 集合的数组
 * @param set_count 数组中集合的数量
 * @param arena 内存池，用于分配内存
 */
void compute_follow_sets(Grammar* grammar, SymbolSet* sets, int* set_count, Arena* arena);


#endif