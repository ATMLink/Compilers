#ifndef FIRST_SET_H
#define FIRST_SET_H

#include "arena.h"
#include "grammar.h"
#include "first_follow.h"

// first_set.h - First 集合头文件
// 定义 First 集合相关接口

void compute_first_sets(Grammar* grammar, SymbolSet* sets, int* set_count, Arena* arena);

#endif