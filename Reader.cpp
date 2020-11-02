#include <iostream>
#include "SharedObject.h"
#include <stdio.h>
#include <ctime>
using namespace std;

struct MyShared {
	int threadID;
	int reportID;
	time_t elapsedTime;
	bool flag;
	int delay;
};

int main(void)
{
	cout << "I am a reader" << endl;
	Shared<MyShared> shared("sharedMemory");
	MyShared* threadValues = shared.get();

	time_t currentTime;

	while (threadValues->flag != true) {
		sleep(2);
		printf("\nReader Thread: %d %d %ld", threadValues->threadID, threadValues->reportID, threadValues->elapsedTime);
	}
	std::endl(std::cout);

	return 0;
}