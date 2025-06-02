// ast.h - 抽象语法树（AST）头文件
// 定义 AST 节点类型和相关接口

#ifndef AST_H
#define AST_H

// AST 节点类型枚举
typedef enum {
    AST_BINARY,    // 二元运算节点
    AST_NUMBER,    // 数字节点
    AST_IDENTIFIER // 标识符节点
} ASTNodeType;

// AST 节点结构体
typedef struct ASTNode {
    ASTNodeType type;   // 节点类型
    char op;            // 运算符：+ - * /
    struct ASTNode* left;  // 左子树
    struct ASTNode* right; // 右子树
    char value[32];     // 用于数字或变量名
} ASTNode;

// 创建二元运算节点
ASTNode* create_binary_node(char op, ASTNode* left, ASTNode* right);
// 创建数字节点
ASTNode* create_number_node(const char* value);
// 创建标识符节点
ASTNode* create_identifier_node(const char* name);
// 打印 AST，便于调试
void print_ast(ASTNode* node, int indent);

#endif
