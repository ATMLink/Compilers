#ifndef DFA_H
#define DFA_H

#include "nfa.h"
#include "arena.h"
#include "matcher.h"

#define MAX_DFA_STATES 256
#define MAX_ALPHABET 256 // 假设 ASCII 字符集

typedef struct DFAState DFAState;
typedef struct DFA DFA;

struct DFAState {
    int is_accepting; // 是否为接受状态
    DFAState* transitions[MAX_ALPHABET]; // 每个字符的转移目标
    StateList* nfa_states; // 对应的 NFA 状态集（用于子集构造）
};

struct DFA {
    DFAState* start; // 起始状态
    DFAState** states; // 所有状态的数组
    int state_count; // 状态数量
};

DFA* create_dfa_from_nfa(struct Arena* arena, NFA nfa);
int simulate_dfa(DFA* dfa, const char* input);
void free_dfa(struct Arena* arena, DFA* dfa);

#endif