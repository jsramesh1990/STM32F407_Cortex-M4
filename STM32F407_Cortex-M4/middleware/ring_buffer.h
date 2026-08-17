#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>


/*
 * ============================================================
 * Ring Buffer
 *
 * Used for:
 *
 *     UART RX
 *     UART TX
 *     Command processing
 *
 * ============================================================
 */

typedef enum
{
    RING_BUFFER_OK = 0,
    RING_BUFFER_ERROR,
    RING_BUFFER_FULL,
    RING_BUFFER_EMPTY

} RingBuffer_Status_t;


typedef struct
{
    uint8_t *buffer;

    uint32_t size;

    volatile uint32_t head;

    volatile uint32_t tail;

} RingBuffer_t;


/*
 * Initialize ring buffer.
 */
RingBuffer_Status_t RingBuffer_Init(
    RingBuffer_t *ring,
    uint8_t *buffer,
    uint32_t size);


/*
 * Push one byte.
 */
RingBuffer_Status_t RingBuffer_Push(
    RingBuffer_t *ring,
    uint8_t data);


/*
 * Pop one byte.
 */
RingBuffer_Status_t RingBuffer_Pop(
    RingBuffer_t *ring,
    uint8_t *data);


/*
 * Check whether buffer is empty.
 */
uint8_t RingBuffer_IsEmpty(
    const RingBuffer_t *ring);


/*
 * Check whether buffer is full.
 */
uint8_t RingBuffer_IsFull(
    const RingBuffer_t *ring);


/*
 * Number of bytes available.
 */
uint32_t RingBuffer_Available(
    const RingBuffer_t *ring);


/*
 * Clear ring buffer.
 */
void RingBuffer_Clear(
    RingBuffer_t *ring);

#endif /* RING_BUFFER_H */
