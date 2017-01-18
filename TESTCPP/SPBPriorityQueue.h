#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creates new SPBPQueue
 *
 * Complexity O(n)
 *
 * @param int maxSize - max size of queue, must be > 0
 * @return
 * NULL in case allocation failure ocurred
 * Otherwise, pointer to a new SPBPQueue
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Creates a copy of an exist SPBPQueue
 *
 * Complexity O(n)
 *
 * @param SPBPQueue* source - a pointer to the exist queue to be copied
 * @return
 * NULL in case allocation failure ocurred
 * Otherwise, pointer to a new SPBPQueue
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * Deletes a SPBPQueue, and frees its resources
 *
 * Complexity O(n)
 * @param SPBPQueue* source - a pointer to the exist queue to be deleted
 *
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 *  Removes all the elements in the queue
 *
 *  Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the exist queue to be cleared
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * Returns the number of elements in the queue
 *
 * Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * queues size
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * Returns the maximum capacity of the queue
 *
 * Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * queues max size
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * Inserts an element to the queue
 *
 * Complexity O(n)
 *
 * @param
 * 1. SPBPQueue* source - a pointer to the queue
 * 2. int index - index of new element
 * 3. double value - value of new element
 * @return
 * (index < 0)||(source == NULL) return SP_BPQUEUE_INVALID_ARGUMENT
 * size == maxSize (dosent consider if element did / did not entered) return SP_BPQUEUE_FULL
 * Otherwise, return SP_BPQUEUE_SUCCESS
 */

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * Removes the element with the lowest value
 *
 * Complexity O(1)
 *
 * @param  SPBPQueue* source - a pointer to the queue
 * @return
 * (source == NULL) return SP_BPQUEUE_INVALID_ARGUMENT
 * size == 0 maxSize return SP_BPQUEUE_EMPTY
 * Otherwise, return SP_BPQUEUE_SUCCESS
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 *  Gets pointer to a new BPQueueElement and copys into it the element with the lowest
 *  value from queue
 *
 * Complexity O(1)
 *
 * @param
 * 1. SPBPQueue* source - a pointer to the queue
 * 2. BPQueueElement* res - a pointer to the new element which has been initialized before
 * @return
 * (source == NULL)||(res == NULL) return SP_BPQUEUE_INVALID_ARGUMENT
 * source->size == 0 maxSize return SP_BPQUEUE_EMPTY
 * Otherwise, return SP_BPQUEUE_SUCCESS
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 *  Gets pointer to a new BPQueueElement and copys into it the element with the highest
 *  value from queue
 *
 * Complexity O(1)
 *
 * @param
 * 1. SPBPQueue* source - a pointer to the queue
 * 2. BPQueueElement* res - a pointer to the new element which has been initialized before
 * @return
 * (source == NULL)||(res == NULL) return SP_BPQUEUE_INVALID_ARGUMENT
 * source->size == 0 maxSize return SP_BPQUEUE_EMPTY
 * Otherwise, return SP_BPQUEUE_SUCCESS
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 *  Returns  the minimum value in the queu
 *
 *  Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * size == 0 return -1
 * Otherwise, return min value in queue
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 *  Returns  the max value in the queue
 *
 *  Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * size == 0 return -1
 * Otherwise, return max value in queue
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 *  Returns true if the queue is empty
 *
 *  Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * true if empty
 * Otherwise, false
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 *  Returns true if the queue is full
 *
 *  Complexity O(1)
 *
 * @param SPBPQueue* source - a pointer to the queue
 * @return
 * true if full
 * Otherwise, false
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
