// first_follow.c - First/Follow 集合相关实现
// 负责符号集的查找、创建和字符集操作

#include "first_follow.h"
#include "grammar.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

SymbolSet* get_or_create_set(SymbolSet* sets, 
                             int* count, 
                             char symbol, 
                             Arena* arena)
{
    // symbol对应的表如果存在就返回这个表
    for(int i = 0; i < *count; i++){
        if(sets[i].symbol == symbol) return &sets[i];
    }
    // symbol对应的表不存在，则创建新表
    SymbolSet* set = &sets[(*count)++];
    set->symbol = symbol;
    set->first = arena_alloc(arena, GRAMMAR_MAX_SYMBOLS);
    set->follow = arena_alloc(arena, GRAMMAR_MAX_SYMBOLS);
    if(!set->first || !set->follow){
        fprintf(stderr, "Error: Failed to allocate memory for First or Follow.\n");
        return NULL; 
    }
    set->first[0] = '\0';
    set->follow[0] = '\0';
    return set;
}

bool add_char(char* set, char c){
    if(strchr(set, c)) return false;
    int len = strlen(set);
    set[len] = c;
    set[len+1] = '\0';
    return true;
}

