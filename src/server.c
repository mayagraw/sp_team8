#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h> //for threading , link with lpthread
#include "../include/common.h" 
#include "../include/db.h"
#include "../include/threadpool.h"

#define THREAD 10;
#define QUEUE 25;

pthread_mutex_t threadpool_lock;

int main()
{
    /*
    threadpool_t *thread_pool;

    pthread_mutex_init(&threadpool_lock, NULL);
    thread_pool = threadpool_create(THREAD, QUEUE, 0);
    if(!thread_pool){
        //pool creating failed, exit
        return -1;
    }*/
    /*
       Do the following initialization 
       1. Queues
       2. DB
       3. UI
       Add the following tasks 
       1. UI
       2. DB
       3. Accepting new connections
       4. Sending tasks
       5. Keep alive
       6. Thread for receiving on various FDs 
       Destruction
       1. DB
       2. Queue
       3. close all the FDs
       4. All threads
       */

    //threadpool_destroy(thread_pool,0);
    return 0;
}
