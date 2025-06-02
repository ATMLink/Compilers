// arena.c - 简单内存池（Arena）分配器实现
// 用于高效分配和回收大量小对象，适合编译器数据结构

#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

// 创建一个新的Arena，大小为size字节
Arena* arena_create(size_t size) {
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    if (!arena) {
        fprintf(stderr, "Failed to allocate Arena\n");
        return NULL;
    }
    arena->buffer = (char*)malloc(size);
    if (!arena->buffer) {
        fprintf(stderr, "Failed to allocate Arena buffer\n");
        free(arena);
        return NULL;
    }
    arena->size = size;
    arena->offset = 0;
    return arena;
}

// 从Arena中分配大小为size的内存块
void* arena_alloc(Arena* arena, size_t size) {
    if (arena == NULL || arena->buffer == NULL) {
        fprintf(stderr, "Invalid Arena\n");
        return NULL;
    }
    if (arena->offset + size > arena->size) {
        fprintf(stderr, "Arena out of memory\n");
        return NULL;
    }
    void* ptr = arena->buffer + arena->offset;
    arena->offset += size;
    return ptr;
}

// 释放Arena及其占用的内存
void arena_free(Arena* arena) {
    if (arena) {
        free(arena->buffer);
        free(arena);
    }
}