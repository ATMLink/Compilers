// nfa.c - 非确定性有限自动机（NFA）实现
// 负责 NFA 的构建、状态转移和正则表达式到 NFA 的转换

#include "nfa.h"
#include <stdlib.h>
#include <stdio.h>

// 创建一个新的状态
State* create_state(struct Arena* arena) {
    State* s = (State*)arena_alloc(arena, sizeof(State));
    if (!s) {
        fprintf(stderr, "Failed to allocate state\n");
        return NULL;
    }
    s->is_accepting = 0;
    s->transitions = NULL;
    return s;
}

// 添加状态转移
void add_transition(struct Arena* arena, State* from, char symbol, State* to) {
    if (!from || !to) return;
    Transition* t = (Transition*)arena_alloc(arena, sizeof(Transition));
    if (!t) {
        fprintf(stderr, "Failed to allocate transition\n");
        return;
    }
    t->symbol = symbol;
    t->target = to;
    t->next = from->transitions;
    from->transitions = t;
}

// 创建单字符 NFA
NFA create_char_nfa(struct Arena* arena, char c) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    if (!start || !accept) return (NFA){NULL, NULL};
    add_transition(arena, start, c, accept);
    if (!start->transitions) return (NFA){NULL, NULL};
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建连接 NFA
NFA create_concat_nfa(struct Arena* arena, NFA a, NFA b) {
    add_transition(arena, a.accept, '\0', b.start);
    a.accept->is_accepting = 0;
    return (NFA){a.start, b.accept};
}

// 创建 Kleene 星号 NFA
NFA create_star_nfa(struct Arena* arena, NFA inner) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', inner.start);
    add_transition(arena, start, '\0', accept);
    add_transition(arena, inner.accept, '\0', inner.start);
    add_transition(arena, inner.accept, '\0', accept);
    inner.accept->is_accepting = 0;
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建选择 NFA
NFA create_or_nfa(struct Arena* arena, NFA a, NFA b) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', a.start);
    add_transition(arena, start, '\0', b.start);
    add_transition(arena, a.accept, '\0', accept);
    add_transition(arena, b.accept, '\0', accept);
    a.accept->is_accepting = 0;
    b.accept->is_accepting = 0;
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建点号 NFA
// NFA create_dot_nfa(struct Arena* arena) {
//     State* start = create_state(arena);
//     State* accept = create_state(arena);
//     if (!start || !accept) return (NFA){NULL, NULL};
//     add_transition(arena, start, '.', accept);
//     if (!start->transitions) return (NFA){NULL, NULL};
//     accept->is_accepting = 1;
//     return (NFA){start, accept};
// }
NFA create_dot_nfa(struct Arena* arena) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, ANY_CHAR, accept);
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建加号 NFA
NFA create_plus_nfa(struct Arena* arena, NFA inner) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', inner.start);
    add_transition(arena, inner.accept, '\0', inner.start);
    add_transition(arena, inner.accept, '\0', accept);
    inner.accept->is_accepting = 0;
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建问号 NFA
NFA create_question_nfa(struct Arena* arena, NFA inner) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', inner.start);
    add_transition(arena, start, '\0', accept);
    add_transition(arena, inner.accept, '\0', accept);
    inner.accept->is_accepting = 0;
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建左括号 NFA
NFA create_left_paren_nfa(struct Arena* arena) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', accept);
    accept->is_accepting = 1;
    return (NFA){start, accept};
}

// 创建右括号 NFA
NFA create_right_paren_nfa(struct Arena* arena) {
    State* start = create_state(arena);
    State* accept = create_state(arena);
    add_transition(arena, start, '\0', accept);
    accept->is_accepting = 1;
    return (NFA){start, accept};
}