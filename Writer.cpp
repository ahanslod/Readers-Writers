#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <time.h>
#include "Blockable.h"
#include <ctime>
using namespace std;


struct MyShared{
	int threadID;
	int reportID;
	time_t elapsedTime;
	bool flag;
	int delay;
};

class WriterThread : public Thread {
public:
	int threadNum;
	int delay;
	bool flag;

	WriterThread(int in) :Thread(8 * 1000) {
		this->threadNum = in; //or whatever you want/need here
	}

	virtual long ThreadMain(void) override {
		//declare shared memory var so this thread can access it
		Shared<MyShared> shared("sharedMemory");

		// Initialize values
		shared->reportID = 0;


		while (true)
		{
			time_t epoch = time(NULL);
			sleep(delay);
			//time_t currentTime;
			//write to shared memory
			shared->threadID = threadNum;
			shared->reportID++;
			shared->elapsedTime = time(NULL) - epoch;
			shared->delay = delay;

			if (flag) {//Exit loop to end the thread
				shared->flag = flag;
				break;
			}
		}
		return 0;
	}

};

int main(void)
{
	string input = "";
	int delay;
	int threadCount = 1;
	
	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory

	cout << "I am a Writer" << endl;

	WriterThread *thread1;

	while (input != "no") {
		//create thread using user input
		cout << "Would you like to create a writer thread?" << endl;
		cin >> input;

		if (input == "yes") {
			cout << "What is the delay time for this thread?" << endl;
			cin >> delay;
			thread1 = new WriterThread(threadCount++);
			thread1->flag = false;
			thread1->delay = delay;

		}
		else if (input == "no") {
			thread1->flag = true;
			delete thread1;
		}
	}
}