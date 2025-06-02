#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "grammar.h"
// #define GRAMMAR_MAX_SYMBOLS 32

// tree_node.h - 语法树（CST）节点头文件
// 定义具体语法树节点相关的数据结构和接口

typedef struct TreeNode {
    char symbol;  // 节点表示的符号
    struct TreeNode* children[GRAMMAR_MAX_SYMBOLS];  // 子节点指针数组
    int child_count;  // 子节点数量
} TreeNode;

// 创建一个新的语法树节点
// symbol - 节点表示的符号
// 返回值 - 新建的语法树节点指针
TreeNode* create_node(char symbol);

// 打印语法树（缩进打印）
// node - 要打印的语法树节点
// indent - 当前缩进级别
void print_tree(TreeNode* node, int indent);

#endif
