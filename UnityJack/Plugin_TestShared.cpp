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

#include "AudioPluginUtil.h"
#include "TestSharedLib.cpp"

#define DEBUG_OUT

namespace UnityJackAudio
{

enum Param
{
  P_PARAM1,
  P_INDEX,
  P_NUM
};

struct EffectData
{
  float p[P_NUM];
  float tmpbuffer_in[1024];
  float tmpbuffer_out[1024];
};

int InternalRegisterEffectDefinition( UnityAudioEffectDefinition& definition )
{
  int numparams        = P_NUM;
  definition.paramdefs = new UnityAudioParameterDefinition[numparams];
  RegisterParameter( definition, "INDEX", "", 0.0f, 64.0f, 0.0f, 1.0f, 1.0f, P_INDEX, "User-defined parameter 1 (read/write)" );
  RegisterParameter( definition, "VOL", "", 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, P_PARAM1, "User-defined parameter 1 (read/write)" );

  return numparams;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK CreateCallback( UnityAudioEffectState* state )
{
  EffectData* data = new EffectData;
  memset( data, 0, sizeof( EffectData ) );
  state->effectdata = data;
  InitParametersFromDefinitions( InternalRegisterEffectDefinition, data->p );

  // TODO: this should not be neccesary since we are calling
  // client creation from C#
  // Get unique index on start
  //    data->p[P_INDEX] = (float)JackClient::getInstance().GenerateIndex();
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ReleaseCallback( UnityAudioEffectState* state )
{
  EffectData* data = state->GetEffectData<EffectData>();
  delete data;
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ProcessCallback( UnityAudioEffectState* state, float* inbuffer, float* outbuffer, unsigned int length,
                                                               int inchannels, int outchannels )
{
  EffectData* data = state->GetEffectData<EffectData>();

  for ( unsigned int n = 0; n < length; n++ )
  {
    for ( int i = 0; i < outchannels; i++ )
    {
      outbuffer[n * outchannels + i] = inbuffer[n * outchannels + i] * data->p[P_PARAM1];
    }
  }

  // WARNING! This works only depending on the framerate
  // configured in Unity and Jack.
  // The "Best Performance" setting gives us BUFSIZE = 1024 samples per channel
  // which corresponds to 1024 samples in Jack

#ifdef DEBUG_OUT
  if ( inchannels == 2 )
  {
    // downmix
    for ( unsigned int i = 0, j = 0; i < length * 2; i += 2 )
    {
      data->tmpbuffer_out[j++] = inbuffer[i] + inbuffer[i + 1];
    }

    JackClient::getInstance().SetData( data->p[P_INDEX], data->tmpbuffer_out );
  }
  else if ( inchannels == 1 )
  {
    JackClient::getInstance().SetData( data->p[P_INDEX], inbuffer );
  }
#else
  if ( inchannels == 2 )
  {
    // upmix
    JackClient::getInstance().GetData( data->p[P_INDEX], data->tmpbuffer_in );

    for ( int i = 0, j = 0; i < length * 2; i += 2 )
    {
      outbuffer[i]     = data->tmpbuffer_in[j++];
      outbuffer[i + 1] = outbuffer[i];
    }
  }
  else if ( inchannels == 1 )
  {
    JackClient::getInstance().GetData( data->p[P_INDEX], outbuffer );
  }
#endif

  //    std::cout << "Processing data " << length << " channels " << inchannels << std::endl;
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK SetFloatParameterCallback( UnityAudioEffectState* state, int index, float value )
{
  EffectData* data = state->GetEffectData<EffectData>();
  data->p[index]   = value;
  return UNITY_AUDIODSP_OK;
}
UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK GetFloatParameterCallback( UnityAudioEffectState* state, int index, float* value, char* )
{
  EffectData* data = state->GetEffectData<EffectData>();
  if ( value != NULL ) *value = data->p[index];
  return UNITY_AUDIODSP_OK;
}
int UNITY_AUDIODSP_CALLBACK GetFloatBufferCallback( UnityAudioEffectState*, const char*, float*, int ) { return UNITY_AUDIODSP_OK; }

} // namespace UnityJackAudio


extern "C" UNITY_AUDIODSP_EXPORT_API bool CreateClient( unsigned int bufferSize, unsigned int inputs, unsigned int outputs )
{
  return UnityJackAudio::JackClient::getInstance().createClient( bufferSize, inputs, outputs );
}

extern "C" UNITY_AUDIODSP_EXPORT_API bool DestroyClient() 
{
    return UnityJackAudio::JackClient::getInstance().destroyClient();
}

extern "C" UNITY_AUDIODSP_EXPORT_API void GetAllData( float* buffer )
{ 
    UnityJackAudio::JackClient::getInstance().GetAllData( buffer );
}

extern "C" UNITY_AUDIODSP_EXPORT_API void SetAllData( float* buffer ) 
{ 
    UnityJackAudio::JackClient::getInstance().SetAllData( buffer );
}

extern "C" UNITY_AUDIODSP_EXPORT_API void RegisterLogCallback( LogCallback callback )
{
  UnityJackAudio::JackClient::getInstance().RegisterLogCallback( callback );
}