#include "AudioPluginUtil.h"
#include "TestSharedLib.cpp"

#define DEBUG_OUT

namespace TestSharedStack
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
    float tmpbuffer_in[BUFSIZE];
    float tmpbuffer_out[BUFSIZE];

};

int InternalRegisterEffectDefinition(UnityAudioEffectDefinition& definition)
{
    int numparams = P_NUM;
    definition.paramdefs = new UnityAudioParameterDefinition[numparams];
    RegisterParameter(definition, "INDEX", "", 0.0f, 64.0f, 0.0f, 1.0f, 1.0f, P_INDEX, "User-defined parameter 1 (read/write)");
    RegisterParameter(definition, "VOL", "", 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, P_PARAM1, "User-defined parameter 1 (read/write)");

    return numparams;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK CreateCallback(UnityAudioEffectState* state)
{
    EffectData* data = new EffectData;
    memset(data, 0, sizeof(EffectData));
    state->effectdata = data;
    InitParametersFromDefinitions(InternalRegisterEffectDefinition, data->p);

    // TODO: this should not be neccesary since we are calling
    // client creation from C#
    // Get unique index on start
    data->p[P_INDEX] = (float)TestClass::getInstance().GenerateIndex();
    return UNITY_AUDIODSP_OK;
}


UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ReleaseCallback(UnityAudioEffectState* state)
{
    EffectData* data = state->GetEffectData<EffectData>();
    delete data;
    return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK ProcessCallback(UnityAudioEffectState* state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int outchannels)
{
    EffectData* data = state->GetEffectData<EffectData>();

    for (unsigned int n = 0; n < length; n++)
    {
        for (int i = 0; i < outchannels; i++)
        {
            outbuffer[n * outchannels + i] = inbuffer[n * outchannels + i] * data->p[P_PARAM1];
        }
    }
    
    // WARNING! This works only depending on the framerate
    // configured in Unity and Jack.
    // The "Best Performance" setting gives us BUFSIZE = 1024 samples per channel
    // which corresponds to 1024 samples in Jack
    
#ifdef DEBUG_OUT
    if (inchannels == 2)
    {
        //downmix
        for (int i = 0, j = 0; i < length * 2; i += 2)
        {
            data->tmpbuffer_out[j++] = inbuffer[i] + inbuffer[i+1];
        }
        
        TestClass::getInstance().SetData( data->p[P_INDEX], data->tmpbuffer_out);

        
    } else if (inchannels == 1) {
        TestClass::getInstance().SetData( data->p[P_INDEX], inbuffer );
    }
#else
    if (inchannels == 2)
    {
        // upmix
        TestClass::getInstance().GetData( data->p[P_INDEX], data->tmpbuffer_in);

        for (int i = 0,j=0; i < length * 2; i += 2)
        {
            outbuffer[i] = data->tmpbuffer_in[j++];
            outbuffer[i+1] = outbuffer[i];
        }
    } else if (inchannels == 1) {
        TestClass::getInstance().GetData( data->p[P_INDEX], outbuffer);
    }
#endif
    
//    std::cout << "Processing data " << length << " channels " << inchannels << std::endl;
    return UNITY_AUDIODSP_OK;
}

UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK SetFloatParameterCallback(UnityAudioEffectState* state, int index, float value)
{
    EffectData* data = state->GetEffectData<EffectData>();
    data->p[index] = value;
    return UNITY_AUDIODSP_OK;
}
UNITY_AUDIODSP_RESULT UNITY_AUDIODSP_CALLBACK GetFloatParameterCallback(UnityAudioEffectState* state, int index, float* value, char *valuestr)
{
    EffectData* data = state->GetEffectData<EffectData>();
    if (value != NULL) *value = data->p[index];
    return UNITY_AUDIODSP_OK;
}
int UNITY_AUDIODSP_CALLBACK GetFloatBufferCallback(UnityAudioEffectState* state, const char* name, float* buffer, int numsamples)
{
    return UNITY_AUDIODSP_OK;
}



} //!namespace

extern "C" UNITY_AUDIODSP_EXPORT_API bool CreateClient(int inputs, int outputs)
{
    return TestSharedStack::TestClass::getInstance().createClient(inputs, outputs);
}
extern "C" UNITY_AUDIODSP_EXPORT_API bool DestroyClient()
{
    return TestSharedStack::TestClass::getInstance().destroyClient();
}

extern "C" UNITY_AUDIODSP_EXPORT_API void GetAllData(float* buffer)
{
    TestSharedStack::TestClass::getInstance().GetAllData(buffer);
}

extern "C" UNITY_AUDIODSP_EXPORT_API void SetAllData(float* buffer)
{
    TestSharedStack::TestClass::getInstance().SetAllData(buffer);
}

