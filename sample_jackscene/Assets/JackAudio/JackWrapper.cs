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

    static public bool StartJackClient(uint bufferSize,
                                       uint sampleRate,
                                       uint inchannels,
                                       uint outchannels)
    {
        CreateClient(bufferSize, sampleRate);
        RegisterPorts(inchannels, outchannels);
        return true;
    }
    
    static public void DestroyJackClient()
    {
        DestroyClient();
    }

    static public void SetMixedData(float[] buffer)
    {
        SetAllData(buffer);
    }

    static public void GetMixedData(float[] buffer)
    {
        GetAllData(buffer);
    }

    [DllImport("UnityJackAudio")]
    private static extern bool CreateClient(uint bufferSize, uint sampleRate);
    [DllImport("UnityJackAudio")]
    private static extern bool DestroyClient();
    [DllImport("UnityJackAudio")]
    private static extern bool RegisterPorts( uint inputs, uint outputs );
    [DllImport("UnityJackAudio")]
    private static extern void WriteBuffer( int channel, float[] buffer, int size );
    [DllImport("UnityJackAudio")]
    private static extern void ReadBuffer( int channel, float[] buffer, int size );
    [DllImport("UnityJackAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RegisterLogCallback(LogCallback buffer);
}

}
