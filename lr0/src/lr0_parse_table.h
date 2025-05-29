#ifndef LR0_PARSE_TABLE_H
#define LR0_PARSE_TABLE_H

#include "grammar.h"
#include "viable_prefix_dfa.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_STATES 128
#define MAX_SYMBOLS 64


typedef enum {
    ACTION_NONE = 0,
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT,
    ACTION_ERROR
} ActionType;

typedef struct {
    ActionType type;
    union {
        uint8_t shift_state; // ACTION_SHIFT
        uint8_t rule_index;  // ACTION_REDUCE
    } data;
} ActionEntry;

typedef struct {
    ActionEntry action[MAX_STATES][MAX_SYMBOLS]; // [state][terminal_index]
    int8_t go_to[MAX_STATES][MAX_SYMBOLS];       // [state][nonterminal_index]
    uint8_t state_count;
    uint8_t terminal_count;
    uint8_t nonterminal_count;
    const Grammar* grammar;
} LR0ParseTable;

void build_lr0_parse_table(const Grammar* grammar, const DFA* dfa, LR0ParseTable* table);
void print_lr0_parse_table(const Grammar* grammar, const LR0ParseTable* table, const DFA* dfa, FILE* out);

#endif // LR0_PARSE_TABLE_H
