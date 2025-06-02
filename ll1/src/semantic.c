// semantic.c - 语义分析实现
// 负责类型检查、作用域管理等语义分析功能
#include "semantic.h"
#include "tac_generator.h"
#include <stdio.h>
#include <string.h>

static int semantic_temp_count = 0;

// char* new_temp() {
//     static char buf[32];
//     sprintf(buf, "t%d", temp_count++);
//     return strdup(buf);
// }

void generate_tac_from_ast(ASTNode* node, char* result_name) {
    // 处理数字和标识符节点，直接将值拷贝到结果名称
    if (node->type == AST_NUMBER || node->type == AST_IDENTIFIER) {
        strcpy(result_name, node->value);
    } 
    // 处理二元运算节点
    else if (node->type == AST_BINARY) {
        char left[32], right[32];
        // 递归生成左子树和右子树的三地址码
        generate_tac_from_ast(node->left, left);
        generate_tac_from_ast(node->right, right);
        // 生成临时变量名称
        strcpy(result_name, new_temp());
        char op_str[2] = { node->op, '\0' };
        // 生成三地址码
        gen_TAC(op_str, left, right, result_name);
    }
}
