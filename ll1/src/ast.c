#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ASTNode* create_binary_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_number_node(const char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    strcpy(node->value, value);
    return node;
}

ASTNode* create_identifier_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_IDENTIFIER;
    strcpy(node->value, name);
    return node;
}

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
