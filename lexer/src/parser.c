// parser.c - 正则表达式递归下降语法分析器实现
// 负责将正则表达式字符串解析为 NFA（非确定性有限自动机）
// 采用递归下降方法，支持 |、*、+、?、()、. 等正则语法

#include "parser.h"
#include "lexer.h"
#include <stdio.h>

// 预声明递归子程序，分别对应正则文法的各个非终结符
static NFA parse_expr(const char** p, struct Arena* arena);    // expr ::= term ('|' term)*
static NFA parse_term(const char** p, struct Arena* arena);    // term ::= factor+
static NFA parse_factor(const char** p, struct Arena* arena);  // factor ::= base ('*'|'+'|'?')*
static NFA parse_base(const char** p, struct Arena* arena);    // base ::= CHAR | '.' | '(' expr ')'

// 入口函数：解析正则表达式字符串，返回 NFA 的起始状态指针
State* parse_regex(const char* regex, struct Arena* arena) {
    const char* p = regex; // 输入指针
    NFA result = parse_expr(&p, arena); // 递归下降解析
    result.accept->is_accepting = 1;    // 标记接受状态
    return result.start;                // 返回 NFA 起始状态
}

// expr ::= term ('|' term)*
// 支持正则中的或运算，左递归消除
static NFA parse_expr(const char** p, struct Arena* arena) {
    NFA left = parse_term(p, arena); // 先解析第一个 term
    while (1) {
        const char* backup = *p;     // 备份指针，便于回溯
        Token* tok = lexer_next_token(p, arena);
        if (tok->type == T_OR) {
            // 匹配到 |，递归解析右侧 term
            NFA right = parse_term(p, arena);
            left = create_or_nfa(arena, left, right); // 合并为 or NFA
        } else {
            *p = backup; // 回溯
            break;
        }
    }
    return left;
}

// term ::= factor+
// 支持正则中的串联，至少一个 factor
static NFA parse_term(const char** p, struct Arena* arena) {
    NFA result = parse_factor(p, arena); // 先解析第一个 factor
    while (1) {
        const char* backup = *p;
        Token* tok = lexer_next_token(p, arena);

        // 如果遇到 |、) 或结束，说明 term 结束
        if (tok->type == T_OR || tok->type == T_RPAREN || tok->type == T_EOF) {
            *p = backup;
            break;
        }

        *p = backup;
        NFA next = parse_factor(p, arena); // 继续解析下一个 factor
        result = create_concat_nfa(arena, result, next); // 串联 NFA
    }
    return result;
}

// factor ::= base ('*' | '+' | '?')*
// 支持正则中的闭包、正闭包、可选
static NFA parse_factor(const char** p, struct Arena* arena) {
    NFA base = parse_base(p, arena); // 先解析 base
    while (1) {
        const char* backup = *p;
        Token* tok = lexer_next_token(p, arena);
        if (tok->type == T_STAR) {
            base = create_star_nfa(arena, base); // * 闭包
        } else if (tok->type == T_PLUS) {
            base = create_plus_nfa(arena, base); // + 正闭包
        } else if (tok->type == T_QUESTION) {
            base = create_question_nfa(arena, base); // ? 可选
        } else {
            *p = backup;
            break;
        }
    }
    return base;
}

// base ::= CHAR | '.' | '(' expr ')'
// 支持普通字符、任意字符、括号分组
static NFA parse_base(const char** p, struct Arena* arena) {
    Token* tok = lexer_next_token(p, arena);
    if (tok->type == T_CHAR) {
        return create_char_nfa(arena, tok->value); // 普通字符
    } else if (tok->type == T_DOT) {
        return create_dot_nfa(arena); // 任意字符
    } else if (tok->type == T_LPAREN) {
        NFA inner = parse_expr(p, arena); // 括号内递归解析
        Token* close = lexer_next_token(p, arena);
        if (close->type != T_RPAREN) {
            // 括号不匹配，报错退出
            fprintf(stderr, "Error: expected ')' but got something else.\n");
            exit(1);
        }
        return inner;
    } else {
        // 非法 token，报错退出
        fprintf(stderr, "Error: unexpected token in base.\n");
        exit(1);
    }
}