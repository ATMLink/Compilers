// atmlink/compilers/Compilers-7e07cfdaca0ac296ef0b06750c1d6dff49f71d74/lr0/main.c

#define UNITY_BUILD // 启用 Unity Build

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

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
    // 为文法和DFA创建独立的Arena
    Arena* grammar_arena = arena_create(1024*1024);
    Arena* dfa_arena = arena_create(1024 * 1024);
    // 为分析表也创建一个独立的Arena
    Arena* table_arena = arena_create(1024*1024);

    if (!grammar_arena || !dfa_arena || !table_arena) {
        fprintf(stderr, "Failed to create arena\n");
        if (grammar_arena) arena_free(grammar_arena);
        if (dfa_arena) arena_free(dfa_arena);
        if (table_arena) arena_free(table_arena);
        return 1;
    }

    char filename[] = "grammar.txt";
    GrammarResultGrammar result =  read_grammar(filename, grammar_arena);
    if (result.status != GRAMMAR_OK) {
        fprintf(stderr, "Error: Failed to read grammar from file. Status code: %d\n", result.status);
        goto cleanup;
    }

    Grammar* grammar = result.value;
    print_grammar(grammar);

    DFA dfa;
    build_viable_prefix_dfa(grammar, &dfa, dfa_arena);
    print_dfa(&dfa);

    // 构建并打印 LR(0) 解析表
    LR0ParseTable parse_table;
    // *** 修改：传入 table_arena 用于哈希表内存分配 ***
    bool build_ok = build_lr0_parse_table(grammar, &dfa, &parse_table, table_arena);
    print_lr0_parse_table(&parse_table, stdout);

    // 如果分析表构建成功（无冲突），则启动分析驱动程序
    if (build_ok) {
        printf("\nParse table built successfully. The grammar is LR(0).\n");
        
        const char* input_string_1 = "(i+i)";
        lr0_parse_input(&parse_table, input_string_1);

        const char* input_string_2 = "i+i+i";
        lr0_parse_input(&parse_table, input_string_2);

        const char* invalid_input = "i+)";
        lr0_parse_input(&parse_table, invalid_input);

    } else {
        fprintf(stderr, "\nErrors detected during parse table construction. The grammar is NOT LR(0).\n");
    }

cleanup:
    // 统一释放所有资源
    grammar_free(grammar);
    dfa_free(&dfa);
    arena_free(grammar_arena);
    arena_free(dfa_arena);
    arena_free(table_arena);
    return 0;
}