#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <time.h>
#include "Blockable.h"
#include <ctime>
#include "Semaphore.h"
using namespace std;

// Initialize two semaphores
Semaphore s1("sOne", 1, true);
Semaphore s2("sTwo", 1, true);

struct MyShared {
	int threadID;
	int reportID;
	int elapsedTime;
	bool flag;
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
		int reportID = 1;


		while (true)
		{
			time_t currentTime = time(NULL);
			sleep(delay);
			s1.Wait();
			shared->threadID = threadNum;
			shared->reportID = reportID++;
			shared->elapsedTime = time(NULL) - currentTime;
			s2.Signal();

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
		//create thread using user input ....
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