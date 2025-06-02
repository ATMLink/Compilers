#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include "grammar.h"

// parse_utils.h - 分析工具头文件
// 定义 LR(0) 分析相关的辅助函数接口

int symbol_to_index(const Grammar* grammar, char symbol);
// 将文法符号转换为对应的索引值

bool is_terminal(const Grammar* grammar, char symbol);
// 判断给定符号是否为终结符

bool is_nonterminal(const Grammar* grammar, char symbol);
// 判断给定符号是否为非终结符

// FOLLOW 集应由你已有的 FIRST/FOLLOW 模块提供
const bool* get_follow_set(const Grammar* grammar, char nonterminal); // 返回 bool[GRAMMAR_MAX_SYMBOLS]

#endif
