// semantic_stack.h
#ifndef SEMANTIC_STACK_H
#define SEMANTIC_STACK_H

typedef struct {
    char* symbol;     // 非终结符或终结符
    char* value;      // 中间变量名、常量、变量名等语义值
} SemanticSymbol;

void push_semantic(const char* symbol, const char* value);
SemanticSymbol pop_semantic();
SemanticSymbol top_semantic();
void clear_semantic_stack();

#endif
