#include <iostream>
#include "SharedObject.h"
#include <stdio.h>
#include <ctime>
#include "Semaphore.h"
using namespace std;


struct MyShared {
	int threadID;
	int reportID;
	int elapsedTime;
	bool flag;
};

int main(void)
{
	cout << "I am a reader" << endl;
	// Access to sharedMemory and both semaphores
	Shared<MyShared> shared("sharedMemory");
	Semaphore s1("sOne", false);
	Semaphore s2("sTwo", false);

	MyShared* threadValues = shared.get();

	while (threadValues->flag != true) {
		s2.Wait();
		printf("\nReader Thread: %d %d %ld", threadValues->threadID, threadValues->reportID, threadValues->elapsedTime);
		s1.Signal();
	}
	std::endl(std::cout);

	return 0;
}
