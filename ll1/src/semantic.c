// semantic.c
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
    if (node->type == AST_NUMBER || node->type == AST_IDENTIFIER) {
        strcpy(result_name, node->value);
    } else if (node->type == AST_BINARY) {
        char left[32], right[32];
        generate_tac_from_ast(node->left, left);
        generate_tac_from_ast(node->right, right);
        strcpy(result_name, new_temp());
        char op_str[2] = { node->op, '\0' };
        gen_TAC(op_str, left, right, result_name);
    }
}
