#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include "ast.h"
#include "tree_node.h"

// 从解析树节点构建AST
ASTNode* build_ast(TreeNode* node);

#endif
