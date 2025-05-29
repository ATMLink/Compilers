#define UNITY_BUILD // 启用 Unity Build

#include <stdio.h>
#include <stdlib.h> 

// Arena
#include "src/arena.h"      
#include "src/arena.c"

#include "src/grammar.h"
#include "src/grammar.c"

#include "src/viable_prefix_dfa.h"
#include "src/viable_prefix_dfa.c"

#include "src/lr0_parse_table.h"
#include "src/lr0_parse_table.c"

int main(){
    Arena* grammar_arena = arena_create(1024*1024);
    Arena* dfa_arena = arena_create(1024 * 1024);
    if (!grammar_arena || !dfa_arena) {
        fprintf(stderr, "Failed to create arena\n");
        if (grammar_arena) arena_free(grammar_arena);
        if (dfa_arena) arena_free(dfa_arena);
        return 1;
    }

    char filename[] = "grammar.txt";
    GrammarResultGrammar result =  read_grammar(filename, grammar_arena);
    if (result.status != GRAMMAR_OK) {
        fprintf(stderr, "Error: Failed to read grammar from file. Status code: %d\n", result.status);
        arena_free(grammar_arena);
        arena_free(dfa_arena);
        return 1;
    }

    Grammar* grammar = result.value;
    print_grammar(grammar);

    DFA dfa;
    build_viable_prefix_dfa(grammar, &dfa, dfa_arena);
    print_dfa(&dfa);

    // 构建并打印 LR(0) 解析表
    LR0ParseTable parse_table = {0};
    build_lr0_parse_table(grammar, &dfa, &parse_table);
    print_lr0_parse_table(grammar, &parse_table, &dfa, stdout);

    grammar_free(grammar);
    dfa_free(&dfa);
    arena_free(grammar_arena);
    arena_free(dfa_arena);
    return 0;
}