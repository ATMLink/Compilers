#include "tree_node.h"
#include <stdlib.h>
#include <stdio.h>

TreeNode* create_node(char symbol) {
    TreeNode* node = malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->symbol = symbol;
    node->child_count = 0;
    return node;
}

void print_tree(TreeNode* node, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%c\n", node->symbol);
    for (int i = 0; i < node->child_count; i++) {
        print_tree(node->children[i], indent + 1);
    }
}
