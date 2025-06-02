// arena.h - 内存池（Arena）分配器头文件
// 定义 Arena 结构体和相关接口

#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

// Arena 结构体，表示一个内存池
typedef struct Arena {
    char* buffer;    // 内存池的起始地址
    size_t size;     // 内存池的总大小
    size_t offset;   // 当前已分配的大小
}Arena;

// 创建一个新的 Arena，大小为 size
Arena* arena_create(size_t size);

// 从指定的 Arena 中分配大小为 size 的内存块
void* arena_alloc(Arena* arena, size_t size);

// 释放指定的 Arena 占用的内存
void arena_free(Arena* arena);

#endif // ARENA_H