#include "nfa.h"
#include <stdlib.h>

State* create_state(struct Arena* arena) {
    State* state = (State*)arena_alloc(arena, sizeof(state));
    state -> is_accepting = 0;
    state -> transitions = NULL;
    return state;
}

void add_transition(struct Arena* arena, State* from, char symbol, State* to){
    Transition* transition = (Transition*)arena_alloc(arena, sizeof(Transition));
    transition -> symbol = symbol;
    transition -> target = to;
    transition -> next = from -> transitions;
    from -> transitions = transition;
}