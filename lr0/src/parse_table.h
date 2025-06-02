// parse_table.h - 分析表头文件
// 定义分析表相关的数据结构和接口

#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include "grammar.h"
#include "viable_prefix_dfa.h"

#define MAX_PARSE_STATES 128
#define MAX_SYMBOLS 128

typedef enum {
    ACTION_ERROR = 0,
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT
} ActionType;

typedef struct {
    ActionType type;
    uint8_t next_state;  // For SHIFT
    const Rule* rule;    // For REDUCE
} ActionEntry;

typedef struct {
    ActionEntry action[MAX_PARSE_STATES][GRAMMAR_MAX_SYMBOLS]; // ACTION[state][terminal]
    int8_t go_to[MAX_PARSE_STATES][GRAMMAR_MAX_SYMBOLS];       // GOTO[state][nonterminal]
} ParseTable;

void construct_parse_table(const Grammar* grammar, const DFA* dfa, ParseTable* table);

#endif
