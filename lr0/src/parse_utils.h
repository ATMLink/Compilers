#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include "grammar.h"

int symbol_to_index(const Grammar* grammar, char symbol);
bool is_terminal(const Grammar* grammar, char symbol);
bool is_nonterminal(const Grammar* grammar, char symbol);

// FOLLOW 集应由你已有的 FIRST/FOLLOW 模块提供
const bool* get_follow_set(const Grammar* grammar, char nonterminal); // 返回 bool[GRAMMAR_MAX_SYMBOLS]

#endif
