// lexer.c
#include <stdio.h>
#include <ctype.h>
#include "lexer.h"

Token* lexer_next_token(const char** regex, 
                        struct Arena* arena) 
{
    // 检查输入参数是否有效
    if(regex == NULL || *regex == NULL || arena == NULL){
        return NULL;
    }

    // 跳过空白字符
    while(isSpace(**regex)){
        (*regex)++;
    }

    // 分配一个新的 Token
    // 这里使用 arena_alloc 函数从内存池中分配内存
    Token* token = (Token*)arena_alloc(arena, sizeof(Token));
    if(!token){
        return NULL; // 内存分配失败
    }

    char c = **regex; // 获取当前字符
    if(c == '\0'){
        token->type = T_EOF; // 到达字符串末尾
        return token;
    }else if(isalnum(c)){
        token->type = T_CHAR; // 字符类型
        token->value = c; // 保存字符值
        (*regex)++; // 移动到下一个字符
    }else if(c == '*'){
        token->type = T_STAR; // 星号类型
        token->value = '*'; // 保存星号值
        (*regex)++; // 移动到下一个字符
    }else{
        token->type = T_INVALID; // 无效类型
        token->value = c; // 保存无效字符值
        (*regex)++; // 消费无效字符以避免无限循环
    }
    return token; // 返回解析的 Token
}