// predict_table.c - 预测分析表实现
// 负责 LL(1) 预测分析表的构建和查询

#include "predict_table.h"
#include <string.h>
#include <stdio.h>

// 获取符号的 SymbolSet*
static SymbolSet* find_symbol_set(SymbolSet* sets, int count, char symbol) {
    for (int i = 0; i < count; ++i) {
        if (sets[i].symbol == symbol) return &sets[i];
    }
    return NULL;
}

// 为预测分析表添加一个条目
static void add_predict_entry(PredictTable* table, char nonterminal, char terminal, Rule* rule) {
    // 防止冲突：一个 [非终结符, 终结符] 对应多个规则，说明不是 LL(1)
    for (int i = 0; i < table->entry_count; ++i) {
        PredictEntry* e = &table->entries[i];
        if (e->nonterminal == nonterminal && e->terminal == terminal) {
            fprintf(stderr, "Conflict in predict table: %c with %c already mapped.\n", nonterminal, terminal);
            return;
        }
    }

    table->entries[table->entry_count++] = (PredictEntry){
        .nonterminal = nonterminal,
        .terminal = terminal,
        .rule = rule
    };
}

// 计算预测分析表
void compute_predict_table(const Grammar* grammar,
                           SymbolSet* sets,
                           int set_count,
                           PredictTable* table)
{
    table->entry_count = 0;

    for (int i = 0; i < grammar->rule_count; ++i) {
        Rule* rule = &grammar->rules[i];
        SymbolSet* left_set = find_symbol_set(sets, set_count, rule->left_hs);
        if (!left_set) continue;

        // 1. 获取 First(右边串)
        bool derives_epsilon = true;
        for (int j = 0; j < rule->right_hs_count; ++j) {
            char sym = rule->right_hs[j];
            if (grammar_is_terminal(sym)) {
                add_predict_entry(table, rule->left_hs, sym, rule);
                derives_epsilon = false;
                break;
            }

            SymbolSet* sym_set = find_symbol_set(sets, set_count, sym);
            if (!sym_set) break;

            for (int k = 0; sym_set->first[k]; ++k) {
                if (sym_set->first[k] != '#') {
                    add_predict_entry(table, rule->left_hs, sym_set->first[k], rule);
                }
            }

            if (!strchr(sym_set->first, '#')) {
                derives_epsilon = false;
                break;
            }
        }

        // 2. 如果 First(α) 包含 ε，加入 Follow(A)
        if (derives_epsilon) {
            for (int j = 0; left_set->follow[j]; ++j) {
                add_predict_entry(table, rule->left_hs, left_set->follow[j], rule);
            }
        }
    }
}

// 查找预测分析表中的规则
const Rule* lookup_predict(const PredictTable* table, char nonterminal, char terminal) {
    for (int i = 0; i < table->entry_count; ++i) {
        if (table->entries[i].nonterminal == nonterminal &&
            table->entries[i].terminal == terminal)
            return table->entries[i].rule;
    }
    return NULL;
}

// 打印预测分析表
void print_predict_table(const PredictTable* table) {
    printf("Predict Table:\n");
    for (int i = 0; i < table->entry_count; ++i) {
        const PredictEntry* e = &table->entries[i];
        printf("  M[%c, %c] = %c -> %s\n",
               e->nonterminal,
               e->terminal,
               e->rule->left_hs,
               e->rule->right_hs);
    }
}
