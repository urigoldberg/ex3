//#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

struct sp_bp_queue_t {
	int maxSize;
	int size;
	int start;
	BPQueueElement* queue;
};

BPQueueElement* createBPQueueElement(int index,double value){
	BPQueueElement* Element = (BPQueueElement*)malloc(sizeof(Element));
	if (Element == NULL) {
		return NULL;
	}
	Element->index = index;
	Element->value = value;
	return Element;
}

SPBPQueue* spBPQueueCreate(int maxSize) {
	if (maxSize > 0) {
		SPBPQueue* queue = (SPBPQueue*) malloc(sizeof(*queue));
		if (queue == NULL) {
			return NULL;
		}
		queue->maxSize = maxSize;
		queue->size = 0;
		queue->start = 0;
		queue->queue = (BPQueueElement*) malloc(
				sizeof(BPQueueElement) * maxSize);
		if (queue->queue == NULL) {
			free(queue);
			return NULL;
		}
		for (int i = 0; i < maxSize; i++) {
			queue->queue[i].index = -1;
			queue->queue[i].value = FLT_MAX;
		}

		return queue;
	} else {
		return NULL;
	}

}

SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
	SPBPQueue* queueCopy = (SPBPQueue*) malloc(sizeof(*queueCopy));
	queueCopy->maxSize = source->maxSize;
	queueCopy->size = source->size;
	queueCopy->start = source->start;
	queueCopy->queue = (BPQueueElement*) malloc(sizeof(BPQueueElement)*source->maxSize);
	for (int i = 0; i < source->size; ++i) {
		queueCopy->queue[(queueCopy->start+i)%(queueCopy->maxSize)].index = source->queue[(queueCopy->start+i)%(queueCopy->maxSize)].index;
		queueCopy->queue[(queueCopy->start+i)%(queueCopy->maxSize)].value = source->queue[(queueCopy->start+i)%(queueCopy->maxSize)].value;
	}
	return queueCopy;
}


void spBPQueueDestroy(SPBPQueue* source) {
	free(source->queue);
	free(source);
}

void spBPQueueClear(SPBPQueue* source) {
	source->size = 0;
	source->start = 0;
}

int spBPQueueSize(SPBPQueue* source) {
	return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source) {
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value) {
	//Assert source != null and maxSize is at least one
	if ((index < 0)||(source == NULL))
		return SP_BPQUEUE_INVALID_ARGUMENT;

	//Array is empty, enter first element
	if (source->size == 0){
		source->queue[source->start].index=index;
		source->queue[source->start].value=value;
		source->size++;
		return SP_BPQUEUE_SUCCESS;
	}

	//Array is neither empty nor full

	if (spBPQueueSize(source) < spBPQueueGetMaxSize(source)){
		for(int i = 0; i < source->size; i++) {
			if (source->queue[(source->start+i)%(source->maxSize)].value > value){
				int tempIndex = source->queue[(source->start+i)%(source->maxSize)].index;
				double tempValue = source->queue[(source->start+i)%(source->maxSize)].value;
				source->queue[(source->start+i)%(source->maxSize)].index = index;
				source->queue[(source->start+i)%(source->maxSize)].value = value;
				index = tempIndex;
				value = tempValue;

				//change for ex3 - if value equal - index is tie breaker
				if (source->queue[(source->start+i)%(source->maxSize)].value == value)
					if (source->queue[(source->start+i)%(source->maxSize)].index > index) {
					int tempIndex = source->queue[(source->start+i)%(source->maxSize)].index;
					double tempValue = source->queue[(source->start+i)%(source->maxSize)].value;
					source->queue[(source->start+i)%(source->maxSize)].index = index;
					source->queue[(source->start+i)%(source->maxSize)].value = value;
					index = tempIndex;
					value = tempValue;

			}
		}
		}
		source->queue[(source->start+source->size)%(source->maxSize)].index = index;
		source->queue[(source->start+source->size)%(source->maxSize)].value = value;
		source->size++;
		return SP_BPQUEUE_SUCCESS;
	}

	//Array is full
	for(int i = 0; i < source->size; i++) {
		if (source->queue[i].value > value){
			int tempIndex = source->queue[(source->start+i)%(source->maxSize)].index;
			double tempValue = source->queue[(source->start+i)%(source->maxSize)].value;
			source->queue[(source->start+i)%(source->maxSize)].index = index;
			source->queue[(source->start+i)%(source->maxSize)].value = value;
			index = tempIndex;
			value = tempValue;
			}
		//change for ex3 - if value equal - index is tie breaker
		if (source->queue[(source->start+i)%(source->maxSize)].value == value)
			if (source->queue[(source->start+i)%(source->maxSize)].index > index) {
			int tempIndex = source->queue[(source->start+i)%(source->maxSize)].index;
			double tempValue = source->queue[(source->start+i)%(source->maxSize)].value;
			source->queue[(source->start+i)%(source->maxSize)].index = index;
			source->queue[(source->start+i)%(source->maxSize)].value = value;
			index = tempIndex;
			value = tempValue;

			}
	}
	return SP_BPQUEUE_FULL;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source) {
	if (source == NULL)
			return SP_BPQUEUE_INVALID_ARGUMENT;
	if (source->size == 0)
		return SP_BPQUEUE_EMPTY;
	source->size--;
	source->start = (source->start+1)%(source->maxSize);
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res) {
	if ((source == NULL)||(res == NULL))
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if (source->size == 0)
		return SP_BPQUEUE_EMPTY;
	res->index = source->queue[source->start].index;
	res->value = source->queue[source->start].value;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res) {
	if ((source == NULL)||(res == NULL))
			return SP_BPQUEUE_INVALID_ARGUMENT;
	if (source->size == 0)
		return SP_BPQUEUE_EMPTY;
	res->index = source->queue[(source->start+source->size-1)%source->maxSize].index;
	res->value = source->queue[(source->start+source->size-1)%source->maxSize].value;
	return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue* source){
	assert(source != NULL);
	if (source->size == 0){
		return -1;
	}
	return source->queue[source->start].value;
}

double spBPQueueMaxValue(SPBPQueue* source){
	assert(source != NULL);
	if (source->size == 0){
		return -1;
	}
	return source->queue[(source->start+(source->size)-1)%source->maxSize].value;
}

bool spBPQueueIsEmpty(SPBPQueue* source) {
	assert(source != NULL);
	return (source->size == 0);
}

bool spBPQueueIsFull(SPBPQueue* source){
	assert(source != NULL);
		return (source->size == source->maxSize);
}

void printarry (SPBPQueue* spbqueue) {
	printf("\nStart printing...\n");
	for (int i = 0; i < spbqueue->size ;i++ ){
		printf ("index in array: %d index of element %d value of element %f \n",i,spbqueue->queue[(i+spbqueue->start)%spbqueue->maxSize].index,spbqueue->queue[(i+spbqueue->start)%(spbqueue->maxSize)].value);
	}
}


