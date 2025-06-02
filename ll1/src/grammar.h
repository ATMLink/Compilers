#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "arena.h"
#include <stdbool.h>
#include <stdint.h>


#define GRAMMAR_MAX_SYMBOLS 64
#define GRAMMAR_MAX_RULES 128
#define GRAMMAR_MAX_LINE_LEN 256

#ifdef DEBUG_GRAMMAR
#define GRAMMAR_DEBUG(fmt, ...) fprintf(stderr, "[GRAMMAR DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define GRAMMAR_DEBUG(fmt, ...) ((void)0)
#endif

// Rule - 表示文法中的一条产生式
typedef struct Rule
{
    char left_hs;   // 产生式左边的非终结符
    char* right_hs; // 产生式右边的串，包括终结符与非终结符
    uint8_t right_hs_count;
} Rule;

// Grammar - 表示一个文法，包括所有的产生式、符号等信息
typedef struct Grammar
{
    Rule* rules;                  // 文法规则数组
    uint8_t rule_count;          // 规则数量
    char nonterminals[GRAMMAR_MAX_SYMBOLS]; // 非终结符集合
    uint8_t nonterminals_count;  // 非终结符数量
    char terminals[GRAMMAR_MAX_SYMBOLS];     // 终结符集合
    uint8_t terminals_count;     // 终结符数量
    char start_symbol;           // 起始符号
    
} Grammar;

// ==== 错误码 ====
typedef enum {
    GRAMMAR_OK = 0,                            // 无错误
    GRAMMAR_ERROR_INVALID_ARGUMENT,            // 无效参数
    GRAMMAR_ERROR_INVALID_FORMAT,              // 格式无效
    GRAMMAR_ERROR_INVALID_NONTERMINAL,        // 非法非终结符
    GRAMMAR_ERROR_ALLOCATION_FAILED,           // 内存分配失败
    GRAMMAR_ERROR_IO_FAILED,                   // 输入输出错误
    GRAMMAR_ERROR_TOO_MANY_RULES,              // 规则数量过多
    GRAMMAR_ERROR_TOO_MANY_SYMBOLS             // 符号数量过多
} GrammarStatus;

/// GrammarResultGrammar - 用于返回 Grammar* 的结果类型
typedef struct GrammarResultGrammar{
    GrammarStatus status;  // 状态码
    Grammar* value;        // Grammar 指针
} GrammarResultGrammar;

/// GrammarResultChar - 用于返回单个字符的结果类型
typedef struct GrammarResultChar{
    GrammarStatus status;  // 状态码
    char value;            // 返回的字符
} GrammarResultChar;

/// GrammarResultString - 用于返回字符串的结果类型
typedef struct GrammarResultString{
    GrammarStatus status;  // 状态码
    char* value;          // 返回的字符串
} GrammarResultString;

/// GrammarResultVoid - 用于无值返回的结果类型
typedef struct GrammarResultVoid{
    GrammarStatus status;  // 状态码
} GrammarResultVoid;

bool grammar_is_terminal(char c);
bool grammar_is_nonterminal(char c);

GrammarResultGrammar read_grammar(const char* filename, Arena* arena);
void print_grammar(const Grammar* grammar);

#endif