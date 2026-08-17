#include "ring_buffer.h"


/*
 * Initialize ring buffer.
 */

RingBuffer_Status_t RingBuffer_Init(
    RingBuffer_t *ring,
    uint8_t *buffer,
    uint32_t size)
{
    if ((ring == NULL) ||
        (buffer == NULL) ||
        (size < 2U))
    {
        return RING_BUFFER_ERROR;
    }

    ring->buffer = buffer;

    ring->size = size;

    ring->head = 0U;

    ring->tail = 0U;

    return RING_BUFFER_OK;
}


/*
 * Push data into ring buffer.
 */

RingBuffer_Status_t RingBuffer_Push(
    RingBuffer_t *ring,
    uint8_t data)
{
    uint32_t next_head;

    if (ring == NULL)
    {
        return RING_BUFFER_ERROR;
    }

    next_head =
        (ring->head + 1U) %
        ring->size;

    /*
     * If next head equals tail,
     * buffer is full.
     */

    if (next_head == ring->tail)
    {
        return RING_BUFFER_FULL;
    }

    ring->buffer[ring->head] =
        data;

    ring->head =
        next_head;

    return RING_BUFFER_OK;
}


/*
 * Pop data from ring buffer.
 */

RingBuffer_Status_t RingBuffer_Pop(
    RingBuffer_t *ring,
    uint8_t *data)
{
    if ((ring == NULL) ||
        (data == NULL))
    {
        return RING_BUFFER_ERROR;
    }

    /*
     * Empty condition.
     */

    if (ring->head == ring->tail)
    {
        return RING_BUFFER_EMPTY;
    }

    *data =
        ring->buffer[ring->tail];

    ring->tail =
        (ring->tail + 1U) %
        ring->size;

    return RING_BUFFER_OK;
}


/*
 * Check empty.
 */

uint8_t RingBuffer_IsEmpty(
    const RingBuffer_t *ring)
{
    if (ring == NULL)
    {
        return 1U;
    }

    return
        (ring->head == ring->tail);
}


/*
 * Check full.
 */

uint8_t RingBuffer_IsFull(
    const RingBuffer_t *ring)
{
    uint32_t next_head;

    if (ring == NULL)
    {
        return 0U;
    }

    next_head =
        (ring->head + 1U) %
        ring->size;

    return
        (next_head == ring->tail);
}


/*
 * Number of bytes available.
 */

uint32_t RingBuffer_Available(
    const RingBuffer_t *ring)
{
    if (ring == NULL)
    {
        return 0U;
    }

    if (ring->head >= ring->tail)
    {
        return ring->head -
               ring->tail;
    }

    return
        ring->size -
        ring->tail +
        ring->head;
}


/*
 * Clear buffer.
 */

void RingBuffer_Clear(
    RingBuffer_t *ring)
{
    if (ring == NULL)
    {
        return;
    }

    ring->head = 0U;

    ring->tail = 0U;
}
