#include "lr0_parse_table.h"
#include "viable_prefix_dfa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility: Get index of a terminal symbol in grammar->terminals
static int get_terminal_index(const Grammar* grammar, char terminal) {
    for (int i = 0; i < grammar->terminals_count; i++) {
        if (grammar->terminals[i] == terminal) return i;
    }
    return -1;
}

// Utility: Get index of a non-terminal symbol in grammar->nonterminals
static int get_nonterminal_index(const Grammar* grammar, char nonterminal) {
    for (int i = 0; i < grammar->nonterminals_count; i++) {
        if (grammar->nonterminals[i] == nonterminal) return i;
    }
    return -1;
}

// Check if DFA item is of the form A -> alpha . (dot at end)
static bool is_reduce_item(const DFAItem* item) {
    return item->dot >= item->right_len;
}

// Find rule index in grammar matching DFA item
static int find_rule_index(const Grammar* grammar, const DFAItem* item) {
    for (int i = 0; i < grammar->rule_count; i++) {
        Rule* rule = &grammar->rules[i];
        if (rule->left_hs == item->left_symbol && rule->right_hs_count == item->right_len) {
            if (strncmp(rule->right_hs, item->right_symbols, item->right_len) == 0) {
                return i;
            }
        }
    }
    return -1;
}

void build_lr0_parse_table(const Grammar* grammar, const DFA* dfa, LR0ParseTable* table) {
    memset(table->action, ACTION_ERROR, sizeof(table->action));
    memset(table->go_to, -1, sizeof(table->go_to));

    for (int state = 0; state < dfa->state_count; state++) {
        ItemSet* set = &dfa->states[state];
        for (int i = 0; i < set->item_count; i++) {
            DFAItem* item = &set->items[i];

            if (is_reduce_item(item)) {
                int rule_index = find_rule_index(grammar, item);
                if (item->left_symbol == grammar->rules[0].left_hs && item->right_len == grammar->rules[0].right_hs_count) {
                    int dollar_index = get_terminal_index(grammar, '#');
                    table->action[state][dollar_index].type = ACTION_ACCEPT;
                } else {
                    for (int t = 0; t < grammar->terminals_count; t++) {
                        table->action[state][t].type = ACTION_REDUCE;
                        table->action[state][t].data.rule_index = rule_index;
                    }
                }
            }
        }
    }

    for (int t = 0; t < dfa->transition_count; t++) {
        Transition* tr = &dfa->transitions[t];
        char sym = tr->symbol;
        int from = tr->from_state;
        int to = tr->to_state;

        if (grammar_is_terminal(sym)) {
            int index = get_terminal_index(grammar, sym);
            table->action[from][index].type = ACTION_SHIFT;
            table->action[from][index].data.shift_state = to;
        } else {
            int index = get_nonterminal_index(grammar, sym);
            table->go_to[from][index] = to;
        }
    }
}

void print_lr0_parse_table(const Grammar* grammar, const LR0ParseTable* table, const DFA* dfa, FILE* out) {
    fprintf(out, "LR(0) ACTION Table:\n");
    fprintf(out, "State\t");
    for (int t = 0; t < grammar->terminals_count; t++) {
        fprintf(out, "%c\t", grammar->terminals[t]);
    }
    fprintf(out, "\n");
    for (int s = 0; s < dfa->state_count; s++) {
        fprintf(out, "%d\t", s);
        for (int t = 0; t < grammar->terminals_count; t++) {
            ActionEntry entry = table->action[s][t];
            switch (entry.type) {
                case ACTION_SHIFT:
                    fprintf(out, "s%d\t", entry.data.shift_state);
                    break;
                case ACTION_REDUCE:
                    fprintf(out, "r%d\t", entry.data);
                    break;
                case ACTION_ACCEPT:
                    fprintf(out, "acc\t");
                    break;
                case ACTION_ERROR:
                default:
                    fprintf(out, "-\t");
                    break;
            }
        }
        fprintf(out, "\n");
    }

    fprintf(out, "\nLR(0) GOTO Table:\n");
    fprintf(out, "State\t");
    for (int nt = 0; nt < grammar->nonterminals_count; nt++) {
        fprintf(out, "%c\t", grammar->nonterminals[nt]);
    }
    fprintf(out, "\n");
    for (int s = 0; s < dfa->state_count; s++) {
        fprintf(out, "%d\t", s);
        for (int nt = 0; nt < grammar->nonterminals_count; nt++) {
            int target = table->go_to[s][nt];
            if (target >= 0) {
                fprintf(out, "%d\t", target);
            } else {
                fprintf(out, "-\t");
            }
        }
        fprintf(out, "\n");
    }
}
