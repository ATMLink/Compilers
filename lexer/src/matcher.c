// matcher.c - 正则表达式匹配器实现
// 负责将 DFA/NFA 应用于输入字符串，实现正则 匹配功能

#include "matcher.h"
#include <string.h>

typedef struct StateList {
    State* state;
    struct StateList* next;
} StateList;

static void add_state(struct Arena* arena, StateList** list, State* s) {
    for (StateList* p = *list; p; p = p->next)
        if (p->state == s) return;

    StateList* node = (StateList*)arena_alloc(arena, sizeof(StateList));
    node->state = s;
    node->next = *list;
    *list = node;
}

// epsilon_closure - 计算状态 s 的 ε 闭包
// 将所有通过 ε 转换可达的状态添加到状态列表 list 中
static void epsilon_closure(struct Arena* arena, StateList** list, State* s) {
    add_state(arena, list, s);
    for (Transition* t = s->transitions; t; t = t->next)
        if (t->symbol == '\0')
            epsilon_closure(arena, list, t->target);
}

// simulate_nfa - 使用 NFA 模拟器模拟状态转移
// 从初始状态 start 开始，处理输入字符串 input
// 返回 1 表示匹配成功，返回 0 表示匹配失败
int simulate_nfa(State* start, const char* input, struct Arena* arena) {
    StateList* current = NULL;
    epsilon_closure(arena, &current, start);

    for (const char* p = input; *p; ++p) {
        StateList* next = NULL;
        for (StateList* node = current; node; node = node->next) {
            for (Transition* t = node->state->transitions; t; t = t->next) {
                if (t->symbol == *p || t->symbol == ANY_CHAR) {
                    epsilon_closure(arena, &next, t->target);
                }
            }
        }
        current = next;
    }

    for (StateList* node = current; node; node = node->next)
        if (node->state->is_accepting)
            return 1;

    return 0;
}