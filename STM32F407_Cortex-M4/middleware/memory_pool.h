#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include <stddef.h>

/*
 * ============================================================
 * Memory Pool
 *
 * Fixed-size memory allocator for embedded systems.
 *
 * Target:
 *     STM32F407 Cortex-M4
 *
 * Purpose:
 *     Avoid repeated malloc/free operations and
 *     reduce heap fragmentation.
 * ============================================================
 */

typedef enum
{
    MEMORY_POOL_OK = 0,
    MEMORY_POOL_ERROR,
    MEMORY_POOL_FULL,
    MEMORY_POOL_INVALID

} MemoryPool_Status_t;


/*
 * Memory pool configuration.
 *
 * Example:
 *
 *     64 blocks
 *     64 bytes per block
 *
 * Total:
 *
 *     64 * 64 = 4096 bytes
 */

#define MEMORY_POOL_BLOCK_SIZE       64U
#define MEMORY_POOL_BLOCK_COUNT      64U


typedef struct
{
    uint8_t *memory;

    uint8_t *used;

    uint32_t block_size;

    uint32_t block_count;

    uint32_t used_count;

} MemoryPool_t;


/*
 * Initialize memory pool.
 */
MemoryPool_Status_t MemoryPool_Init(
    MemoryPool_t *pool,
    void *memory,
    uint8_t *used,
    uint32_t block_size,
    uint32_t block_count);


/*
 * Allocate one block.
 */
void *MemoryPool_Allocate(
    MemoryPool_t *pool);


/*
 * Release one block.
 */
MemoryPool_Status_t MemoryPool_Free(
    MemoryPool_t *pool,
    void *ptr);


/*
 * Get number of used blocks.
 */
uint32_t MemoryPool_GetUsedCount(
    const MemoryPool_t *pool);


/*
 * Get number of free blocks.
 */
uint32_t MemoryPool_GetFreeCount(
    const MemoryPool_t *pool);


/*
 * Reset complete memory pool.
 */
void MemoryPool_Reset(
    MemoryPool_t *pool);

#endif /* MEMORY_POOL_H */
