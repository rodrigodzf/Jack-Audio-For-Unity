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
                                                               int /*inchannels*/,
                                                               int outchannels )
{
  EffectData* data = state->GetEffectData<EffectData>();
  for ( unsigned int n = 0; n < length; n++ )
  {
    for ( int i = 0; i < outchannels; i++ )
    {
      outbuffer[n * outchannels + i] = inbuffer[n * outchannels + i] * data->p[P_PARAM1];
    }
  }
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