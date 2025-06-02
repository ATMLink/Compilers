// arena.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arena.h"

// 定义 ArenaChunk 结构体，表示一个内存块
typedef struct ArenaChunk {
    char* buffer;             // 实际的内存缓冲区
    size_t size;              // 缓冲区大小
    size_t offset;            // 当前分配到的偏移位置（已使用的内存）
    struct ArenaChunk* next;  // 指向下一个内存块的指针（形成链表）
} ArenaChunk;

// Arena 结构体，管理多个内存块
struct Arena {
    ArenaChunk* head;     // 指向链表中第一个内存块
    size_t chunk_size;    // 每个内存块的默认大小
};

// 创建一个新的内存块
static ArenaChunk* arena_chunk_create(size_t size) {
    // 为 ArenaChunk 结构体本身分配内存
    ArenaChunk* chunk = (ArenaChunk*)malloc(sizeof(ArenaChunk));
    if (!chunk) return NULL;

    // 为 buffer 分配指定大小的内存
    chunk->buffer = (char*)malloc(size);
    if (!chunk->buffer) {
        free(chunk); // 若分配失败，释放结构体本身
        return NULL;
    }

    chunk->size = size;       // 记录内存块大小
    chunk->offset = 0;        // 初始未使用
    chunk->next = NULL;       // 还没有后续块
    return chunk;
}

// 创建一个 Arena 实例
struct Arena* arena_create(size_t chunk_size) {
    struct Arena* arena = (struct Arena*)malloc(sizeof(struct Arena));
    if (!arena) return NULL;

    arena->chunk_size = chunk_size;                 // 设置默认块大小
    arena->head = arena_chunk_create(chunk_size);   // 创建首个内存块
    if (!arena->head) {
        free(arena);  // 创建失败时清理
        return NULL;
    }
    return arena;
}

// 从 Arena 中分配一段指定大小的内存
void* arena_alloc(struct Arena* arena, size_t size) {
    if (arena == NULL || arena->head == NULL) return NULL;

    ArenaChunk* chunk = arena->head;

    // 遍历当前块链表，尝试从中找到可用空间
    while (chunk) {
        // 对 offset 进行 8 字节对齐
        size_t aligned_offset = (chunk->offset + 7) & ~((size_t)7);

        // 检查当前块中是否还有足够的空间
        if (aligned_offset + size <= chunk->size) {
            void* ptr = chunk->buffer + aligned_offset;  // 返回对齐后的地址
            chunk->offset = aligned_offset + size;        // 更新 offset
            return ptr;
        }

        // 当前块不足空间，尝试进入下一个块
        if (chunk->next) {
            chunk = chunk->next;
        } else {
            break;  // 到链表末尾
        }
    }

    // 当前所有块都无足够空间，需要创建一个新块
    size_t new_chunk_size = (size > arena->chunk_size) ? size : arena->chunk_size;
    ArenaChunk* new_chunk = arena_chunk_create(new_chunk_size);
    if (!new_chunk) {
        fprintf(stderr, "Arena out of memory (unable to allocate new chunk)\n");
        return NULL;
    }

    // 将新块添加到链表中
    chunk->next = new_chunk;

    void* ptr = new_chunk->buffer;   // 新块起始地址
    new_chunk->offset = size;        // 更新新块使用偏移
    return ptr;
}

// 销毁 Arena，释放所有内存
void arena_free(struct Arena* arena) {
    if (!arena) return;

    ArenaChunk* chunk = arena->head;
    // 逐个释放每个内存块
    while (chunk) {
        ArenaChunk* next = chunk->next;
        free(chunk->buffer);  // 释放缓冲区
        free(chunk);          // 释放结构体
        chunk = next;
    }

    free(arena);  // 最后释放 Arena 本身
}
