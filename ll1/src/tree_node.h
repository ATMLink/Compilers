#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "grammar.h"
// #define GRAMMAR_MAX_SYMBOLS 32

typedef struct TreeNode {
    char symbol;
    struct TreeNode* children[GRAMMAR_MAX_SYMBOLS];
    int child_count;
} TreeNode;

// 创建一个新的语法树节点
TreeNode* create_node(char symbol);

// 打印语法树（缩进打印）
void print_tree(TreeNode* node, int indent);

#endif
