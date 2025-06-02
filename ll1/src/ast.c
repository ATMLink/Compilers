// ast.c - 抽象语法树（AST）相关实现
// 负责 AST 节点的创建、遍历和打印等操作

#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 创建一个二元运算符节点
// op: 运算符
// left: 左子树
// right: 右子树
ASTNode* create_binary_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

// 创建一个数字字面量节点
// value: 数字的字符串表示
ASTNode* create_number_node(const char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    strcpy(node->value, value);
    return node;
}

// 创建一个标识符节点
// name: 标识符的名称
ASTNode* create_identifier_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_IDENTIFIER;
    strcpy(node->value, name);
    return node;
}

// 打印 AST 的函数
// node: 当前节点
// indent: 缩进级别，用于表示树的层次结构
void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; ++i) printf("  ");
    if (node->type == AST_BINARY) {
        printf("Op: %c\n", node->op);
        print_ast(node->left, indent + 1);
        print_ast(node->right, indent + 1);
    } else if (node->type == AST_NUMBER) {
        printf("Num: %s\n", node->value);
    } else if (node->type == AST_IDENTIFIER) {
        printf("Id: %s\n", node->value);
    }
}
