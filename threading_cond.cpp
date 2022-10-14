#include <iostream>
#include <assert.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv2 = PTHREAD_COND_INITIALIZER;
bool pt_done = false; //print_threads done
bool pa_done = false; //print_are done


void *print_threads(void *arg){
    cout << "threads" << endl;

    pthread_mutex_lock(&m1);
    pt_done = true;
    pthread_cond_signal(&cv1);
    pthread_mutex_unlock(&m1);
    return NULL;
}

void *print_are(void *arg){
    pthread_mutex_lock(&m1);
    while (!pt_done)  
        pthread_cond_wait(&cv1, &m1);
    cout << "are" << endl;
    pthread_mutex_unlock(&m1);
    return NULL;
}

void *print_cool(void *arg){

    cout << "cool!" << endl;
    
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;
    int rc;
	
    // Create the threads 
    rc = pthread_create(&t1, NULL, print_threads, NULL);
    assert(rc == 0);
    rc = pthread_create(&t2, NULL, print_are, NULL);
    assert(rc == 0);
    rc = pthread_create(&t3, NULL, print_cool, NULL);
    assert(rc == 0);
	
    // clean up
    rc = pthread_join(t1, NULL);
    assert(rc == 0);
    rc = pthread_join(t2, NULL);
    assert(rc == 0);
    rc = pthread_join(t3, NULL);
    assert(rc == 0);

    return 0;
}