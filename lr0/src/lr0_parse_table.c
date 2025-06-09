// atmlink/compilers/Compilers-7e07cfdaca0ac296ef0b06750c1d6dff49f71d74/lr0/src/lr0_parse_table.c

#include "lr0_parse_table.h"
#include "viable_prefix_dfa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// --- 哈希表实现 ---

#define HASH_TABLE_SIZE 257 // 使用一个素数作为哈希表大小

// 哈希函数：根据状态和符号计算哈希值
static size_t hash_func(uint8_t state, char symbol) {
    return (state * 31 + (unsigned char)symbol) % HASH_TABLE_SIZE;
}

// ---- Action 哈希表函数 ----

// 创建 Action 哈希表
static void action_table_create(ActionHashTable* table, Arena* arena) {
    table->size = HASH_TABLE_SIZE;
    table->buckets = arena_alloc(arena, sizeof(ActionNode*) * table->size);
    memset(table->buckets, 0, sizeof(ActionNode*) * table->size); // 所有桶初始化为 NULL
}

// 插入 Action，内置冲突检测
static void action_table_insert(ActionHashTable* table, Arena* arena, uint8_t state, char terminal, ActionEntry new_action, bool* has_conflict) {
    size_t index = hash_func(state, terminal);
    ActionNode* head = table->buckets[index];

    // 检查键是否已存在
    for (ActionNode* node = head; node; node = node->next) {
        if (node->state == state && node->terminal == terminal) {
            // 键已存在，检查冲突
            if (node->action->type != new_action.type || (node->action->type == ACTION_SHIFT && node->action->data.shift_state != new_action.data.shift_state) || (node->action->type == ACTION_REDUCE && node->action->data.rule_index != new_action.data.rule_index) ) {
                if ((node->action->type == ACTION_SHIFT && new_action.type == ACTION_REDUCE) || (node->action->type == ACTION_REDUCE && new_action.type == ACTION_SHIFT)) {
                     fprintf(stderr, "[Conflict] Shift-Reduce conflict at state %d on terminal '%c'.\n", state, terminal);
                } else if (node->action->type == ACTION_REDUCE && new_action.type == ACTION_REDUCE) {
                     fprintf(stderr, "[Conflict] Reduce-Reduce conflict at state %d on terminal '%c'.\n", state, terminal);
                }
                *has_conflict = true;
            }
            // 更新（覆盖）动作
            *(node->action) = new_action;
            return;
        }
    }

    // 键不存在，创建新节点并插入到链表头部
    ActionNode* new_node = arena_alloc(arena, sizeof(ActionNode));
    new_node->state = state;
    new_node->terminal = terminal;
    new_node->action = arena_alloc(arena, sizeof(ActionEntry));
    *(new_node->action) = new_action;
    new_node->next = head;
    table->buckets[index] = new_node;
}

// 查找 Action
static const ActionEntry* action_table_lookup(const ActionHashTable* table, uint8_t state, char terminal) {
    size_t index = hash_func(state, terminal);
    for (ActionNode* node = table->buckets[index]; node; node = node->next) {
        if (node->state == state && node->terminal == terminal) {
            return node->action;
        }
    }
    return NULL; // 未找到
}


// ---- GOTO 哈希表函数 ----

// 创建 GOTO 哈希表
static void goto_table_create(GotoHashTable* table, Arena* arena) {
    table->size = HASH_TABLE_SIZE;
    table->buckets = arena_alloc(arena, sizeof(GotoNode*) * table->size);
    memset(table->buckets, 0, sizeof(GotoNode*) * table->size);
}

// 插入 GOTO
static void goto_table_insert(GotoHashTable* table, Arena* arena, uint8_t state, char nonterminal, uint8_t to_state) {
     size_t index = hash_func(state, nonterminal);
     // 这里我们假设没有 GOTO 冲突，直接插入
     GotoNode* new_node = arena_alloc(arena, sizeof(GotoNode));
     new_node->state = state;
     new_node->nonterminal = nonterminal;
     new_node->to_state = to_state;
     new_node->next = table->buckets[index];
     table->buckets[index] = new_node;
}

// 查找 GOTO，返回目标状态，-1表示未找到
static int8_t goto_table_lookup(const GotoHashTable* table, uint8_t state, char nonterminal) {
    size_t index = hash_func(state, nonterminal);
    for (GotoNode* node = table->buckets[index]; node; node = node->next) {
        if (node->state == state && node->nonterminal == nonterminal) {
            return node->to_state;
        }
    }
    return -1; // 未找到
}


// --- 核心功能函数（已适配哈希表）---

// ... (get_terminal_index, get_nonterminal_index, etc. from previous response) ...
static int get_terminal_index(const Grammar* grammar, char terminal) { /* ... */ }
static int get_nonterminal_index(const Grammar* grammar, char nonterminal) { /* ... */ }
static bool is_reduce_item(const DFAItem* item) { /* ... */ }
static int find_rule_index(const Grammar* grammar, const DFAItem* item) { /* ... */ }


bool build_lr0_parse_table(const Grammar* grammar, const DFA* dfa, LR0ParseTable* table, Arena* arena) {
    // 初始化
    table->grammar = grammar;
    table->state_count = dfa->state_count;
    table->terminal_count = grammar->terminals_count;
    table->nonterminal_count = grammar->nonterminals_count;
    action_table_create(&table->action_table, arena);
    goto_table_create(&table->goto_table, arena);

    bool has_conflict = false;

    // 遍历所有状态
    for (int state = 0; state < dfa->state_count; state++) {
        ItemSet* set = &dfa->states[state];

        // 遍历项目，处理规约和接受动作
        for (int i = 0; i < set->item_count; i++) {
            DFAItem* item = &set->items[i];
            if (is_reduce_item(item)) {
                if (item->left_symbol == grammar->rules[0].left_hs) {
                    action_table_insert(&table->action_table, arena, state, '#', (ActionEntry){.type=ACTION_ACCEPT}, &has_conflict);
                } else {
                    int rule_index = find_rule_index(grammar, item);
                    if (rule_index != -1) {
                        for (int t = 0; t < grammar->terminals_count; t++) {
                            char terminal = grammar->terminals[t];
                            action_table_insert(&table->action_table, arena, state, terminal, (ActionEntry){.type=ACTION_REDUCE, .data.rule_index=rule_index}, &has_conflict);
                        }
                    }
                }
            }
        }
    }
    
    // 遍历转移，处理移入和GOTO动作
    for (int t = 0; t < dfa->transition_count; t++) {
        Transition* tr = &dfa->transitions[t];
        if (grammar_is_terminal(tr->symbol)) {
            ActionEntry shift_action = {.type=ACTION_SHIFT, .data.shift_state=tr->to_state};
            action_table_insert(&table->action_table, arena, tr->from_state, tr->symbol, shift_action, &has_conflict);
        } else {
            goto_table_insert(&table->goto_table, arena, tr->from_state, tr->symbol, tr->to_state);
        }
    }

    return !has_conflict;
}


void print_lr0_parse_table(const LR0ParseTable* table, FILE* out) {
    const Grammar* grammar = table->grammar;
    fprintf(out, "LR(0) ACTION Table (Hash-based):\n");
    fprintf(out, "State\t");
    // 打印表头（所有终结符）
    for (int t = 0; t < grammar->terminals_count; t++) {
        fprintf(out, "%c\t", grammar->terminals[t]);
    }
     // 额外打印结束符'#'
    fprintf(out, "#\t");
    fprintf(out, "\n");

    // 遍历所有状态，打印每一行
    for (int s = 0; s < table->state_count; s++) {
        fprintf(out, "%d\t", s);
        // 对每个终结符查找并打印动作
        for (int t = 0; t < grammar->terminals_count; t++) {
            const ActionEntry* entry = action_table_lookup(&table->action_table, s, grammar->terminals[t]);
            if (!entry) {
                fprintf(out, "-\t");
                continue;
            }
            switch (entry->type) {
                case ACTION_SHIFT: fprintf(out, "s%d\t", entry->data.shift_state); break;
                // *** BUG已修复：使用 .rule_index ***
                case ACTION_REDUCE: fprintf(out, "r%d\t", entry->data.rule_index); break;
                case ACTION_ACCEPT: fprintf(out, "acc\t"); break;
                default: fprintf(out, "-\t"); break;
            }
        }
        // 额外查找并打印结束符'#'的动作
        const ActionEntry* entry_dollar = action_table_lookup(&table->action_table, s, '#');
        if(entry_dollar && entry_dollar->type == ACTION_ACCEPT){
            fprintf(out, "acc\t");
        } else {
            fprintf(out, "-\t");
        }
        fprintf(out, "\n");
    }

    fprintf(out, "\nLR(0) GOTO Table (Hash-based):\n");
    fprintf(out, "State\t");
    for (int nt = 0; nt < grammar->nonterminals_count; nt++) {
        fprintf(out, "%c\t", grammar->nonterminals[nt]);
    }
    fprintf(out, "\n");

    for (int s = 0; s < table->state_count; s++) {
        fprintf(out, "%d\t", s);
        for (int nt = 0; nt < grammar->nonterminals_count; nt++) {
            int8_t target = goto_table_lookup(&table->goto_table, s, grammar->nonterminals[nt]);
            if (target >= 0) {
                fprintf(out, "%d\t", target);
            } else {
                fprintf(out, "-\t");
            }
        }
        fprintf(out, "\n");
    }
}


void lr0_parse_input(const LR0ParseTable* table, const char* input) {
    printf("\n--- Starting LR(0) Parsing for Input: \"%s\" ---\n", input);
    const Grammar* grammar = table->grammar;

    uint8_t state_stack[MAX_STATES];
    int stack_top = 0;
    state_stack[stack_top] = 0;

    const char* ip = input;

    while (1) {
        uint8_t current_state = state_stack[stack_top];
        char lookahead = (*ip == '\0') ? '#' : *ip;

        const ActionEntry* action = action_table_lookup(&table->action_table, current_state, lookahead);

        if (!action) {
            fprintf(stderr, "Error: Syntax error at state %d with lookahead '%c'. No action defined.\n", current_state, lookahead);
            return;
        }

        switch (action->type) {
            case ACTION_SHIFT: {
                uint8_t next_state = action->data.shift_state;
                printf("State %d, Lookahead '%c': SHIFT to state %d\n", current_state, lookahead, next_state);
                state_stack[++stack_top] = next_state;
                ip++;
                break;
            }
            case ACTION_REDUCE: {
                uint8_t rule_idx = action->data.rule_index;
                const Rule* rule = &grammar->rules[rule_idx];
                size_t rhs_len = strlen(rule->right_hs);
                printf("State %d, Lookahead '%c': REDUCE by rule %d (%c -> %s)\n", current_state, lookahead, rule_idx, rule->left_hs, rule->right_hs);
                
                stack_top -= rhs_len;

                uint8_t state_after_pop = state_stack[stack_top];
                int8_t goto_state = goto_table_lookup(&table->goto_table, state_after_pop, rule->left_hs);

                if (goto_state == -1) {
                     fprintf(stderr, "Error: No GOTO entry for state %d on symbol %c.\n", state_after_pop, rule->left_hs);
                     return;
                }

                printf("  GOTO(state %d, %c) -> state %d\n", state_after_pop, rule->left_hs, goto_state);
                state_stack[++stack_top] = goto_state;
                break;
            }
            case ACTION_ACCEPT:
                printf("State %d, Lookahead '%c': ACCEPT. Input string is valid.\n", current_state, lookahead);
                return;
            default:
                fprintf(stderr, "Internal Error: Unknown action type.\n");
                return;
        }
    }
}