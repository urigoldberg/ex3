#include "SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Checks Create
//Checks Enqueue
//Checks Dequeue
//Checks clear
//Checks MinValue
//Checks MaxValue
//Checks peek
bool Test01() {
	//Generate random size for SPBPQueue, size at least 2
	srand(time(NULL));
	int sizeofqueue = rand()%100+2;

	//Create an empty queue
	SPBPQueue* QueueTest = spBPQueueCreate(sizeofqueue);

	//Adds elements until full capacity, saves lowest & highest values
	int index = rand()%1000;
	double value =(double)(rand()%1000);
	int max = value;
	int min = value;
	for (int i = 0; i < sizeofqueue; i++) {
		spBPQueueEnqueue(QueueTest,index,value);
		if (value > max) {
			max = value;
		}
		if (value < min) {
			min = value;
		}
		index = rand()%1000;
		value =(double)(rand()%1000);
	}

	//Assert Min Max queue are Min Max values
	ASSERT_TRUE(spBPQueueMinValue(QueueTest) == min);
	ASSERT_TRUE(spBPQueueMaxValue(QueueTest) == max);

	//Dequeue all elements
	//Each one has to have higher value than his prev
	//Each one has to be lower than max and higher than min
	BPQueueElement* res = (BPQueueElement*)malloc(sizeof(res));
	spBPQueuePeek(QueueTest,res);
	int prevValue = res->value;
	SP_BPQUEUE_MSG Deq2 = spBPQueueDequeue(QueueTest);
	while (Deq2 == SP_BPQUEUE_SUCCESS) {
		spBPQueuePeek(QueueTest,res);
		ASSERT_TRUE(res->value >= prevValue);
		ASSERT_TRUE(res->value <= max);
		ASSERT_TRUE(res->value >= min);
		prevValue = res->value;
		Deq2 = spBPQueueDequeue(QueueTest);

	}

	//Enqueue and Dequeue randomly, at least 6 times
	int howManyLoops = rand()%500+6;
	for (int i = 0; i < howManyLoops; i++){

		//clear when arrive to middle
		//asserts clear
		if (i == howManyLoops/2){
			spBPQueueClear(QueueTest);
			ASSERT_TRUE(spBPQueueIsEmpty(QueueTest));
		}

		//Adding Elements
		int howManyAdds = rand()%200;
		for (int j = 0; j < howManyAdds; j++){
			spBPQueueEnqueue(QueueTest,rand()%100,(double)(rand()%1000));
		}


		//Delete Elements
		for (int j = 0; j < howManyAdds; j++){
			spBPQueueDequeue(QueueTest);
		}
	}

	//Check queque while not empty
	if (!spBPQueueIsEmpty(QueueTest)){
		spBPQueuePeek(QueueTest,res);
		int prevValue = res->value;
		SP_BPQUEUE_MSG Deq2 = spBPQueueDequeue(QueueTest);
		while (Deq2 == SP_BPQUEUE_SUCCESS) {
			spBPQueuePeek(QueueTest,res);
			ASSERT_TRUE(res->value >= prevValue);
			prevValue = res->value;
			Deq2 = spBPQueueDequeue(QueueTest);
			}
	}

	//free resources
	spBPQueueDestroy(QueueTest);
	free(res);
	return true;
}


//Checks Copy
//Checks spBPQueueSize
//Checks spBPQueueGetMaxSize
//Checks isEmpty
//Checks isFull
//Checks peek
//Checks peeklast

bool Test02() {
	//Create a new empty queue and asserts its empty
	SPBPQueue* QueueTest = spBPQueueCreate(4);
	ASSERT_TRUE(spBPQueueIsEmpty(QueueTest));

	//Create the same queue as the queue in the example
	spBPQueueEnqueue(QueueTest,5,1);
	spBPQueueEnqueue(QueueTest,5,2);
	spBPQueueEnqueue(QueueTest,2,10);
	spBPQueueEnqueue(QueueTest,3,2.1);
	spBPQueueEnqueue(QueueTest,7,2.5);
	spBPQueueEnqueue(QueueTest,100,20);
	//Queue should look like this (<1,5>,<2,5>,<2.1,3>,<2.5,7>)

	//Asserts size is 4
	//Asserts maxSize is 4
	//Asserts isFull function
	ASSERT_TRUE(spBPQueueGetMaxSize(QueueTest)==4);
	ASSERT_TRUE(spBPQueueSize(QueueTest)==4);
	ASSERT_TRUE(spBPQueueIsFull(QueueTest));

	//Asserts peeklast
	BPQueueElement* elem = (BPQueueElement*)malloc(sizeof(elem));
	spBPQueuePeekLast(QueueTest,elem);
	ASSERT_TRUE(elem->value==2.5);

	//create copy
	SPBPQueue* QueueTestCopy = spBPQueueCopy(QueueTest);

	//Verifying copy is not the original queue
	ASSERT_TRUE(QueueTest != QueueTestCopy);
	spBPQueueDequeue(QueueTestCopy);
	spBPQueueDequeue(QueueTestCopy);
	spBPQueueDequeue(QueueTestCopy);

	//Copy should look like this (<2.5,7>)
	//Asserts Copy looks like this (<2.5,7>)

	spBPQueuePeek(QueueTestCopy,elem);
	spBPQueueDequeue(QueueTestCopy);
	//printf("%d %f",elem->index,elem->value);
	ASSERT_TRUE(elem->index == 7);
	//ASSERT_TRUE(elem->value == 2.5);
	ASSERT_TRUE(spBPQueueIsEmpty(QueueTestCopy));

	//free resources
	spBPQueueDestroy(QueueTest);
	spBPQueueDestroy(QueueTestCopy);
	free(elem);
	return true;

}


/**int main() {
	RUN_TEST(Test01);
	RUN_TEST(Test02);
	return 0;
}*/


