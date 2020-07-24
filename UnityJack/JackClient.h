// Copyright (C) 2020  Rodrigo Diaz
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

#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>

#include "Jack.h"
#include "Logger.h"
#include "Stream.h"
namespace AudioPluginJack
{

class JackClient
{

public:
  static JackClient& getInstance()
  {
    static JackClient instance; // Guaranteed to be destroyed.
    return instance;
  }

  int write_buffer( int idx, float* buffer, int buffer_size )
  {
    if ( !activated ) { return -1; }
    if ( output_streams.count(idx) == 0 ) { return -1; }
    output_streams[idx]->write(buffer, buffer_size);
    return 0;
  }

  int read_buffer( int idx, float* buffer, int buffer_size )
  {
    if ( !activated ) { return -1; } 
    if ( input_streams.count(idx) == 0 ) { return -1; }
    input_streams[idx]->read(buffer, buffer_size);
    return 0;
  }

  int set_callbacks()
  {
    /* add registration callback */
    // if ( 0 != jack_set_client_registration_callback( client, JackClient::RegistrationCallback, this ) )
    // {
      // LOG("Could not register callback");
      // return -1;
    // }

    /* tell the JACK server to call `process()' whenever
    there is work to be done.
    */
    if ( 0 != jack_set_process_callback( client, JackClient::Process, this ) )
    {
      LOG("Could not set process callback");
      return -1;
    }

    /* tell the JACK server to call `jack_shutdown()' if
    it ever shuts down, either entirely, or if it
    just decides to stop calling us.
    */
    jack_on_shutdown( client, JackClient::Shutdown, this );

    return 0;
  }

  bool register_ports_streams( const int requested_inputs, const int requested_outputs )
  {
    // panic
    if ( activated ) { return false; }
    if ( !client ) { return false; }

    LOG("Registering input ports:  " << requested_inputs);
    LOG("Registering output ports: " << requested_outputs);

    for (unsigned int i = 0; i < requested_inputs; i++)
    {
      // create internal stream
      input_streams.insert({i, std::make_unique<Stream>()});

      // register corresponding port
      std::string portname("in" + std::to_string(i));
      jack_port_t* port = jack_port_register( client, portname.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );
      if (nullptr != port) { input_ports.push_back(port); }
    }

    for (unsigned int i = 0; i < requested_outputs; i++)
    {
      // create internal stream
      output_streams.insert({i, std::make_unique<Stream>()});

      // register corresponding port
      std::string portname =("out" + std::to_string(i));
      jack_port_t* port = jack_port_register( client, portname.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
      if (nullptr != port){ output_ports.push_back(port); }
    }

    inputs = requested_inputs;
    outputs = requested_outputs;
    return true;
  }

  bool unregister_ports_streams()
  {
    // panic
    // if ( !activated ) { return false; }

    // for(auto& port : input_ports)
    // {
    //   if(0 != jack_port_unregister(client, port))
    //   {
    //     LOG("could not unregister port");
    //     return false;
    //   }
    // }

    // for(auto& port : output_ports)
    // {
    //   if(0 != jack_port_unregister(client, port))
    //   {
    //     LOG("could not unregister port");
    //     return false;
    //   }
    // }

    inputs  = 0;
    outputs = 0;
    std::cout << "clear streams" << std::endl;
    input_streams.clear();
    output_streams.clear();

    std::cout << "clear ports" << std::endl;
    input_ports.clear();
    output_ports.clear();
    return true;
  }


  bool create_client( int buffer_size,
                      int sample_rate )
  {
    const std::lock_guard<std::mutex> lock(mutex);

    if ( !activated )
    {
      LOG("Creating Client with: ");
      LOG("BufferSize: " << buffer_size);
      LOG("SampleRate: " << sample_rate);

      jack_status_t status;
      client = jack_client_open( "Unity3D", JackNullOption, &status );
      if ( !client )
      {
        LOG("Could not create the client");
        return false;
      }
      
      // make sure we are dealing with the same buffer size and sample rate
      current_buffer_size = jack_get_buffer_size( client );
      if ( buffer_size != current_buffer_size )
      {
        LOG("Invalid buffersize");
        return false;
      }

      // set callbacks
      if ( 0 != set_callbacks() )
      {
        LOG("could not set callbacks");
        return false;
      }
    }
    return true;
  }

  bool activate()
  {
    if ( !client ) { return false; }

    if ( 0 != jack_activate(client) )
    {
      ERROR("Could not activate client");
      activated = false;
      return false;
    }

    LOG("Client created and activated");
    activated = true;
    return activated;
  }

  bool destroy_client()
  {
    const std::lock_guard<std::mutex> lock(mutex);

    if ( activated )
    {
      activated = false; // important: activated flag must be false before resetting the client.
      std::cout << "about to deactivate" << std::endl;

      if( 0 != jack_deactivate(client))
      {
        std::cout << "could not deactivate client" << std::endl;
        LOG("could not deactivate jack");
        return false;
      }

      std::cout << "deactivated" << std::endl;

      if( 0 != jack_client_close(client))
      {
        std::cout << "could not close client" << std::endl;
        LOG("could not close jack");
        return false;
      }

      std::cout << "closed" << std::endl;


      unregister_ports_streams();

      std::cout << "unregistered ports" << std::endl;

      //client = nullptr;
      current_buffer_size = 0;
      //LOG("Client deactivated and uninitialized");
      return true;
    }
    return false;
  }

  int get_buffer_size()
  {
    if ( activated )
    {
      return current_buffer_size;
    }
    return 0;
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
  static int Process( nframes_t buffer_size, void* arg )
  {
    if ( 0 == buffer_size ) { return 0; }
    JackClient* jack_client = ( JackClient* )arg;
    if ( nullptr == jack_client ){ return 0; }
    if ( false == jack_client->activated ) { return 0; }
    if ( jack_client->input_streams.size() != jack_client->inputs ) { return 0; }
    if ( jack_client->input_ports.size() != jack_client->inputs ) { return 0; }
    if ( jack_client->output_streams.size() != jack_client->outputs ) { return 0; }
    if ( jack_client->output_ports.size() != jack_client->outputs ) { return 0; }
    
    // get the input and output buffers
    for ( unsigned int i = 0; i < jack_client->inputs; i++ )
    {
      sample_t *in = static_cast<sample_t*>(jack_port_get_buffer( jack_client->input_ports[i], buffer_size ));
      jack_client->input_streams[i]->write(in, buffer_size);
    }

    for ( unsigned int i = 0; i < jack_client->outputs; i++ )
    {
      sample_t *out = static_cast<sample_t*>(jack_port_get_buffer( jack_client->output_ports[i], buffer_size ));
      jack_client->output_streams[i]->read(out, buffer_size);
    }

    return 0;
  }

  static void Shutdown( void* ) {}

  static void RegistrationCallback( const char* name, int, void* )
  {
    LOG("Registered with name:" << name);
  }

  void register_log_callback( LogCallback callback )
  { 
    if ( logger )
    {
      logger->register_log_callback(callback); 
    }
  }

private:
  jack_client_t* client = nullptr;
  std::map<unsigned int, std::unique_ptr<Stream>> input_streams;
  std::map<unsigned int, std::unique_ptr<Stream>> output_streams;
  std::vector<jack_port_t*> output_ports;
  std::vector<jack_port_t*> input_ports;

  unsigned int inputs     = 0;
  unsigned int outputs    = 0;
  nframes_t current_buffer_size = 0;
  std::unique_ptr<Logger> logger;

  std::mutex mutex;
  volatile bool activated = false;
};

} // namespace UnityJackAudio
