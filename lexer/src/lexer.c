// lexer.c - 词法分析器实现
// 负责将输入正则表达式字符串分割为 Token 流，供后续语法分析使用

#include <stdio.h>
#include <ctype.h>
#include "lexer.h"

// 词法分析主函数：从输入字符串 regex 中提取下一个 Token
// 参数 regex: 指向正则表达式字符串的指针指针
// 参数 arena: 内存分配器，用于分配 Token 结构体
// 返回: 新分配的 Token 指针，或遇到错误/结束时返回 NULL
Token* lexer_next_token(const char** regex, struct Arena* arena) {
    // 参数有效性检查，防止空指针
    if (regex == NULL || *regex == NULL || arena == NULL) {
        return NULL;
    }

    // 跳过所有空白字符，支持灵活的正则输入格式
    while (isspace(**regex)) {
        (*regex)++;
    }

    // 分配新 Token 结构体，所有 Token 都由 arena 管理
    Token* token = (Token*)arena_alloc(arena, sizeof(Token));
    if (!token) {
        return NULL;
    }

    // 取当前字符，作为本次 Token 的内容
    char c = **regex;
    if (c == '\0') {
        // 输入结束，返回 EOF Token
        token->type = T_EOF;
        token->value = c;
        return token;
    }

    // 根据当前字符判断 Token 类型
    switch (c) {
        case '+':
            token->type = T_PLUS;      // 加号
            break;
        case '?':
            token->type = T_QUESTION;  // 问号
            break;
        case '|':
            token->type = T_OR;        // 或运算符
            break;
        case '(': 
            token->type = T_LPAREN;    // 左括号
            break;
        case ')':
            token->type = T_RPAREN;    // 右括号
            break;
        case '.':
            token->type = T_DOT;       // 点号，正则中的任意字符
            break;
        case '*':
            token->type = T_STAR;      // 星号，正则中的闭包
            break;
        default:
            if (isalnum(c)) {
                token->type = T_CHAR;  // 普通字母或数字字符
            } else {
                token->type = T_INVALID; // 非法字符，无法识别
            }
            break;
    }

    // 记录 Token 的原始字符
    token->value = c;
    // 指针前移，准备下一个 Token
    (*regex)++;
    return token;
}