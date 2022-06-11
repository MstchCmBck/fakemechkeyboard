// Code get from this example
// https://loulou.developpez.com/tutoriels/openal/premiers-pas/

#include "soundreader.h"
#include <stdio.h>


bool TEST_ERROR(const char* msg) {
    ALCenum error = alGetError();
    char err_msg[255] = "[%s] ";
    strcat(err_msg, msg);
    if (error != AL_NO_ERROR) {
        strcat(err_msg, " failed with error %d\n");
        fprintf(stderr, err_msg, __FILE__, error);
        return true;
    }
    else {
        strcat(err_msg, " succeeded\n");
        fprintf(stdout, err_msg, __FILE__);
        return false;
    }
}


// Open default sound output for openAL
bool initOpenAl() {
    ALCdevice* device = alcOpenDevice(NULL);
    TEST_ERROR("alcOpenDevice");
    if (!device) return false;

    ALCcontext* context = alcCreateContext(device, NULL);
    TEST_ERROR("alcCreateContext");
    if (!context) return false;

    if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}

    return true;
}


void shutdownOpenAl() {
    ALCcontext* context = alcGetCurrentContext();
    ALCdevice*  device  = alcGetContextsDevice(context);

    // Deactive the context
    alcMakeContextCurrent(NULL);
    TEST_ERROR("alcMakeContextCurrent");
    // Destroy the context
    alcDestroyContext(context);
    TEST_ERROR("alcDestroyContext");
    // CLose the device
    alcCloseDevice(device);
    TEST_ERROR("alcCloseDevice");
}


ALuint loadSound(const char* filename) {
    printf("Trying to open file named %s\n", filename);

    // Open of a sound file with libsndfile
    SF_INFO fileInfos;
    fileInfos.format = 0;
    SNDFILE* file = sf_open(filename, SFM_READ, &fileInfos);
    if (!file) {
        int error = sf_error(NULL);
        const char* error_str = sf_error_number(error);
        printf("sf_open() failed = %s\n", error_str);
        return 0;
    }

    // Get sample number and sample rate
    ALsizei sampleNbr  = (ALsizei) (fileInfos.channels * fileInfos.frames);
    ALsizei sampleRate = (ALsizei) fileInfos.samplerate;

    printf("Nbr of sample = %d\n", sampleNbr);
    printf("Sample rate = %d\n", sampleRate);

    short samplesArray[sampleNbr];

    if (sf_read_short(file, samplesArray, sampleNbr) < sampleNbr) {
        int error = sf_error(NULL);
        const char* error_str = sf_error_number(error);
        printf("sf_read_short failed = %s\n", error_str);
        return 0;
    }
    else {
        printf("sf_read_short succedded\n");
    }

    sf_close(file);

    // Get the number of channels from the fileInfos
    ALenum format;
    switch (fileInfos.channels) {
        case 1: format = AL_FORMAT_MONO16;
            break;
        case 2: format = AL_FORMAT_STEREO16;
            break;
        default:
            return 0;
    }

    // OpenAL buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    TEST_ERROR("alGenBuffers");

    // Fill the buffer with data from the file
    ALsizei bufferSize = sampleNbr * (ALsizei)sizeof(ALushort);
    alBufferData(
        buffer,
        format,
        samplesArray,
        bufferSize,
        sampleRate);
    TEST_ERROR("alBufferData");

    return buffer;
}


void playSound(ALuint buffer) {
    // Source creation
    ALuint source;
    alGenSources(1, &source);

    // Attach a buffer to our source
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    TEST_ERROR("alSourcei attachement");

    // Actual play
    alSourcePlay(source);
    TEST_ERROR("alSourcePlay");

    ALint Status;
    do
    {
        // Récupération et affichage de la position courante de lecture en secondes
        ALfloat Seconds = 0.f;
        alGetSourcef(source, AL_SEC_OFFSET, &Seconds);

        // Récupération de l'état du son
        alGetSourcei(source, AL_SOURCE_STATE, &Status);
    }
    while (Status == AL_PLAYING);

    // Deattach the buffer by setting the propietary AL_BUFFER to 0
    alSourcei(source, AL_BUFFER, AL_NONE);
    TEST_ERROR("alSourcei deatachement");
    alDeleteSources(1, &source);
    TEST_ERROR("alDeleteSources");
}
