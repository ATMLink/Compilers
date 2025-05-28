#define UNITY_BUILD // 启用 Unity Build

#include <stdio.h>
#include <stdlib.h> 

// Arena
#include "src/arena.h"      
#include "src/arena.c"

#include "src/first_follow.h"
#include "src/first_follow.c"

#include "src/grammar.h"
#include "src/grammar.c"

#include "src/first_set.h"
#include "src/first_set.c"

#include "src/follow_set.h"
#include "src/follow_set.c"

#include "src/TAC_generator.h"
#include "src/TAC_generator.c"
#include "src/asm_generator.h"
#include "src/asm_generator.c"
#include "src/reg_allocator.h"
#include "src/reg_allocator.c"
#include "src/ast.h"
#include "src/ast.c"
#include "src/semantic.h"
#include "src/semantic.c"

int main() {
    // 读取文法
    Arena* arena = arena_create(1024 * 10);
    GrammarResultGrammar result = read_grammar("grammar.txt", arena);
    if (result.status != GRAMMAR_OK) {
        fprintf(stderr, "Error reading grammar\n");
        return 1;
    }
    Grammar* grammar = result.value;

    // 计算 FIRST/FOLLOW
    SymbolSet* sets = arena_alloc(arena, GRAMMAR_MAX_SYMBOLS * sizeof(SymbolSet));
    int set_count = 0;
    compute_first_sets(grammar, sets, &set_count, arena);
    compute_follow_sets(grammar, sets, &set_count, arena);

    // 构造表达式 AST（暂时手写）
    ASTNode* node = create_binary_node('+',
        create_identifier_node("a"),
        create_binary_node('*',
            create_identifier_node("b"),
            create_identifier_node("c")));

    printf("\n--- AST ---\n");
    print_ast(node, 0);

    // 生成 TAC
    init_TAC();
    char result_name[32];
    generate_tac_from_ast(node, result_name);
    printf("\n--- TAC ---\n");
    print_TAC();

    // 输出汇编
    printf("\n--- ASM ---\n");
    gen_asm();

    arena_free(arena);
    return 0;
}