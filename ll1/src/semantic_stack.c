// semantic_stack.c - 语义栈实现
// 负责 LL(1) 语法分析过程中的语义动作支持
// 包含语义栈的基本操作：压栈、弹栈、查看栈顶元素和清空栈

#include "semantic_stack.h"
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 1000

// 定义语义符号结构体数组作为栈
static SemanticSymbol stack[MAX_STACK_SIZE];
// 栈顶指针，初始化为-1表示栈为空
static int top = -1;

/**
 * @brief 压入语义符号到栈顶
 * 
 * @param symbol 语义符号
 * @param value  语义值
 */
void push_semantic(const char* symbol, const char* value) {
    // 如果栈已满，返回
    if (top >= MAX_STACK_SIZE - 1) return;
    top++;
    // 复制符号和数值字符串到新分配的内存
    stack[top].symbol = strdup(symbol);
    stack[top].value = strdup(value);
}

/**
 * @brief 弹出栈顶的语义符号
 * 
 * @return SemanticSymbol 弹出的语义符号，若栈为空则返回空的SemanticSymbol
 */
SemanticSymbol pop_semantic() {
    SemanticSymbol empty = {NULL, NULL};
    // 如果栈为空，返回空的SemanticSymbol
    if (top < 0) return empty;
    // 获取栈顶元素
    SemanticSymbol s = stack[top--];
    return s;
}

/**
 * @brief 获取栈顶的语义符号而不弹出
 * 
 * @return SemanticSymbol 当前栈顶的语义符号，若栈为空则返回空的SemanticSymbol
 */
SemanticSymbol top_semantic() {
    SemanticSymbol empty = {NULL, NULL};
    // 如果栈为空，返回空的SemanticSymbol
    if (top < 0) return empty;
    return stack[top];
}

/**
 * @brief 清空语义栈
 * 
 * 释放栈中所有语义符号占用的内存
 */
void clear_semantic_stack() {
    while (top >= 0) {
        free(stack[top].symbol);
        free(stack[top].value);
        top--;
    }
}
