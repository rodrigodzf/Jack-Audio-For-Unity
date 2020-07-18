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

    static public bool StartJackClient(uint bufferSize, uint inchannels, uint outchannels)
    {
        return CreateClient(bufferSize, inchannels, outchannels);
    }
    
    static public void DestroyJackClient()
    {
        Debug.Log("Disabling Jack");
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

    #region DllImport
    #if UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX
        [DllImport("JackAudioForUnity")]
        private static extern bool CreateClient(uint bufferSize, uint inchannels, uint outchannels);
        [DllImport("JackAudioForUnity")]
        private static extern bool DestroyClient();
        [DllImport("JackAudioForUnity")]
        private static extern void GetAllData(float[] buffer);
        [DllImport("JackAudioForUnity")]
        private static extern void SetAllData(float[] buffer);
    #else
        [DllImport("UnityJackAudio")]
        private static extern bool CreateClient(uint bufferSize, uint inchannels, uint outchannels);
        [DllImport("UnityJackAudio")]
        private static extern bool DestroyClient();
        [DllImport("UnityJackAudio")]
        private static extern void GetAllData(float[] buffer);
        [DllImport("UnityJackAudio")]
        private static extern void SetAllData(float[] buffer);
        [DllImport("UnityJackAudio", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RegisterLogCallback(LogCallback buffer);
    #endif
    #endregion

}

}
