#include "parse_utils.h"

int symbol_to_index(const Grammar* grammar, char symbol) {
    for (int i = 0; i < grammar->terminals_count; ++i)
        if (grammar->terminals[i] == symbol) return i;
    for (int i = 0; i < grammar->nonterminals_count; ++i)
        if (grammar->nonterminals[i] == symbol) return i;
    return -1;
}

bool is_terminal(const Grammar* grammar, char c) {
    for (int i = 0; i < grammar->terminals_count; ++i)
        if (grammar->terminals[i] == c) return true;
    return false;
}

bool is_nonterminal(const Grammar* grammar, char c) {
    for (int i = 0; i < grammar->nonterminals_count; ++i)
        if (grammar->nonterminals[i] == c) return true;
    return false;
}
