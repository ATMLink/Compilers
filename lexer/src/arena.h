// arena.h
#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>  // 提供 size_t 类型定义

// 前向声明 Arena 结构体。
// 注意：这里没有使用 typedef，用户必须显式使用 "struct Arena"
struct Arena;

/**
 * 创建一个 Arena 分配器。
 * 
 * @param size 每个内存块（chunk）的初始大小。用于控制后续的内存分配策略。
 * @return 返回 Arena 分配器的指针，失败返回 NULL。
 */
struct Arena* arena_create(size_t size);

/**
 * 从 Arena 分配器中分配一段内存。
 * 
 * Arena 会从当前的内存块中寻找空间，必要时会创建新的块。
 * 所有内存对齐到 8 字节，适用于常见数据类型。
 * 
 * @param arena 要使用的 Arena 分配器
 * @param size 要分配的内存大小（字节）
 * @return 成功返回指向分配内存的指针，失败返回 NULL。
 */
void* arena_alloc(struct Arena* arena, size_t size);

/**
 * 销毁 Arena 分配器，并释放其分配的所有内存块。
 * 
 * 所有由 arena_alloc 分配的内存也会随之释放，无需单独释放。
 * 调用该函数后，arena 指针不再有效。
 * 
 * @param arena 要销毁的 Arena 分配器
 */
void arena_free(struct Arena* arena);

#endif // ARENA_H
