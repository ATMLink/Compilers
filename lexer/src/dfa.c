#include "dfa.h"
#include <stdio.h>
#include <string.h>

/**
 * 创建一个新的 DFA 状态，使用 Arena 分配内存。
 * 初始化该状态为非接受态，清空所有转换表和关联的 NFA 状态列表。
 */
static DFAState* create_dfa_state(struct Arena* arena) {
    DFAState* s = (DFAState*)arena_alloc(arena, sizeof(DFAState));
    if (!s) {
        fprintf(stderr, "Failed to allocate DFA state\n");
        return NULL;
    }
    s->is_accepting = 0;                     // 默认不是接受状态
    s->nfa_states = NULL;                    // 初始化为空的 NFA 状态集合
    memset(s->transitions, 0, sizeof(DFAState*) * MAX_ALPHABET);  // 所有字母转移初始化为空
    return s;
}

/**
 * 检查某个 NFA 状态是否在给定的状态列表中。
 * 用于集合判等和闭包处理。
 */
static int state_list_contains(StateList* list, State* state) {
    for (StateList* node = list; node; node = node->next) {
        if (node->state == state) return 1;
    }
    return 0;
}

/**
 * 判断两个状态集合是否相等（即包含相同的 NFA 状态）。
 * 这个用于检查某个 NFA 状态集合是否已经存在于 DFA 状态列表中。
 */
static int state_lists_equal(StateList* a, StateList* b) {
    for (StateList* node_a = a; node_a; node_a = node_a->next) {
        if (!state_list_contains(b, node_a->state)) return 0;
    }
    for (StateList* node_b = b; node_b; node_b = node_b->next) {
        if (!state_list_contains(a, node_b->state)) return 0;
    }
    return 1;
}

/**
 * 查找是否已有包含相同 NFA 状态集合的 DFA 状态。
 * 如果存在，返回该 DFA 状态；否则返回 NULL。
 */
static DFAState* find_dfa_state(DFA* dfa, StateList* nfa_states) {
    for (int i = 0; i < dfa->state_count; i++) {
        if (state_lists_equal(dfa->states[i]->nfa_states, nfa_states)) {
            return dfa->states[i];
        }
    }
    return NULL;
}

/**
 * 从 NFA 构建 DFA（子集构造法）。
 * 使用 Arena 管理所有内存分配。
 */
DFA* create_dfa_from_nfa(struct Arena* arena, NFA nfa) {
    // 分配 DFA 结构体
    DFA* dfa = (DFA*)arena_alloc(arena, sizeof(DFA));
    if (!dfa) {
        fprintf(stderr, "Failed to allocate DFA\n");
        return NULL;
    }

    // 分配 DFA 状态数组
    dfa->states = (DFAState**)arena_alloc(arena, sizeof(DFAState*) * MAX_DFA_STATES);
    if (!dfa->states) {
        fprintf(stderr, "Failed to allocate DFA states array\n");
        return NULL;
    }
    dfa->state_count = 0;

    // 创建 DFA 起始状态（为 NFA 起始状态的 ε 闭包）
    DFAState* start = create_dfa_state(arena);
    if (!start) return NULL;
    epsilon_closure(arena, &start->nfa_states, nfa.start);
    start->is_accepting = state_list_contains(start->nfa_states, nfa.accept);  // 判断是否为接受状态
    dfa->states[dfa->state_count++] = start;
    dfa->start = start;

    // 子集构造算法主循环
    for (int i = 0; i < dfa->state_count; i++) {
        DFAState* current = dfa->states[i];

        // 对每一个输入符号进行转移计算
        for (int c = 0; c < MAX_ALPHABET; c++) {
            StateList* next_nfa_states = NULL;

            // 遍历当前 DFA 状态对应的 NFA 状态集合
            for (StateList* node = current->nfa_states; node; node = node->next) {
                for (Transition* t = node->state->transitions; t; t = t->next) {
                    if (t->symbol == c || t->symbol == ANY_CHAR) {
                        epsilon_closure(arena, &next_nfa_states, t->target);
                    }
                }
            }

            // 如果存在转移状态集合
            if (next_nfa_states) {
                DFAState* next_dfa_state = find_dfa_state(dfa, next_nfa_states);

                // 若该状态集合尚不存在，创建新状态
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

                // 设置 DFA 转移
                current->transitions[c] = next_dfa_state;
            }
        }
    }

    return dfa;
}

/**
 * 模拟运行 DFA，输入为字符串 input。
 * 若最终落在接受状态，返回 1，否则返回 0。
 */
int simulate_dfa(DFA* dfa, const char* input) {
    if (!dfa || !dfa->start) return 0;
    DFAState* current = dfa->start;

    for (const char* p = input; *p; p++) {
        current = current->transitions[(unsigned char)*p];
        if (!current) return 0; // 没有对应转移，匹配失败
    }

    return current->is_accepting;
}

/**
 * 使用 Arena 分配内存，无需手动释放。
 * 此函数存在是为了接口统一，实际中无操作。
 */
void free_dfa(struct Arena* arena, DFA* dfa) {
    // Arena-based memory management, no explicit free needed
}
