// semantic_stack.h - 语义栈头文件
// 定义 LL(1) 语法分析过程中的语义栈相关接口

#ifndef SEMANTIC_STACK_H
#define SEMANTIC_STACK_H

// 语义符号类型，包含符号及其对应的语义值
typedef struct {
    char* symbol;     // 非终结符或终结符
    char* value;      // 中间变量名、常量、变量名等语义值
} SemanticSymbol;

// 将符号及其语义值压入语义栈
void push_semantic(const char* symbol, const char* value);

// 弹出语义栈顶元素
SemanticSymbol pop_semantic();

// 获取语义栈顶元素但不弹出
SemanticSymbol top_semantic();

// 清空语义栈
void clear_semantic_stack();

#endif
