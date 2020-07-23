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

#include "Jack.h"

#include <memory>

namespace AudioPluginJack
{

class Stream
{

public:
  Stream( const size_t size = 1024 )
  { 
    ring_buffer = jack_ringbuffer_create(sizeof(sample_t) * size);
  }

  ~Stream()
  {
    jack_ringbuffer_free(ring_buffer);
  }

public:

  size_t write( sample_t* buffer, const size_t size )
  {
    if ( jack_ringbuffer_write_space(ring_buffer) > 0 )
    {
      return jack_ringbuffer_write(ring_buffer, (char*)buffer, size * sizeof(sample_t) );
    }
    return 0;
  }

  size_t read( sample_t* buffer, const size_t size )
  {
    if( jack_ringbuffer_read_space(ring_buffer) > 0 )
    {
      return jack_ringbuffer_read(ring_buffer, (char*)buffer, size * sizeof(sample_t) );
    }
    return 0;
  }

private:
  jack_ringbuffer_t* ring_buffer = nullptr;
};

} //! namespace UnityJackAudio