#ifndef PREDICT_TABLE_H
#define PREDICT_TABLE_H

#include "grammar.h"
#include "first_follow.h"
#include "arena.h"

#define MAX_PREDICT_ENTRIES 512

// PredictEntry - 预测分析表中的一项条目
// 包含一个非终结符、一个终结符（或 epsilon），以及指向相应规则的指针
typedef struct PredictEntry {
    char nonterminal;   // 非终结符
    char terminal;      // 终结符（或 '#' 表示 epsilon）
    Rule* rule;         // 指向规则
} PredictEntry;

// PredictTable - 预测分析表
// 包含多个 PredictEntry 条目
typedef struct PredictTable {
    PredictEntry entries[MAX_PREDICT_ENTRIES];
    int entry_count;
} PredictTable;

// compute_predict_table - 计算预测分析表
// 根据文法和符号集合计算 LL(1) 预测分析表
void compute_predict_table(const Grammar* grammar,
                           SymbolSet* sets,
                           int set_count,
                           PredictTable* table);

// lookup_predict - 查找预测分析表
// 根据给定的非终结符和终结符查找相应的规则
const Rule* lookup_predict(const PredictTable* table, char nonterminal, char terminal);

// print_predict_table - 打印预测分析表
// 打印预测分析表的所有条目
void print_predict_table(const PredictTable* table);

#endif
