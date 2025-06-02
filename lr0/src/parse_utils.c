// parse_utils.c - 分析工具实现
// 负责 LR(0) 分析相关的辅助函数

#include "parse_utils.h"

// 将文法符号转换为索引
// 输入：文法指针和一个符号
// 输出：符号在文法中的索引，未找到则返回 -1
int symbol_to_index(const Grammar* grammar, char symbol) {
    for (int i = 0; i < grammar->terminals_count; ++i)
        if (grammar->terminals[i] == symbol) return i;
    for (int i = 0; i < grammar->nonterminals_count; ++i)
        if (grammar->nonterminals[i] == symbol) return i;
    return -1;
}

// 检查字符是否为终结符
// 输入：文法指针和一个字符
// 输出：如果字符是终结符，则返回 true；否则返回 false
bool is_terminal(const Grammar* grammar, char c) {
    for (int i = 0; i < grammar->terminals_count; ++i)
        if (grammar->terminals[i] == c) return true;
    return false;
}

// 检查字符是否为非终结符
// 输入：文法指针和一个字符
// 输出：如果字符是非终结符，则返回 true；否则返回 false
bool is_nonterminal(const Grammar* grammar, char c) {
    for (int i = 0; i < grammar->nonterminals_count; ++i)
        if (grammar->nonterminals[i] == c) return true;
    return false;
}
