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

#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 )
#define UNITY_WIN 1
#elif defined( __MACH__ ) || defined( __APPLE__ )
#define UNITY_OSX 1
#elif defined( __ANDROID__ )
#define UNITY_ANDROID 1
#elif defined( __linux__ )
#define UNITY_LINUX 1
#endif

#include <memory> //for std::unique_ptr
#include <stdio.h>

#if UNITY_OSX | UNITY_LINUX
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif UNITY_WIN
#include <windows.h>
#define _STDINT_H // for jack int definition
#endif

#if defined( __GNUC__ ) || defined( __SNC__ )
#define TESTSHARED_ALIGN( val ) __attribute__( ( aligned( val ) ) ) __attribute__( ( packed ) )
#elif defined( _MSC_VER )
#define TESTSHARED_ALIGN( val ) __declspec( align( val ) )
#else
#define TESTSHARED_ALIGN( val )
#endif

#include "Logger.h"
#include "InternalJackClient.h"
#include <array>

// #define TRACKS 16
// #define BUFSIZE 1024
template <typename T, int M, int N>
using array2d = std::array<std::array<T, N>, M>;

namespace UnityJackAudio
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

  int GenerateIndex() { return _index++; }
  int SetAllData( float* buffer )
  {

    if ( !initialized ) return 0;

    client->setAudioBuffer( buffer );
    return 0;
  }

  int SetData( int idx, float* buffer )
  {

    if ( !initialized ) return 0;

    for ( int i = 0; i < mBufferSize; i++ )
    {
      mixedBuffer[( i * _outputs ) + idx] = buffer[i];
    }

    // Increase the index until the mixed buffer is filled.
    track++;

    // if filled send to ringbuffer, restart index
    if ( track == _outputs )
    {
      client->setAudioBuffer( mixedBuffer );
      track = 0;
    }

    return 0;
  }

  void GetAllData( float* buffer )
  {
    if ( !initialized ) return;
    client->getAudioBuffer( buffer );
  }

  bool createClient( int bufferSize, int inputs, int outputs )
  {
    if ( !initialized )
    {
      LOG("Creating Client with: ");
      LOG("bufferSize: " << bufferSize);
      LOG("inputs: " << inputs);
      LOG("outputs: " << outputs);

      _inputs     = inputs;
      _outputs    = outputs;
      mBufferSize = bufferSize;

      client.reset( new InternalJackClient( "Unity3D", bufferSize, inputs, outputs ) );

      if ( 0 != client->initialize() )
      {
        ERROR("Could not initialize client");
        return -1;
      }

      mixedBuffer   = ( float* )malloc( _outputs * mBufferSize * sizeof( float ) );
      mixedBufferIn = ( float* )malloc( _inputs * mBufferSize * sizeof( float ) );

      initialized = true;
    }
    return initialized;
  }

  bool destroyClient()
  {
    std::cout << "Destroying" << std::endl;
    if ( initialized )
    {
      initialized = false; // important: initialized flag must be false before resetting the client.
      client.reset();
      free( mixedBuffer );
      free( mixedBufferIn );
    }
    return initialized;
  }

private:
  JackClient()
  {
    logger = std::make_unique<Logger>();
  }

public:
  JackClient( JackClient const& ) = delete;
  void operator=( JackClient const& ) = delete;

public:
  void RegisterLogCallback( LogCallback callback )
  { 
    if ( logger )
    {
      logger->RegisterLogCallback(callback); 
    }
  }

private:
  // TODO: use better this? http://stackoverflow.com/questions/35008089/elegantly-define-multi-dimensional-array-in-modern-c
  std::unique_ptr<InternalJackClient> client;
  std::unique_ptr<Logger> logger;
  // float mixedBuffer[TRACKS * BUFSIZE];
  // float mixedBufferIn[TRACKS * BUFSIZE];
  float* mixedBuffer;
  float* mixedBufferIn;

  int  mBufferSize = 0;
  int  track;
  bool initialized = false;
  int  _inputs, _outputs;
  int  _index;
};

} // namespace UnityJackAudio
