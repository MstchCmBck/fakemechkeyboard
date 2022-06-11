#include <stdio.h>
#include "soundreader.h"
#include "keythread.h"


int main() {
    initOpenAl();
    // Create an array of bufferized sound
    ALuint buffer = loadSound("/home/damien/Music/startupsound/Windows98.flac");
    // Use this array with keyPressed()

    int64_t coreNbr = getNumberOfCore();
    pthread_t threadArray[coreNbr];
    initiateAllThreads(threadArray, coreNbr);

    // TODO
    // Function to look at :
    //   + startAThread
    //   + keyPressed
    //   + submitTask
    //   + executeTask

    while(1) {
        getchar();
        keyPressed(&buffer);
    }

    joinAllThreads(threadArray, coreNbr);
    shutdownOpenAl();

    return 0;
}
