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

#include <string>
#include <sstream>

// Logger callback
extern "C" typedef void ( *LogCallback )( int, const char* );

namespace AudioPluginJack
{

static LogCallback mLogCallback = nullptr;

class Logger
{
public:
  enum eLogType
  {
    JACK_LOG     = 0,
    JACK_WARNING = 1,
    JACK_ERROR   = 2
  };

  void register_log_callback( LogCallback callback ) { mLogCallback = callback; }

public:
  static void log( const eLogType level, const std::string &msg )
  {
    if ( nullptr != mLogCallback )
    {
      mLogCallback( static_cast<int>( level ), msg.c_str() );
    }
  }
};

#define LOG( msg )                          \
{                                           \
  std::stringstream ss;                     \
  ss << msg;                                \
  Logger::log( Logger::JACK_LOG, ss.str() );     \
}

#define WARNING( msg )                      \
{                                           \
  std::stringstream ss;                     \
  ss << msg;                                \
  Logger::log( Logger::JACK_WARNING, ss.str() ); \
}

#define ERR( msg )                        \
{                                           \
  std::stringstream ss;                     \
  ss << msg;                                \
  Logger::log( Logger::JACK_ERROR, ss.str() );   \
}

} // namespace UnityJackAudio
