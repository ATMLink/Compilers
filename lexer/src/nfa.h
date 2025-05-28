// nfa.h
#ifndef NFA_H
#define NFA_H

#define ANY_CHAR -1 // 用于表示任意字符的转移

#include "arena.h"

typedef struct State State;
typedef struct Transition Transition;

// 转移结构体
struct Transition {
    char symbol; // '\0' for epsilon
    State* target;
    Transition* next;
};

// 状态结构体
typedef struct State {
    int is_accepting;
    Transition* transitions;
} State;

// 非确定性有限自动机（NFA）结构体
typedef struct {
    State* start;
    State* accept;
} NFA;

// 函数声明
State* create_state(struct Arena* arena);
void add_transition(struct Arena* arena, State* from, char symbol, State* to);

// NFA 创建函数
NFA create_char_nfa(struct Arena* arena, char c);
NFA create_concat_nfa(struct Arena* arena, NFA a, NFA b);
NFA create_star_nfa(struct Arena* arena, NFA inner);
NFA create_or_nfa(struct Arena* arena, NFA a, NFA b);
NFA create_dot_nfa(struct Arena* arena);
NFA create_plus_nfa(struct Arena* arena, NFA inner);
NFA create_question_nfa(struct Arena* arena, NFA inner);
NFA create_left_paren_nfa(struct Arena* arena);
NFA create_right_paren_nfa(struct Arena* arena);

#endif