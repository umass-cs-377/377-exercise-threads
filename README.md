# COMPSCI 377 LAB: Threading

## Purpose

This exercise is designed to cover threads and how to use gdb to debug threads. Please make sure that all of your answers to questions in these labs come from work done in the Edlab environment – otherwise, they may be inconsistent results and will not receive points. Please read through this lab and follow the instructions. After you do that, visit Gradescope and complete the questions associated with this lab by the assigned due date.

## Setup

Once you have logged in to Edlab, you can clone this repo using

```bash
git clone https://github.com/umass-cs-377/377-exercise-threads.git
```

Then you can use `cd` to open the directory you just cloned:

```bash
cd 377-exercise-threads
```

This repository includes a `Makefile` that allows you to locally compile and run all the sample code listed in this tutorial. You can compile them by running `make`. Feel free to modify the source files yourself, after making changes you can run `make` again to build new binaries from your modified files. You can also use `make clean` to remove all the built files, this command is usually used when something went wrong during the compilation so that you can start fresh.

## Part 1: Threads

With many programs, it can be advantageous to have multiple threads of execution running at once (for example, a simple game server could be running several games at a time, one per thread, and possibly multiple players playing a game, each of those a thread). To this end, we use threading, which allows us to run multiple processes from the same original process. We can make multiple threads conduct different operations, run simultaneously, or wait for each other to finish. Please look at the code below, and read the commented sections:

**`threading.cpp`**

```c
#include <iostream>
#include <assert.h>
#include <pthread.h>
#include <chrono>

using namespace std;

void *printing(void *arg){
	// simply prints out a string
	cout << "377 is a class!\n";
	return NULL;
}

void *truth(void *arg){
	// prints out a truth or a lie if the value parameter is true or false,
	// respectively
	bool value = *((bool*)(arg));
	int count = 1;
	while (count <= 10){
		if (value){
			cout << "#" << count << ": 377 is cool!\n";
		} else {
			cout << "#" << count << ": 377 is not cool...\n";
		}
		count++;
	}
	return NULL;
}

int main() {
	pthread_t one, two, three;
	int rc;
	cout << "Beginning Threads.\n";

	// Setting up values to pass in as arguments
	bool truth_vals[2] = {false, true};

	// Creating Threads

	// Creates a thread that will run the truth() method with
	// the parameter of 'false'
	rc = pthread_create(&one, NULL, truth, &(truth_vals[0]));

	//Creates a thread that will run the printing() method
	rc = pthread_create(&two, NULL, printing, NULL);

  // Creates a thread that will run the truth() method with
	// the parameter of 'true'
	rc = pthread_create(&three, NULL, truth, &(truth_vals[1]));

	// Each of the below lines starts a thread, and pauses the execution
	// of the main function until each of them is finished.
	rc = pthread_join(one, NULL); //Runs thread one
	assert(rc == 0); //Makes sure thread one ran correctly
	cout << "Thread #1 finished.\n";

	// Joins thread two
	rc = pthread_join(two, NULL);

	// Makes sure thread two ran correctly
	assert(rc == 0);
	cout << "Thread #2 finished.\n";

 	// Joins thread three
	rc = pthread_join(three, NULL);

	// Makes sure thread three ran correctly
	assert(rc == 0);

	cout << "Thread #3 finished.\n";
	cout << "All threads finished.\n";

	return 0;
}
```

As can be seen, using `pthread_join()` pauses the main method, but does not necessarily pause the other threads from running since they were created before `pthread_join()` is called. However, the main method will wait until thread one is finished to go past `rc = pthread_join(one, NULL)`, until thread two is finished to go past `rc = pthread_join(two, NULL)`, and until thread three is finished to go past `rc = pthread_join(three, NULL)`.

## Part 2: GDB

You have seen the basics of `gdb` before. In this exercise we will be focused on using `gdb` to debug programs that use multiple threads. To run `gdb` on a process that uses multiple threads, simply open it with gdb like any other executable (`gdb PROGRAM`, where `PROGRAM` is the name of the executable). When you do so, you should be able to see output as such:

`[New Thread 0x7f85e5bce700 (LWP 364)]`

As can be seen above, the memory address is shown for the created thread in the form 0x\_\_\_\_, and the LWP address for the thread is shown after LWP. The LWP address of a thread isn’t something we will work with in this class, but it useful to know that it is the identifier assigned to the thread by the operating system. In the above image, the address of the thread is `0x7f85e5bce700`, and the LWP of the thread is `364`.

We can also find out more information about threads through our use of `gdb`. The `info threads` command will list the info in the screenshot for all currently running threads in the process, as well as the id of its originating thread (the thread that created the thread). The thread we are currently in will have an asterisk to its left. Threads that request time to wait will also show the amount of time they requested and need – the threads in `threading.cpp` will do this when run via `gdb`. Each of the threads also has a local ID listed on the right – using `gdb`, we can switch between these threads with the command `thread ID`, where `ID` is the id of the thread we are trying to switch to.

## Part 3: Mutex

Threads can be extremely useful, but they can also encounter errors when they attempt to modify the same memory as one another. For example, if two threads each try to increment a value by 1, it may be that both operations occur at the same time, and the value is only incremented once instead of the two times it should be incremented instead. For this reason, we use various techniques to ensure that critical data components are not modified outside of their desired scope. One of these structures is called a lock, which is shared between threads and acts as per its name to prevent other threads from accessing sensitive data while it is locked. Please look at the modified `threading.cpp` below, called `threading_lock.cpp`:

**`threading_lock.cpp`**

```c
#include <iostream>
#include <assert.h>
#include <pthread.h>
#include <chrono>

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
```

As you can see when running this code, thread `one` or `three` will always finish printing out its 10 statements before the other prints out any of its 10 statements. This is because when we lock `mtx` at the start of `truth()`, it prevents further calls of `truth()` to progress past that point until we call `pthread_mutex_unlock(&mtx)` from the same thread that locked it. In essence, this allows us to stop any threads that rely on `mtx`, ensuring that certain pieces of code do not run at the same time as one another even if we want multiple processes to be running simultaneously.
