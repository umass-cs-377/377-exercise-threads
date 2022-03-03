#include <iostream>
#include <assert.h>
#include <pthread.h> 
#include <chrono>
#include <mutex>

using namespace std;

pthread_mutex_t mtx;

void *printing(void *arg){
	//simply prints out a string
	cout << "377 is a class!\n";
	return NULL;
}

void *truth(void *arg){
	//prints out a truth or a lie if the value parameter is true or false, respectively
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

	//Setting up values to pass in as arguments
	bool truth_vals[2] = {false, true};
	
	//Creating Threads
	rc = pthread_create(&one, NULL, truth, &(truth_vals[0])); //Creates a thread that will run the truth() method with the parameter of 'false'
	rc = pthread_create(&two, NULL, printing, NULL); //Creates a thread that will run the printing() method
	rc = pthread_create(&three, NULL, truth, &(truth_vals[1])); //Creates a thread that will run the truth() method with the parameter of 'true'

	//Each of the below lines starts a thread, and pauses the execution of the main function until each of them is finished.
	rc = pthread_join(one, NULL); //Runs thread one
	assert(rc == 0); //Makes sure thread one ran correctly
	cout << "Thread #1 finished.\n";

	rc = pthread_join(two, NULL); //Runs thread two
	assert(rc == 0); //Makes sure thread two ran correctly
	cout << "Thread #2 finished.\n";

	rc = pthread_join(three, NULL); //Runs thread three
	assert(rc == 0); //Makes sure thread three ran correctly
	cout << "Thread #3 finished.\n";

	cout << "All threads finished.\n";

	return 0;
}