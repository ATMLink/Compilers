// atmlink/compilers/Compilers-7e07cfdaca0ac296ef0b06750c1d6dff49f71d74/lr0/src/lr0_parse_table.h

#ifndef LR0_PARSE_TABLE_H
#define LR0_PARSE_TABLE_H

#include "grammar.h"
#include "viable_prefix_dfa.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// +++ 修正：重新添加 MAX_STATES 定义 +++
#define MAX_STATES 128

// --- 哈希表核心数据结构 ---

// Action 表的节点，用于哈希表的链表
typedef struct ActionNode {
    uint8_t state;
    char terminal;
    struct ActionEntry* action;
    struct ActionNode* next; // 指向下一个冲突节点
} ActionNode;

// Goto 表的节点
typedef struct GotoNode {
    uint8_t state;
    char nonterminal;
    uint8_t to_state;
    struct GotoNode* next;
} GotoNode;

// Action 哈希表
typedef struct {
    ActionNode** buckets; // 指向哈希桶（链表头指针数组）
    size_t size;          // 哈希桶的数量
} ActionHashTable;

// GOTO 哈希表
typedef struct {
    GotoNode** buckets;
    size_t size;
} GotoHashTable;


// --- 分析表的核心数据结构 ---

typedef enum {
    ACTION_NONE = 0,
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT,
    ACTION_ERROR
} ActionType;

typedef struct ActionEntry {
    ActionType type;
    union {
        uint8_t shift_state; // ACTION_SHIFT
        uint8_t rule_index;  // ACTION_REDUCE
    } data;
} ActionEntry;

// LR0 分析表（已重构为使用哈希表）
typedef struct {
    ActionHashTable action_table; // Action表
    GotoHashTable goto_table;     // Goto表
    
    // 保留文法和符号计数的引用，方便打印和解析
    const Grammar* grammar;
    uint8_t state_count;
    uint8_t terminal_count;
    uint8_t nonterminal_count;

} LR0ParseTable;


// --- 函数声明 ---

/**
 * @brief 构建 LR(0) 语法分析表，并进行冲突检测
 * @param grammar 文法指针
 * @param dfa 活前缀DFA指针
 * @param table 要填充的分析表指针
 * @param arena 用于为哈希表分配内存的内存池
 * @return 如果没有冲突，返回 true；否则返回 false
 */
bool build_lr0_parse_table(const Grammar* grammar, const DFA* dfa, LR0ParseTable* table, Arena* arena);

void print_lr0_parse_table(const LR0ParseTable* table, FILE* out);

/**
 * @brief 使用生成的分析表解析输入字符串
 * @param table 分析表指针
 * @param input 要解析的输入字符串
 */
void lr0_parse_input(const LR0ParseTable* table, const char* input);

#endif // LR0_PARSE_TABLE_H