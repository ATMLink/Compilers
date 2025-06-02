#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include "ast.h"
#include "tree_node.h"

// ast_builder.h - 抽象语法树（AST）构建器头文件
// 定义 AST 构建相关的数据结构和接口

// 从解析树节点构建AST
ASTNode* build_ast(TreeNode* node);

#endif
