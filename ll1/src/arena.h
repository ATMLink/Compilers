// arena.h - 内存池（Arena）分配器头文件
// 定义 Arena 结构体和相关接口

#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

// Arena 结构体定义
// 包含用于内存分配的缓冲区指针、大小和当前偏移量
typedef struct Arena {
    char* buffer;   // 指向内存池的指针
    size_t size;    // 内存池的总大小
    size_t offset;  // 当前分配的偏移量
}Arena;

// 创建一个新的 Arena 实例
// size - 内存池的大小
// 返回值 - 指向新创建的 Arena 实例的指针
Arena* arena_create(size_t size);

// 从 Arena 中分配一块内存
// arena - 指向 Arena 实例的指针
// size - 要分配的内存大小
// 返回值 - 指向分配内存块的指针
void* arena_alloc(Arena* arena, size_t size);

// 释放 Arena 占用的内存
// arena - 指向 Arena 实例的指针
void arena_free(Arena* arena);

#endif // ARENA_H