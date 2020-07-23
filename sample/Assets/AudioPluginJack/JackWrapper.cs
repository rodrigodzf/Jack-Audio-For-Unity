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

using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

namespace JackAudio
{
    public delegate void LogCallback(int level, IntPtr log);

public class JackWrapper {

    [DllImport("AudioPluginJack")]
    public static extern bool CreateClient(uint bufferSize,
                                           uint sampleRate);
    [DllImport("AudioPluginJack")]
    public static extern bool ActivateClient();
    [DllImport("AudioPluginJack")]
    public static extern bool DestroyClient();
    [DllImport("AudioPluginJack")]
    public static extern bool RegisterPorts( uint inputs, uint outputs );
    [DllImport("AudioPluginJack")]
    public static extern void WriteBuffer( int channel, float[] buffer, int size );
    [DllImport("AudioPluginJack")]
    public static extern void ReadBuffer( int channel, float[] buffer, int size );
    [DllImport("AudioPluginJack", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RegisterLogCallback(LogCallback buffer);
}

}
