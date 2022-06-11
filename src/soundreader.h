#include <stdbool.h>
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

bool   initOpenAl();
void   shutdownOpenAl();
ALuint loadSound(const char* filename);
void   playSound(ALuint buffer);
