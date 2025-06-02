#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"
#include "predict_table.h"

// parser.h - LL(1) 语法分析器头文件
// 定义 LL(1) 语法分析相关的数据结构和接口

/**
 * @brief 解析给定的标记序列
 *
 * 此函数使用提供的文法和预测分析表对标记序列执行 LL(1) 语法分析。
 *
 * @param g 指向文法结构的指针
 * @param pt 指向预测分析表结构的指针
 * @param tokens 要解析的标记序列
 * @param token_count 标记的数量
 */
void parse_tokens(const Grammar* g, const PredictTable* pt, const char* tokens[], int token_count);

#endif
