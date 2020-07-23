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
#include "JackClient.h"

namespace AudioPluginJack
{

enum Param
{
  PORT,
  LAST
};

struct EffectData
{
  float params[LAST];
};

int InternalRegisterEffectDefinition( UnityAudioEffectDefinition& definition )
{
  int numparams        = LAST;
  definition.paramdefs = new UnityAudioParameterDefinition[numparams];
  RegisterParameter( definition, "Port", "", 0.0f, 64.0f, 0.0f, 1.0f, 1.0f, PORT, "Jack port" );
  return numparams;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK CreateCallback( UnityAudioEffectState* state )
{
  EffectData* data = new EffectData;
  memset( data, 0, sizeof( EffectData ) );
  state->effectdata = data;
  InitParametersFromDefinitions( InternalRegisterEffectDefinition, data->params );
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ReleaseCallback( UnityAudioEffectState* state )
{
  EffectData* data = state->GetEffectData<EffectData>();
  delete data;
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ProcessCallback( UnityAudioEffectState* state,
                                                               float* inbuffer,
                                                               float* outbuffer,
                                                               unsigned int length,
                                                               int inchannels,
                                                               int outchannels )
{
  EffectData* data = state->GetEffectData<EffectData>();

  // verify we have the same buffersize in unity and in jack
  if ( length != JackClient::getInstance().get_buffer_size() )
  {
    return UNITY_AUDIODSP_ERR_UNSUPPORTED;
  }

  // if we receive stereo we use only the left channel, since jack ports are only mono
  // otherwise pass the original buffer
  if ( inchannels == 2 )
  {
    std::vector<float> mono(length);
    for ( unsigned int i = 0, j = 0; i < length; i++, j += 2 )
    {
      mono[i] = inbuffer[j];
    }
    JackClient::getInstance().write_buffer( data->params[PORT], mono.data(), length );
  }
  else if ( inchannels == 1 )
  {
    JackClient::getInstance().write_buffer( data->params[PORT], inbuffer, length );
  }
  return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK SetFloatParameterCallback( UnityAudioEffectState* state, int index, float value )
{
  EffectData* data = state->GetEffectData<EffectData>();
  data->params[index]   = value;
  return UNITY_AUDIODSP_OK;
}
UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK GetFloatParameterCallback( UnityAudioEffectState* state, int index, float* value, char* )
{
  EffectData* data = state->GetEffectData<EffectData>();
  if ( value != NULL ) *value = data->params[index];
  return UNITY_AUDIODSP_OK;
}
int UNITY_AUDIODSP_CALLBACK GetFloatBufferCallback( UnityAudioEffectState*, const char*, float*, int ) { return UNITY_AUDIODSP_OK; }

} //! namespace AudioPluginJack

extern "C" UNITY_AUDIODSP_EXPORT_API bool CreateClient( unsigned int buffer_size,
                                                        unsigned int sample_rate )
{
  return AudioPluginJack::JackClient::getInstance().create_client( buffer_size, sample_rate );
}

extern "C" UNITY_AUDIODSP_EXPORT_API bool ActivateClient()
{
  return AudioPluginJack::JackClient::getInstance().activate();
}

extern "C" UNITY_AUDIODSP_EXPORT_API bool DestroyClient()
{
  return AudioPluginJack::JackClient::getInstance().destroy_client();
}

extern "C" UNITY_AUDIODSP_EXPORT_API bool RegisterPorts( unsigned int inputs, unsigned int outputs )
{
  return AudioPluginJack::JackClient::getInstance().register_ports_streams( inputs, outputs );
}

extern "C" UNITY_AUDIODSP_EXPORT_API void WriteBuffer( int channel, float* buffer, int size )
{
  AudioPluginJack::JackClient::getInstance().write_buffer( channel, buffer, size );
}

extern "C" UNITY_AUDIODSP_EXPORT_API void ReadBuffer( int channel, float* buffer, int size )
{
  AudioPluginJack::JackClient::getInstance().read_buffer( channel, buffer, size );
}

extern "C" UNITY_AUDIODSP_EXPORT_API void RegisterLogCallback( LogCallback callback )
{
  AudioPluginJack::JackClient::getInstance().register_log_callback( callback );
}