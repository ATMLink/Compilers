#include "ast_builder.h"
#include <stdlib.h>
#include <stdio.h>

// 根据文法构建AST
ASTNode* build_ast(TreeNode* node) {
    if (!node) return NULL;

    // 终结符 d 或 b
    if (node->child_count == 0) {
        if (node->symbol == 'd') {
            return create_identifier_node("d");
        } else if (node->symbol == 'b') {
            return create_identifier_node("b");
        } else {
            return NULL; // ε 或其它终结符
        }
    }

    // 非终结符 A -> D B | ε
    if (node->symbol == 'A') {
        if (node->child_count == 2) {
            ASTNode* left = build_ast(node->children[0]); // D
            ASTNode* right = build_ast(node->children[1]); // B
            if (left && right)
                return create_binary_node('+', left, right);
            else
                return left ? left : right;
        } else if (node->child_count == 0) {
            return NULL; // ε
        }
    }

    // 非终结符 D -> d A | d | ε
    if (node->symbol == 'D') {
        if (node->child_count == 2 &&
            node->children[0]->symbol == 'd' &&
            node->children[1]->symbol == 'A') {
            ASTNode* id = create_identifier_node("d");
            ASTNode* a = build_ast(node->children[1]);
            return create_binary_node('*', id, a);
        } else if (node->child_count == 1 &&
                   node->children[0]->symbol == 'd') {
            return create_identifier_node("d");
        } else if (node->child_count == 0) {
            return NULL; // ε
        }
    }

    // 非终结符 B -> b
    if (node->symbol == 'B') {
        if (node->child_count == 1 && node->children[0]->symbol == 'b') {
            return create_identifier_node("b");
        } else {
            return NULL;
        }
    }

    return NULL;
}
