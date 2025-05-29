// semantic_stack.h
#include "semantic_stack.h"
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 1000

static SemanticSymbol stack[MAX_STACK_SIZE];
static int top = -1;

void push_semantic(const char* symbol, const char* value) {
    if (top >= MAX_STACK_SIZE - 1) return;
    top++;
    stack[top].symbol = strdup(symbol);
    stack[top].value = strdup(value);
}

SemanticSymbol pop_semantic() {
    SemanticSymbol empty = {NULL, NULL};
    if (top < 0) return empty;
    SemanticSymbol s = stack[top--];
    return s;
}

SemanticSymbol top_semantic() {
    SemanticSymbol empty = {NULL, NULL};
    if (top < 0) return empty;
    return stack[top];
}

void clear_semantic_stack() {
    while (top >= 0) {
        free(stack[top].symbol);
        free(stack[top].value);
        top--;
    }
}
