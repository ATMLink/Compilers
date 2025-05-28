#define UNITY_BUILD // 启用 Unity Build
#include <stdio.h>

#include "../src/lexer.h"
#include "../src/lexer.c"

#include "../src/arena.h"
#include "../src/arena.c"

// int main(){
//     struct Arena* arena = arena_create(64);
//     Token* token;
//     const char regexp_1[] = " we*a  ";
//     const char* ptr_1 = regexp_1;  // 创建一个指针
//     while ((token = lexer_next_token(&ptr_1, arena))->type != T_EOF)
//     {
//         switch (token->type) {
//             case T_CHAR:
//                 printf("token type=T_CHAR, value=%c\n", token->value);
//                 break;
//             case T_STAR:
//                 printf("token type=T_STAR, value=%c\n", token->value);
//                 break;
//             case T_INVALID:
//                 printf("token type=T_INVALID, value=%c\n", token->value);
//                 break;
//             case T_EOF:
//                 printf("token type=T_EOF\n");
//                 break;
//         }
//     }
//     arena_free(arena);
//     return 0;
// }
void test_lexer(const char* regex) {
    printf("Testing lexer for: \"%s\"\n", regex);

    struct Arena* arena = arena_create(64);
    Token* token;
    const char* ptr = regex;

    while ((token = lexer_next_token(&ptr, arena))->type != T_EOF) {
        switch (token->type) {
            case T_CHAR:
                printf("  Token: T_CHAR     '%c'\n", token->value); break;
            case T_STAR:
                printf("  Token: T_STAR     '*'\n"); break;
            case T_OR:
                printf("  Token: T_OR     '|'\n"); break;
            case T_PLUS:
                printf("  Token: T_PLUS    '+'\n"); break;
            case T_QUESTION:
                printf("  Token: T_QUESTION '?' \n"); break;
            case T_LPAREN:
                printf("  Token: T_LPAREN   '('\n"); break;
            case T_RPAREN:
                printf("  Token: T_RPAREN   ')'\n"); break;
            case T_DOT:
                printf("  Token: T_DOT      '.'\n"); break;
            case T_INVALID:
                printf("  Token: T_INVALID  '%c'\n", token->value); break;
            default:
                printf("  Token: UNKNOWN\n"); break;
        }
    }

    printf("  Token: T_EOF\n\n");
    arena_free(arena);
}


int main() {
    test_lexer("a");
    test_lexer("a*");
    test_lexer("ab");
    test_lexer("a.b");
    test_lexer("a+b");
    test_lexer("a?b");
    test_lexer("a|b");
    test_lexer("(ab)*");
    test_lexer("a(b|c)*d+e?f.g");

    return 0;
}