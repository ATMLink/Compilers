#ifndef MATCHER_H
#define MATCHER_H
#define ANY_CHAR -1

#include "nfa.h"

typedef struct StateList {
    State* state;
    struct StateList* next;
} StateList;

int simulate_nfa(State* start, const char* input, struct Arena* arena);

#endif