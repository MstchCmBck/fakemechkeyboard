#include "keythread.h"
#include <unistd.h>
#include <stdio.h>

#define FINGER_NUMBER 10

typedef struct {
     void* buffer;
} Task;

Task taskQueue[FINGER_NUMBER];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t  condQueue;


// The routine use by each thread
// Intended to ve updated to play different sound
void* executeTask(void* buffer) {
    ALuint soundFile = *(ALuint*) buffer;
    printf("buffer = %d\n", soundFile);

    while(1) {
        int c = getc(stdin);
        printf("printer char = %c\n", c);
        playSound(soundFile);
    }

    return NULL;
}


// void executeTask(Task* task) {
//     task->taskFunction(task->buffer);
// }


// Add a task to the queue
// Should be called each time a key is pressed
void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);

    pthread_cond_signal(&condQueue);
}


// It's here that we start a thread if our
// taskQueue has been filled
void* startAThread() {
    while(1) {
        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        Task task = taskQueue[0];
        // Move all the queue up
        for (int i = 0; i < taskCount - 1; i++) {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);

        executeTask(&task);
    }
}


void keyPressed(void* buffer) {
    Task task;
    task.buffer = buffer;
    submitTask(task);
}


// Instentiate our thread pool
void initiateAllThreads(pthread_t* threadArray, int64_t threadNumber) {
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);

    for (int i = 0; i < threadNumber; i++) {
        if (pthread_create(&threadArray[i], NULL, &startAThread, NULL) != 0) {
            printf("Creation of thread %d failed\n", i);
        }
    }
}


// To end all of our threads
void joinAllThreads(pthread_t* threadArray, int64_t threadNumber) {

    for (int i = 0; i < threadNumber; i++) {
        if (pthread_join(threadArray[i], NULL)) {
            printf("Join of thread %d\n", i);
        }
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
}


// Get the number of core
// We use that to get the number of thread we can run
int64_t getNumberOfCore() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

