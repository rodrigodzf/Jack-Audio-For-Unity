// Copyright (C) 2016  Rodrigo Diaz
// 
// This file is part of JackAudioUnity.
// 
// JackAudioUnity is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// JackAudioUnity is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with JackAudioUnity.  If not, see <http://www.gnu.org/licenses/>.
// 

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#define UNITY_WIN 1
#elif defined(__MACH__) || defined(__APPLE__)
#define UNITY_OSX 1
#elif defined(__ANDROID__)
#define UNITY_ANDROID 1
#elif defined(__linux__)
#define UNITY_LINUX 1
#endif

#include <stdio.h>
#include <iostream>
#if UNITY_OSX | UNITY_LINUX
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <unistd.h>
    #include <string.h>
#elif UNITY_WIN
    #include <windows.h>
#endif

#if defined(__GNUC__) || defined(__SNC__)
#define TESTSHARED_ALIGN(val) __attribute__((aligned(val))) __attribute__((packed))
#elif defined(_MSC_VER)
#define TESTSHARED_ALIGN(val) __declspec(align(val))
#else
#define TESTSHARED_ALIGN(val)
#endif

//#include "UnityNativeAudio.h"
#include "InternalJackClient.h"
#include <array>

#define TRACKS 16
#define BUFSIZE 512
template <typename T, int M, int N> using array2d = std::array<std::array<T, N>, M>;

namespace TestSharedStack
{

class JackClient 
{
    
public:
    static JackClient& getInstance()
    {
        static JackClient instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    
    int GenerateIndex() {
        return _index++;
    }
    int SetAllData(float* buffer) {
        
        if (!initialized) return 0;

        client->setAudioBuffer(buffer);
        return 0;
    }
    
    int SetData(int idx, float* buffer) {
        
        if (!initialized) return 0;
        
        //idx + i*T
        //  0 + 0*2 = 0
        //  0 + 1*2 = 2
        //  0 + 2*2 = 4
        
        //  1 + 0*2 = 1
        //  1 + 1*2 = 3
        //  1 + 2*2 = 5
        
        for (int i = 0; i < BUFSIZE; i++) {
            mixedBuffer[(i * _outputs) + idx] = buffer[i];
        }
        
        // Increase the index until the mixed buffer is filled.
        track++;

        // if filled send to ringbuffer, restart index
        if (track == _outputs) {
            client->setAudioBuffer(mixedBuffer);
            track = 0;
        }
        
        return 0;
    }
    
    void GetAllData(float* buffer) {
        if (!initialized) return;
        client->getAudioBuffer(buffer);
    }
    

    int GetData(int idx, float* buffer) {
    
        if (!initialized) return 0;

        client->getAudioBuffer(mixedBufferIn);
        
        for (int i = 0; i < BUFSIZE; i++) {
            buffer[i] = mixedBufferIn[(i * _inputs) + idx];
        }
        
        return 0;
    }
    
    bool createClient(int inputs, int outputs)
    {
        if (!initialized){
            std::cout << "Creating Client " << inputs << " " << outputs << std::endl;
            _inputs = inputs;
            _outputs = outputs;
            client.reset(new InternalJackClient("Unity3D",inputs,outputs));
            initialized = true;

        }
        return initialized;
    }
    
    bool destroyClient()
    {
        std::cout << "Destroying" << std::endl;
        if (initialized) {
            initialized = false; // important: initialized flag must be false before resetting the client.
            client.reset();
        }
        return initialized;
    }
    
    
private:
    
    JackClient() {
        std::cout << "Trying to create" << std::endl;
    }
    

public:
    JackClient(JackClient const&) = delete;
    void operator=(JackClient const&)  = delete;

private:

    // TODO: use better this? http://stackoverflow.com/questions/35008089/elegantly-define-multi-dimensional-array-in-modern-c
    std::unique_ptr<InternalJackClient> client;
    float mixedBuffer[TRACKS * BUFSIZE];
    float mixedBufferIn[TRACKS * BUFSIZE];
    int foo = 5;
    int track;
    bool initialized;
    int _inputs, _outputs;
    int _index;
};
    
} // !namespace TestSharedStack


