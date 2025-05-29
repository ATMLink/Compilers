#ifndef PREDICT_TABLE_H
#define PREDICT_TABLE_H

#include "grammar.h"
#include "first_follow.h"
#include "arena.h"

#define MAX_PREDICT_ENTRIES 512

typedef struct PredictEntry {
    char nonterminal;   // 非终结符
    char terminal;      // 终结符（或 '#' 表示 epsilon）
    Rule* rule;         // 指向规则
} PredictEntry;

typedef struct PredictTable {
    PredictEntry entries[MAX_PREDICT_ENTRIES];
    int entry_count;
} PredictTable;

void compute_predict_table(const Grammar* grammar,
                           SymbolSet* sets,
                           int set_count,
                           PredictTable* table);

const Rule* lookup_predict(const PredictTable* table, char nonterminal, char terminal);

void print_predict_table(const PredictTable* table);

#endif
