// lexer.h - 词法分析器头文件
// 定义 Token 类型、TokenType 枚举和词法分析相关接口

#ifndef LEXER_H
#define LEXER_H

#include "arena.h"

// TokenType 枚举，表示所有可能的词法单元类型
typedef enum {
    T_CHAR,      // 普通字符
    T_STAR,      // 星号 *
    T_EOF,       // 文件结束
    T_PLUS,      // 加号 +
    T_QUESTION,  // 问号 ?
    T_OR,        // 竖线 |
    T_LPAREN,    // 左括号 (
    T_RPAREN,    // 右括号 )
    T_DOT,       // 点号 .
    T_INVALID    // 表示无效或无法识别的字符
} TokenType;

// Token 结构体，表示一个词法单元
typedef struct {
    TokenType type; // 词法单元类型
    char value;     // 词法单元的原始字符
} Token;

// 词法分析主函数声明：从输入字符串 regex 中提取下一个 Token
Token* lexer_next_token(const char** regex, struct Arena* arena);

#endif // LEXER_H