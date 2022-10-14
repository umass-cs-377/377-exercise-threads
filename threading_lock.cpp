#include <iostream>
#include <assert.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *printing(void *arg){
	// simply prints out a string
	cout << "377 is a class!\n";
	return NULL;
}

void *truth(void *arg){
	// prints out a truth or a lie if the value
	// of the parameter is true or false, respectively
	bool value = *((bool*)(arg));
	int count = 1;
	pthread_mutex_lock(&mtx);
	while (count <= 10){
		if (value){
			cout << "#" << count << ": 377 is cool!\n";
		} else {
			cout << "#" << count << ": 377 is not cool...\n";
		}
		count++;
	}
	pthread_mutex_unlock(&mtx);
	return NULL;
}

int main() {
	pthread_t one, two, three;
	int rc;
	cout << "Beginning Threads.\n";

	// Setting up values to pass in as arguments
	bool truth_vals[2] = {false, true};
	
	// Creating Threads

	// Creates a thread that will run the truth function with parameter false
	rc = pthread_create(&one, NULL, truth, &(truth_vals[0]));
	assert(rc == 0); // Makes sure thread one was created successfully

	// Creates a thread that will run the printing function
	rc = pthread_create(&two, NULL, printing, NULL);
	assert(rc == 0); // Makes sure thread two was created successfully

	// Creates a thread that will run the truth function with parameter true
	rc = pthread_create(&three, NULL, truth, &(truth_vals[1]));
	assert(rc == 0); // Makes sure thread three was created successfully

	// Each of the below lines pauses the execution of
	// the main thread until the specified thread is finished.
	// Also it cleans up the thread, which prevents zombie threads from
	// using up system resources.
	rc = pthread_join(one, NULL); // Wait for thread one to finish
	assert(rc == 0); // Makes sure thread one ran successfully
	cout << "Thread #1 finished.\n";

	rc = pthread_join(two, NULL); // Wait for thread two to finish
	assert(rc == 0); // Makes sure thread two ran successfully
	cout << "Thread #2 finished.\n";

	rc = pthread_join(three, NULL); // Wait for thread three to finish
	assert(rc == 0); // Makes sure thread three ran successfully
	cout << "Thread #3 finished.\n";

	cout << "All threads finished.\n";

	return 0;
}
