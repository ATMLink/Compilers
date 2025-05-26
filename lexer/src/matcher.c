#include "matcher.h"
#include <string.h>

typedef struct StateList {
    State* state;
    struct StateList* next;
} StateList;

static void add_state(struct Arena* arena, StateList** list, State* state) {
    
}