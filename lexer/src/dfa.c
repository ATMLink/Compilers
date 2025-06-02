#include "dfa.h"
#include <stdio.h>
#include <string.h>

static DFAState* create_dfa_state(struct Arena* arena) {
    DFAState* s = (DFAState*)arena_alloc(arena, sizeof(DFAState));
    if (!s) {
        fprintf(stderr, "Failed to allocate DFA state\n");
        return NULL;
    }
    s->is_accepting = 0;
    s->nfa_states = NULL;
    memset(s->transitions, 0, sizeof(DFAState*) * MAX_ALPHABET);
    return s;
}

static int state_list_contains(StateList* list, State* state) {
    for (StateList* node = list; node; node = node->next) {
        if (node->state == state) return 1;
    }
    return 0;
}

static int state_lists_equal(StateList* a, StateList* b) {
    for (StateList* node_a = a; node_a; node_a = node_a->next) {
        if (!state_list_contains(b, node_a->state)) return 0;
    }
    for (StateList* node_b = b; node_b; node_b = node_b->next) {
        if (!state_list_contains(a, node_b->state)) return 0;
    }
    return 1;
}

static DFAState* find_dfa_state(DFA* dfa, StateList* nfa_states) {
    for (int i = 0; i < dfa->state_count; i++) {
        if (state_lists_equal(dfa->states[i]->nfa_states, nfa_states)) {
            return dfa->states[i];
        }
    }
    return NULL;
}

DFA* create_dfa_from_nfa(struct Arena* arena, NFA nfa) {
    DFA* dfa = (DFA*)arena_alloc(arena, sizeof(DFA));
    if (!dfa) {
        fprintf(stderr, "Failed to allocate DFA\n");
        return NULL;
    }
    dfa->states = (DFAState**)arena_alloc(arena, sizeof(DFAState*) * MAX_DFA_STATES);
    if (!dfa->states) {
        fprintf(stderr, "Failed to allocate DFA states array\n");
        return NULL;
    }
    dfa->state_count = 0;

    // 初始化 DFA 起始状态
    DFAState* start = create_dfa_state(arena);
    if (!start) return NULL;
    epsilon_closure(arena, &start->nfa_states, nfa.start);
    start->is_accepting = state_list_contains(start->nfa_states, nfa.accept);
    dfa->states[dfa->state_count++] = start;
    dfa->start = start;

    // 子集构造法
    for (int i = 0; i < dfa->state_count; i++) {
        DFAState* current = dfa->states[i];
        for (int c = 0; c < MAX_ALPHABET; c++) {
            StateList* next_nfa_states = NULL;
            for (StateList* node = current->nfa_states; node; node = node->next) {
                for (Transition* t = node->state->transitions; t; t = t->next) {
                    if (t->symbol == c || t->symbol == ANY_CHAR) {
                        epsilon_closure(arena, &next_nfa_states, t->target);
                    }
                }
            }
            if (next_nfa_states) {
                DFAState* next_dfa_state = find_dfa_state(dfa, next_nfa_states);
                if (!next_dfa_state) {
                    if (dfa->state_count >= MAX_DFA_STATES) {
                        fprintf(stderr, "Too many DFA states\n");
                        return NULL;
                    }
                    next_dfa_state = create_dfa_state(arena);
                    if (!next_dfa_state) return NULL;
                    next_dfa_state->nfa_states = next_nfa_states;
                    next_dfa_state->is_accepting = state_list_contains(next_nfa_states, nfa.accept);
                    dfa->states[dfa->state_count++] = next_dfa_state;
                }
                current->transitions[c] = next_dfa_state;
            }
        }
    }

    return dfa;
}

int simulate_dfa(DFA* dfa, const char* input) {
    if (!dfa || !dfa->start) return 0;
    DFAState* current = dfa->start;

    for (const char* p = input; *p; p++) {
        current = current->transitions[(unsigned char)*p];
        if (!current) return 0; // 无有效转移，匹配失败
    }

    return current->is_accepting;
}

void free_dfa(struct Arena* arena, DFA* dfa) {
    // Arena-based memory management, no explicit free needed
}