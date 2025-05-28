#include "parser.h"
#include "lexer.h"
#include <stdio.h>

// 预声明
static NFA parse_expr(const char** p, struct Arena* arena);
static NFA parse_term(const char** p, struct Arena* arena);
static NFA parse_factor(const char** p, struct Arena* arena);
static NFA parse_base(const char** p, struct Arena* arena);

State* parse_regex(const char* regex, struct Arena* arena) {
    const char* p = regex;
    NFA result = parse_expr(&p, arena);
    result.accept->is_accepting = 1;
    return result.start;
}

// expr ::= term ('|' term)*
static NFA parse_expr(const char** p, struct Arena* arena) {
    NFA left = parse_term(p, arena);
    while (1) {
        const char* backup = *p;
        Token* tok = lexer_next_token(p, arena);
        if (tok->type == T_OR) {
            NFA right = parse_term(p, arena);
            left = create_or_nfa(arena, left, right);
        } else {
            *p = backup;
            break;
        }
    }
    return left;
}

// term ::= factor+
static NFA parse_term(const char** p, struct Arena* arena) {
    NFA result = parse_factor(p, arena);
    while (1) {
        const char* backup = *p;
        Token* tok = lexer_next_token(p, arena);

        // 如果是下一个起始符或结尾，返回
        if (tok->type == T_OR || tok->type == T_RPAREN || tok->type == T_EOF) {
            *p = backup;
            break;
        }

        *p = backup;
        NFA next = parse_factor(p, arena);
        result = create_concat_nfa(arena, result, next);
    }
    return result;
}

// factor ::= base ('*' | '+' | '?')*
static NFA parse_factor(const char** p, struct Arena* arena) {
    NFA base = parse_base(p, arena);
    while (1) {
        const char* backup = *p;
        Token* tok = lexer_next_token(p, arena);
        if (tok->type == T_STAR) {
            base = create_star_nfa(arena, base);
        } else if (tok->type == T_PLUS) {
            base = create_plus_nfa(arena, base);
        } else if (tok->type == T_QUESTION) {
            base = create_question_nfa(arena, base);
        } else {
            *p = backup;
            break;
        }
    }
    return base;
}

// base ::= CHAR | '.' | '(' expr ')'
static NFA parse_base(const char** p, struct Arena* arena) {
    Token* tok = lexer_next_token(p, arena);
    if (tok->type == T_CHAR) {
        return create_char_nfa(arena, tok->value);
    } else if (tok->type == T_DOT) {
        return create_dot_nfa(arena);
    } else if (tok->type == T_LPAREN) {
        NFA inner = parse_expr(p, arena);
        Token* close = lexer_next_token(p, arena);
        if (close->type != T_RPAREN) {
            // 括号不匹配
            fprintf(stderr, "Error: expected ')' but got something else.\n");
            exit(1);
        }
        return inner;
    } else {
        fprintf(stderr, "Error: unexpected token in base.\n");
        exit(1);
    }
}
