#include "memory_pool.h"

#include <string.h>


/*
 * Initialize memory pool.
 */

MemoryPool_Status_t MemoryPool_Init(
    MemoryPool_t *pool,
    void *memory,
    uint8_t *used,
    uint32_t block_size,
    uint32_t block_count)
{
    if ((pool == NULL) ||
        (memory == NULL) ||
        (used == NULL))
    {
        return MEMORY_POOL_INVALID;
    }

    if ((block_size == 0U) ||
        (block_count == 0U))
    {
        return MEMORY_POOL_INVALID;
    }

    pool->memory =
        (uint8_t *)memory;

    pool->used =
        used;

    pool->block_size =
        block_size;

    pool->block_count =
        block_count;

    pool->used_count =
        0U;

    memset(
        pool->used,
        0,
        block_count
    );

    return MEMORY_POOL_OK;
}


/*
 * Allocate one block.
 */

void *MemoryPool_Allocate(
    MemoryPool_t *pool)
{
    uint32_t index;

    uint8_t *address;

    if (pool == NULL)
    {
        return NULL;
    }

    for (index = 0U;
         index < pool->block_count;
         index++)
    {
        if (pool->used[index] == 0U)
        {
            pool->used[index] = 1U;

            pool->used_count++;

            address =
                pool->memory +
                (index * pool->block_size);

            return address;
        }
    }

    return NULL;
}


/*
 * Free one block.
 */

MemoryPool_Status_t MemoryPool_Free(
    MemoryPool_t *pool,
    void *ptr)
{
    uintptr_t start;
    uintptr_t end;
    uintptr_t address;

    uint32_t index;

    if ((pool == NULL) ||
        (ptr == NULL))
    {
        return MEMORY_POOL_INVALID;
    }

    start =
        (uintptr_t)pool->memory;

    end =
        start +
        ((uintptr_t)pool->block_size *
         pool->block_count);

    address =
        (uintptr_t)ptr;

    /*
     * Verify pointer belongs to pool.
     */

    if ((address < start) ||
        (address >= end))
    {
        return MEMORY_POOL_INVALID;
    }

    /*
     * Pointer must point exactly to
     * the beginning of a block.
     */

    if (((address - start) %
         pool->block_size) != 0U)
    {
        return MEMORY_POOL_INVALID;
    }

    index =
        (uint32_t)((address - start) /
                   pool->block_size);

    if (pool->used[index] == 0U)
    {
        return MEMORY_POOL_ERROR;
    }

    pool->used[index] = 0U;

    if (pool->used_count > 0U)
    {
        pool->used_count--;
    }

    return MEMORY_POOL_OK;
}


/*
 * Get used block count.
 */

uint32_t MemoryPool_GetUsedCount(
    const MemoryPool_t *pool)
{
    if (pool == NULL)
    {
        return 0U;
    }

    return pool->used_count;
}


/*
 * Get free block count.
 */

uint32_t MemoryPool_GetFreeCount(
    const MemoryPool_t *pool)
{
    if (pool == NULL)
    {
        return 0U;
    }

    return pool->block_count -
           pool->used_count;
}


/*
 * Reset memory pool.
 */

void MemoryPool_Reset(
    MemoryPool_t *pool)
{
    if (pool == NULL)
    {
        return;
    }

    memset(
        pool->used,
        0,
        pool->block_count
    );

    pool->used_count = 0U;
}
