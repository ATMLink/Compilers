// lexer.c
#include <stdio.h>
#include <ctype.h>
#include "lexer.h"

Token* lexer_next_token(const char** regex, struct Arena* arena) {
    if (regex == NULL || *regex == NULL || arena == NULL) {
        return NULL;
    }

    while (isspace(**regex)) {
        (*regex)++;
    }

    Token* token = (Token*)arena_alloc(arena, sizeof(Token));
    if (!token) {
        return NULL;
    }

    char c = **regex;
    if (c == '\0') {
        token->type = T_EOF;
        token->value = c;
        return token;
    }

    switch (c) {
        case '+':
            token->type = T_PLUS;
            break;
        case '?':
            token->type = T_QUESTION;
            break;
        case '|':
            token->type = T_OR;
            break;
        case '(':
            token->type = T_LPAREN;
            break;
        case ')':
            token->type = T_RPAREN;
            break;
        case '.':
            token->type = T_DOT;
            break;
        case '*':
            token->type = T_STAR;
            break;
        default:
            if (isalnum(c)) {
                token->type = T_CHAR;
            } else {
                token->type = T_INVALID;
            }
            break;
    }

    token->value = c;
    (*regex)++; // consume current character
    return token;
}