// arena.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arena.h"

typedef struct ArenaChunk {
    char* buffer;
    size_t size;
    size_t offset;
    struct ArenaChunk* next;
} ArenaChunk;

struct Arena {
    ArenaChunk* head;
    size_t chunk_size;
};

// 创建一个新块
static ArenaChunk* arena_chunk_create(size_t size) {
    ArenaChunk* chunk = (ArenaChunk*)malloc(sizeof(ArenaChunk));
    if (!chunk) return NULL;

    chunk->buffer = (char*)malloc(size);
    if (!chunk->buffer) {
        free(chunk);
        return NULL;
    }

    chunk->size = size;
    chunk->offset = 0;
    chunk->next = NULL;
    return chunk;
}

struct Arena* arena_create(size_t chunk_size) {
    struct Arena* arena = (struct Arena*)malloc(sizeof(struct Arena));
    if (!arena) return NULL;

    arena->chunk_size = chunk_size;
    arena->head = arena_chunk_create(chunk_size);
    if (!arena->head) {
        free(arena);
        return NULL;
    }
    return arena;
}

void* arena_alloc(struct Arena* arena, size_t size) {
    if (arena == NULL || arena->head == NULL) return NULL;

    ArenaChunk* chunk = arena->head;

    // 在现有块中找空间
    while (chunk) {
        size_t aligned_offset = (chunk->offset + 7) & ~((size_t)7);  // 8字节对齐
        if (aligned_offset + size <= chunk->size) {
            void* ptr = chunk->buffer + aligned_offset;
            chunk->offset = aligned_offset + size;
            return ptr;
        }

        // 若没有空间，尝试下一块
        if (chunk->next) {
            chunk = chunk->next;
        } else {
            break;
        }
    }

    // 当前链表中无空间，新建一块
    size_t new_chunk_size = (size > arena->chunk_size) ? size : arena->chunk_size;
    ArenaChunk* new_chunk = arena_chunk_create(new_chunk_size);
    if (!new_chunk) {
        fprintf(stderr, "Arena out of memory (unable to allocate new chunk)\n");
        return NULL;
    }

    chunk->next = new_chunk;
    void* ptr = new_chunk->buffer;
    new_chunk->offset = size;
    return ptr;
}

void arena_free(struct Arena* arena) {
    if (!arena) return;

    ArenaChunk* chunk = arena->head;
    while (chunk) {
        ArenaChunk* next = chunk->next;
        free(chunk->buffer);
        free(chunk);
        chunk = next;
    }

    free(arena);
}
