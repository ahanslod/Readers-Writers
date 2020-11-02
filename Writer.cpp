#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <time.h>
#include "Blockable.h"
#include <ctime>
using namespace std;


struct MyShared {
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

	Shared<MyShared> shared("sharedMemory", true);
	vector<WriterThread*> threads;

	cout << "I am a Writer" << endl;

	WriterThread* thread;

	while (input != "no") {
		cout << "Would you like to create a writer thread?" << endl;
		cin >> input;

		if (input == "yes") {
			cout << "What is the delay time for this thread?" << endl;
			cin >> delay;

			thread = new WriterThread(threadCount++);
			thread->flag = false;
			thread->delay = delay;
			threads.push_back(thread);
		}
		else if (input == "no") {
			for (int i = 0; i < threads.size(); i++) {
				threads[i]->flag = true;
				delete threads[i];
			}
		}
	}
}