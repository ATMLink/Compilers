// tree_node.c - 语法树（CST）节点实现
// 负责构建和遍历具体语法树

#include "tree_node.h"
#include <stdlib.h>
#include <stdio.h>

// 创建一个新的树节点
// 参数：symbol - 节点代表的符号
// 返回值：新创建的树节点指针；如果分配失败，则返回NULL
TreeNode* create_node(char symbol) {
    TreeNode* node = malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->symbol = symbol;
    node->child_count = 0;
    return node;
}

// 打印树结构
// 参数：node - 当前节点；indent - 缩进级别，用于控制树的层级显示
void print_tree(TreeNode* node, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%c\n", node->symbol);
    for (int i = 0; i < node->child_count; i++) {
        print_tree(node->children[i], indent + 1);
    }
}
