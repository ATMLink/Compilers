// parse_table.c - 通用分析表实现
// 负责分析表的构建和查询

#include "parse_table.h"
#include "parse_utils.h"
#include <string.h>

void construct_parse_table(const Grammar* grammar, const DFA* dfa, ParseTable* table) {
    memset(table, 0, sizeof(ParseTable));

    for (int state = 0; state < dfa->state_count; ++state) {
        const ItemSet* item_set = &dfa->states[state];

        for (int i = 0; i < item_set->item_count; ++i) {
            const DFAItem* item = &item_set->items[i];
            char lhs = item->left_symbol;

            if (item->dot < item->right_len) {
                // [A → α • a β]，dot未到末尾
                char sym = item->right_symbols[item->dot];
                if (is_terminal(grammar, sym)) {
                    // Shift
                    for (int t = 0; t < dfa->transition_count; ++t) {
                        if (dfa->transitions[t].from_state == state &&
                            dfa->transitions[t].symbol == sym) {
                            int sym_idx = symbol_to_index(grammar, sym);
                            table->action[state][sym_idx] = (ActionEntry){
                                .type = ACTION_SHIFT,
                                .next_state = dfa->transitions[t].to_state,
                                .rule = NULL
                            };
                            break;
                        }
                    }
                }
            } else {
                // [A → α •]，dot在末尾
                if (lhs == grammar->rules[0].left_hs && item->right_len == 1 && item->right_symbols[0] == grammar->rules[0].right_hs[0]) {
                    // 是 S' → S • 的项目，Accept
                    int dollar_idx = symbol_to_index(grammar, '$'); // 假设你处理了 '$' 终结符
                    table->action[state][dollar_idx] = (ActionEntry){
                        .type = ACTION_ACCEPT,
                        .next_state = 0,
                        .rule = NULL
                    };
                } else {
                    // Reduce: 在 FOLLOW(lhs) 中对每个符号都加一个 reduce 动作
                    const bool* follow = get_follow_set(grammar, lhs);
                    for (int j = 0; j < grammar->terminals_count; ++j) {
                        char terminal = grammar->terminals[j];
                        if (follow[(uint8_t)terminal]) {
                            int sym_idx = symbol_to_index(grammar, terminal);
                            // 找到 rule
                            const Rule* rule = NULL;
                            for (int r = 0; r < grammar->rule_count; ++r) {
                                if (grammar->rules[r].left_hs == lhs &&
                                    strncmp(grammar->rules[r].right_hs, item->right_symbols, item->right_len) == 0) {
                                    rule = &grammar->rules[r];
                                    break;
                                }
                            }

                            if (rule != NULL) {
                                table->action[state][sym_idx] = (ActionEntry){
                                    .type = ACTION_REDUCE,
                                    .next_state = 0,
                                    .rule = rule
                                };
                            }
                        }
                    }
                }
            }
        }

        // 填充 GOTO 表
        for (int t = 0; t < dfa->transition_count; ++t) {
            if (dfa->transitions[t].from_state == state) {
                char sym = dfa->transitions[t].symbol;
                if (is_nonterminal(grammar, sym)) {
                    int sym_idx = symbol_to_index(grammar, sym);
                    table->go_to[state][sym_idx] = dfa->transitions[t].to_state;
                }
            }
        }
    }
}
