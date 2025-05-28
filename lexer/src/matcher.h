#ifndef MATCHER_H
#define MATCHER_H
#define ANY_CHAR -1

#include "nfa.h"

int simulate_nfa(State* start, const char* input, struct Arena* arena);

#endif