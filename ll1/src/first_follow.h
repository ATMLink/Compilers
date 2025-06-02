#ifndef FIRST_FOLLOW_H
#define FIRST_FOLLOW_H

#include "arena.h"
#include <stdbool.h>

// first_follow.h - First/Follow 集合头文件
// 定义符号集结构体和相关接口

typedef struct SymbolSet{
    char symbol;
    char* first;
    char* follow;
} SymbolSet;

/**
 * 获取或创建一个符号集
 * 如果符号集已存在，则返回该符号集的指针
 * 如果符号集不存在，则创建一个新的符号集并返回指向它的指针
 */
SymbolSet* get_or_create_set(SymbolSet* sets, int* count, char symbol, Arena* arena);

/**
 * 将字符添加到集合中
 * 如果集合中已存在该字符，则返回 false
 * 否则，将字符添加到集合中并返回 true
 */
bool add_char(char* set, char c);


#endif