#pragma once
#ifdef __APPLE__
#include <CoreAudio/AudioHardware.h>
#include <iostream>
#endif
#include <stdint.h>
#include <iostream>
#include <string> // SylR

#ifndef __APPLE__
#define _STDINT_H
#endif
#include <jack/jack.h>
#include <jack/ringbuffer.h>


#if WIN32 || UNITY_WIN
#define UNITY_AUDIODSP_EXPORT_API __declspec(dllexport)
#else
#define UNITY_AUDIODSP_EXPORT_API
#endif

#define MAX_CHANNELS 64

typedef jack_default_audio_sample_t sample_t;

typedef void(*FuncPtr)(const char *);

#define PRIu32 "lu" 

static FuncPtr Debug;
#define _BUFFER_FRAMES 1024
//#define _SAMPLE_RATE 44100
#define _SAMPLE_RATE 48000
//#define _SAMPLE_RATE 22050
#define UNITY

// channels, bufferBytes, buffer
typedef void(*NativeAudioCallback)(unsigned int channels, unsigned int bufferBytes, sample_t*buffer);

inline void f2i_array(float *fptr, short *buffer, int count)
{
	while (count)
	{
		count--;
		buffer[count] = lrintf(fptr[count]);
	}
}

#ifdef UNITY


//void LOG(const char* l, int id){

	//std::string s = std::to_string(id);

//	char result[100];   // array to hold the result.
//
//	strcpy(result, l); // copy string one into the result.
//	strcat(result, s.c_str()); // append string two to the result.
//
//	Debug(result);

//}
#endif


extern "C"
{
	UNITY_AUDIODSP_EXPORT_API int startNativeAudio(int inchannels, int outchannels); // return success or not
	UNITY_AUDIODSP_EXPORT_API void stopNativeAudio();

	UNITY_AUDIODSP_EXPORT_API int getAudioBuffer(float *buffer);

	UNITY_AUDIODSP_EXPORT_API int setAudioBuffer(float *buffer);

	UNITY_AUDIODSP_EXPORT_API const char* getAudioDeviceName(unsigned int deviceId);
	UNITY_AUDIODSP_EXPORT_API unsigned int getAudioDeviceCount();
	UNITY_AUDIODSP_EXPORT_API unsigned int getDefaultAudioDevice();

	UNITY_AUDIODSP_EXPORT_API unsigned int getAudioInputChannels(unsigned int deviceId);

	UNITY_AUDIODSP_EXPORT_API unsigned int getSampleRate();
	UNITY_AUDIODSP_EXPORT_API unsigned int getBufferFrames();

//	UNITY_AUDIODSP_EXPORT_API void SetDebugFunction(FuncPtr fp)
//	{
//		Debug = fp;
//	}

	UNITY_AUDIODSP_EXPORT_API void listDevices();
	// send/receive
	UNITY_AUDIODSP_EXPORT_API void sendMessageToSSR();
}
