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

#include "src/tac_generator.h"
#include "src/tac_generator.c"
#include "src/asm_generator.h"
#include "src/asm_generator.c"
#include "src/reg_allocator.h"
#include "src/reg_allocator.c"
#include "src/ast.h"
#include "src/ast.c"
#include "src/semantic.h"
#include "src/semantic.c"
#include "src/predict_table.h"
#include "src/predict_table.c"
#include "src/parser.h"
#include "src/parser.c"
#include "src/tree_node.h"
#include "src/tree_node.c"
#include "src/ast_builder.h"
#include "src/ast_builder.c"


// Simple lexer to convert input string to token array
#define MAX_TOKENS 100
const char** tokenize(const char* input, int* token_count) {
    static const char* tokens[MAX_TOKENS];
    *token_count = 0;
    
    for (const char* p = input; *p && *token_count < MAX_TOKENS; p++) {
        if (isspace(*p)) continue; // Skip whitespace
        if (*p == 'c' || *p == 'd' || *p == 'b') { // Valid terminals from grammar
            char* token = malloc(2);
            token[0] = *p;
            token[1] = '\0';
            tokens[(*token_count)++] = token;
        } else {
            fprintf(stderr, "Invalid character in input: %c\n", *p);
            return NULL;
        }
    }
    return tokens;
}

// Free token array
void free_tokens(const char** tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        free((char*)tokens[i]);
    }
}

int main(int argc, char** argv) {
// main.c - LL(1) 语法分析器主程序
// 负责驱动文法读取、First/Follow 集合计算、预测分析表构建、词法分析、语法分析、AST/TAC/汇编生成等全流程
// 支持语法制导翻译（SDT）和汇编代码输出

    // Step 1: Initialize arena for memory management
    // 创建 1MB 的内存池，用于后续所有对象的分配
    Arena* arena = arena_create(1024 * 1024); // 1MB arena
    if (!arena) {
        fprintf(stderr, "Failed to create arena\n");
        return 1;
    }

    // Step 2: Read grammar from file
    // 从 grammar.txt 读取文法，支持带 SDT 语义动作的规则
    GrammarResultGrammar grammar_res = read_grammar("grammar.txt", arena);
    if (grammar_res.status != GRAMMAR_OK) {
        fprintf(stderr, "Failed to read grammar\n");
        arena_free(arena);
        return 1;
    }
    Grammar* grammar = grammar_res.value;
    grammar->start_symbol = 'A'; // Set start symbol
    print_grammar(grammar);

    // Step 3: Compute First and Follow sets
    // 计算所有符号的 First 和 Follow 集合
    SymbolSet sets[GRAMMAR_MAX_SYMBOLS];
    int set_count = 0;
    compute_first_sets(grammar, sets, &set_count, arena);
    compute_follow_sets(grammar, sets, &set_count, arena);

    // 打印 First 和 Follow 集合，便于调试
    printf("\nFirst and Follow Sets:\n");
    for (int i = 0; i < set_count; i++) {
        printf("Symbol %c: First={%s}, Follow={%s}\n", 
               sets[i].symbol, sets[i].first, sets[i].follow);
    }

    // Step 4: Compute predict table
    // 构建 LL(1) 预测分析表
    PredictTable predict_table = {0};
    compute_predict_table(grammar, sets, set_count, &predict_table);
    print_predict_table(&predict_table);

    // Step 5: Tokenize input
    // 将输入字符串分割为 token 数组，供 LL(1) 语法分析使用
    const char* input = (argc > 1) ? argv[1] : "db"; // Default input: "db"
    int token_count;
    const char** tokens = tokenize(input, &token_count);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed\n");
        arena_free(arena);
        return 1;
    }
    printf("\nTokens: ");
    for (int i = 0; i < token_count; i++) {
        printf("%s ", tokens[i]);
    }
    printf("\n");

    // Step 6: Parse tokens and build CST
    // 用 LL(1) 预测分析表对 token 流进行语法分析，构建具体语法树（CST）
    TreeNode* cst_root = NULL;
    parse_tokens_with_tree(grammar, &predict_table, tokens, token_count, &cst_root);
    if (!cst_root) {
        fprintf(stderr, "Parsing failed\n");
        free_tokens(tokens, token_count);
        arena_free(arena);
        return 1;
    }
    printf("\nConcrete Syntax Tree:\n");
    print_tree(cst_root, 0);

    // Step 7: Build AST from CST
    // 从 CST 构建抽象语法树（AST）
    ASTNode* ast_root = build_ast(cst_root);
    if (!ast_root) {
        fprintf(stderr, "AST construction failed\n");
        free_tokens(tokens, token_count);
        arena_free(arena);
        return 1;
    }
    printf("\nAbstract Syntax Tree:\n");
    print_ast(ast_root, 0);

    // Step 8: Generate TAC from AST
    // 从 AST 生成三地址码（TAC）
    init_TAC();
    char result_name[32];
    generate_tac_from_ast(ast_root, result_name);
    printf("\nThree-Address Code:\n");
    print_TAC();

    // Step 9: Optimize TAC
    // 对三地址码进行优化
    optimize_TAC();
    printf("\nOptimized Three-Address Code:\n");
    print_TAC();

    // Step 10: Generate assembly code
    // 根据优化后的 TAC 生成汇编代码
    printf("\nAssembly Code:\n");
    gen_asm();

    // Cleanup
    // 释放 token 数组和 arena 内存池
    free_tokens(tokens, token_count);
    arena_free(arena);
    // Note: AST and CST nodes are not freed as they use arena memory
    return 0;
}