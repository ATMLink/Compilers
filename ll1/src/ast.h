// ast.h
#ifndef AST_H
#define AST_H

typedef enum {
    AST_BINARY,
    AST_NUMBER,
    AST_IDENTIFIER
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char op; // + - * /
    struct ASTNode* left;
    struct ASTNode* right;
    char value[32]; // 用于数字/变量名
} ASTNode;

ASTNode* create_binary_node(char op, ASTNode* left, ASTNode* right);
ASTNode* create_number_node(const char* value);
ASTNode* create_identifier_node(const char* name);
void print_ast(ASTNode* node, int indent);

#endif
