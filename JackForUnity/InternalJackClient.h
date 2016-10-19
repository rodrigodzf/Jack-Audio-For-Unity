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

#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include <jack/types.h>

#include <iostream>
#include <string>
#include <stdexcept>  // for std::runtime_error
#include <vector>     // for std::vector

#define RINGBUF_SIZE 4096
class InternalJackClient
{

public:
    typedef jack_default_audio_sample_t sample_t;
    typedef jack_nframes_t              nframes_t;
    typedef jack_port_t                 port_t;

    InternalJackClient(const std::string name = "Unity3D", const int inputs = 2,
        const int outputs = 2)
    : mClientName(name)
    , mClient(nullptr)
    , mInputs(inputs)
    , mOutputs(outputs)
    {

        jack_status_t status;
        mClient = jack_client_open(name.c_str(), JackNullOption, &status);
        if (!mClient) throw std::runtime_error("Cannot create the client.");

        /* create the ringbuffers */
        _rbout = jack_ringbuffer_create(mOutputs * sizeof(jack_default_audio_sample_t) * RINGBUF_SIZE);
        _rbin = jack_ringbuffer_create(mInputs * sizeof(jack_default_audio_sample_t) * RINGBUF_SIZE);


        /* tell the JACK server to call `process()' whenever
        there is work to be done.
        */

        jack_set_process_callback(mClient, InternalJackClient::Process, this);

        /* tell the JACK server to call `jack_shutdown()' if
        it ever shuts down, either entirely, or if it
        just decides to stop calling us.
        */

        jack_on_shutdown(mClient, InternalJackClient::Shutdown, this);


        //allocate ports
        if (mInputs > 0){
            for(unsigned int i = 0; i < mInputs; i++){
                std::string portname = "in";
                portname.append(std::to_string(i));
                mInputPorts.push_back(
                        jack_port_register (mClient, portname.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0));
            }
            //reserve the data for the jack callback buffers
            for(unsigned int i = 0; i < mInputPorts.size(); i++)
                mIn.push_back(NULL);
        } 
        if (mOutputs > 0){
            for(unsigned int i = 0; i < mOutputs; i++){
                std::string portname = "out";
                portname.append(std::to_string(i));
                mOutputPorts.push_back(
                        jack_port_register (mClient, portname.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0));
            }
            //reserve the data for the jack callback buffers
            for(unsigned int i = 0; i < mOutputPorts.size(); i++)
                mOut.push_back(NULL);
        } 

        mBufferFrames   = jack_get_buffer_size(mClient);
        mSampleRate     = jack_get_sample_rate(mClient);
        
        mOutBufferBytes = mBufferFrames * mOutputs * sizeof(sample_t);
        mInBufferBytes = mBufferFrames * mInputs * sizeof(sample_t);

        
    	if (jack_activate(mClient) != 0) throw std::runtime_error("Cannot activate the client");

        
    }

    virtual ~InternalJackClient()
    {
      if (mClient)
      {
        jack_deactivate(mClient);
        jack_client_close(mClient);
          
      }
    }

    void setAudioBuffer(sample_t *buffer)
    {
        if (mClient == nullptr) return; // This might be called before the client deinitializes

        if (jack_ringbuffer_write_space(_rbout) > 0) jack_ringbuffer_write(_rbout, (char*)buffer, mOutBufferBytes);
    }
    
    void getAudioBuffer(sample_t *buffer)
    {
        if (mClient == nullptr) return; // This might be called before the client deinitializes

        if (jack_ringbuffer_read_space(_rbin) > 0) jack_ringbuffer_read(_rbin, (char*)buffer, mInBufferBytes);
    }

    static int Process(jack_nframes_t nframes, void *arg)
    {
        InternalJackClient *client = (InternalJackClient *)arg;
        //get the input and output buffers
        for (unsigned int i = 0; i < client->mInputs; i++)
            client->mIn[i] = (jack_default_audio_sample_t *)jack_port_get_buffer(client->mInputPorts[i], nframes);
        for (unsigned int i = 0; i < client->mOutputs; i++)
            client->mOut[i] = (jack_default_audio_sample_t *)jack_port_get_buffer(client->mOutputPorts[i], nframes);

        for (int i = 0; i < nframes; i++)
        {
            // IN
            for (int ch = 0; ch < client->mInputs; ch++)
            {
                jack_ringbuffer_write(client->_rbin, (char *)client->mIn[ch], sizeof(jack_default_audio_sample_t));
                client->mIn[ch]++;
                
            }

            // OUT
            for (int ch = 0; ch < client->mOutputs; ch++)
            {
                jack_ringbuffer_read(client->_rbout, (char *)client->mOut[ch], sizeof(jack_default_audio_sample_t));
                client->mOut[ch]++;
            }
        }
        
        return 0;
    }
    
    static void Shutdown(void *arg)
    {}

private:
 
    jack_client_t* mClient;
    int mInputs;
    int mOutputs;
    
    int mBufferFrames;
    int mSampleRate;
    
    int mOutBufferBytes;
    int mInBufferBytes;

    
    jack_ringbuffer_t* _rbin;
    jack_ringbuffer_t* _rbout;
    std::string mClientName;
    std::vector<jack_port_t *> mOutputPorts;
    std::vector<jack_port_t *> mInputPorts;
    std::vector<sample_t*> mOut; 
	std::vector<sample_t*> mIn; 
};
