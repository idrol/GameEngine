#include <iostream>
#include <assert.h>
#include <Engine/Library/Common.h>
#include <Engine/Library/Container/Queue.hpp>

using namespace std;

void TestQueue();

int main() {

	TestQueue();

}



// ---------------------
// Container tests
// ---------------------


// 
// Queue
void TestQueue() {

	Queue<int> myQueue = Queue<int>(true, 5);

	int test;
	myQueue.Dequeue(test);

	printf("Testing queue\n");
	myQueue.Enqueue(1);
	myQueue.Enqueue(2);
	myQueue.Enqueue(3);

	assert(myQueue.Count() == 3);

	printf("Dequeueing\n");
	int output = 0;
	while(!myQueue.Dequeue(output) && myQueue.Count() > 0){}

	assert(output == 1 && myQueue.Count() == 2);

	while (!myQueue.Dequeue(output) && myQueue.Count() > 0) {}
	while (!myQueue.Dequeue(output) && myQueue.Count() > 0) {}
	while (!myQueue.Dequeue(output) && myQueue.Count() > 0) {}

	assert(output == 3 && myQueue.Count() == 0);
	
	printf("Testing mass-insert\n");

	myQueue.Clear();

	assert(myQueue.Count() == 0);

	int count = 100000;

	for(int i=0; i < count; i++)
		myQueue.Enqueue(i);

	assert(myQueue.Count() == count);

	for(int i=0; i < count; i++)
	{
		myQueue.Dequeue(output);
		assert_eq(output, i, "Unexpected value returned from dequeue");
	}
	assert(myQueue.Count() == 0);

	myQueue.Dequeue(output);
	myQueue.Dequeue(output);

	printf("Queue<T> - OK\n");
}


