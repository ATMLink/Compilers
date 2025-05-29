#include "parser.h"
#include "tree_node.h"
#include <string.h>
#include <stdio.h>
#include "predict_table.h"

#define STACK_MAX 1024
#define EPSILON '#'
#define END_SYMBOL '$'

void parse_tokens_with_tree(const Grammar* g, const PredictTable* pt,
                            const char* tokens[], int token_count,
                            TreeNode** root_out) {
    char symbol_stack[STACK_MAX];
    TreeNode* node_stack[STACK_MAX];
    int top = 0;

    // 初始化栈
    symbol_stack[top] = g->start_symbol;
    node_stack[top++] = create_node(g->start_symbol);
    *root_out = node_stack[0]; // root 指向开始符号

    int ip = 0;

    while (top > 0) {
        char X = symbol_stack[--top];
        TreeNode* current_node = node_stack[top];
        const char* a = (ip < token_count) ? tokens[ip] : "$";

        if (X == *a) {
            printf("匹配: %c\n", X);
            ip++;
            continue;
        }

        if (X == EPSILON) continue;

        if (grammar_is_terminal(X)) {
            printf("语法错误：期望 %c，实际 %s\n", X, a);
            return;
        }

        const Rule* rule = lookup_predict(pt, X, *a);
        if (!rule) {
            printf("语法错误：[%c][%s] 无匹配规则\n", X, a);
            return;
        }

        printf("应用规则: %c -> %s\n", rule->left_hs, rule->right_hs);

        // 按照产生式右部构建子节点，并逆序压入栈
        for (int i = rule->right_hs_count - 1; i >= 0; --i) {
            char sym = rule->right_hs[i];
            if (sym == EPSILON) continue;

            TreeNode* child = create_node(sym);
            // 插入到父节点（这里我们顺序插入，打印时会正序）
            current_node->children[current_node->child_count++] = child;

            symbol_stack[top] = sym;
            node_stack[top++] = child;
        }
    }

    if (ip == token_count) {
        printf("输入符号串分析成功！\n");
    } else {
        printf("输入未完全处理，剩余：");
        for (int i = ip; i < token_count; i++) printf("%s ", tokens[i]);
        printf("\n");
    }
}
