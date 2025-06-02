// semantic.h - 语义分析头文件
// 定义语义分析相关的数据结构和接口
#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "ast.h"

// 生成三地址码
// 根据抽象语法树节点生成对应的三地址码并输出到指定文件
void generate_tac_from_ast(ASTNode* node, char* result_name);

#endif
