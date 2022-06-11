#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "soundreader.h"


void initiateAllThreads(
    pthread_t* threadArray,
    int64_t threadNumber);
void joinAllThreads(
    pthread_t* threadArray,
    int64_t threadNumber);
int64_t getNumberOfCore();
void keyPressed(void* buffer);
