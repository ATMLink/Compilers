// parser.h - 正则表达式递归下降解析器头文件
// 定义正则表达式到 NFA 的解析接口

#ifndef PARSER_H
#define PARSER_H

#include "nfa.h"

// 解析正则表达式字符串 regex，返回 NFA 的起始状态指针
State* parse_regex(const char* regex, struct Arena* arena);

#endif