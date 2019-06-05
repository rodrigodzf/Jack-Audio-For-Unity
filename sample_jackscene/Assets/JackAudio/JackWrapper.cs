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

public class JackWrapper {

    static public void StartJackClient(int inchannels, int outchannels)
    {
        Debug.Log("Starting Jack");
        if (!CreateClient(inchannels, outchannels)) {
            Debug.LogError("Jack Server not online");
        }

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
	private static extern bool CreateClient(int inchannels, int outchannels);
    [DllImport("JackAudioForUnity")]
    private static extern bool DestroyClient();
    [DllImport("JackAudioForUnity")]
	private static extern void GetAllData(float[] buffer);
    [DllImport("JackAudioForUnity")]
	private static extern void SetAllData(float[] buffer);
    #else
    [DllImport("AudioPlugin-JackAudioForUnity")]
	private static extern bool CreateClient(int inchannels, int outchannels);
    [DllImport("AudioPlugin-JackAudioForUnity")]
    private static extern bool DestroyClient();
    [DllImport("AudioPlugin-JackAudioForUnity")]
	private static extern void GetAllData(float[] buffer);
    [DllImport("AudioPlugin-JackAudioForUnity")]
	private static extern void SetAllData(float[] buffer);
    #endif
    // [DllImport("UnityJackAudio")]
    // public static extern void SetDebugFunction(IntPtr fp);
    // [DllImport("UnityJackAudio")]
    // private static extern int startNativeAudio(int inchannels, int outchannels);
    // [DllImport("UnityJackAudio")]
    // private static extern void stopNativeAudio();
    // [DllImport("UnityJackAudio")]
    // private static extern int getAudioBuffer(ref float buffer);
    // [DllImport("UnityJackAudio")]
    // private static extern int setAudioBuffer(ref float buffer);
    // [DllImport("UnityJackAudio")]
    // private static extern string getAudioDeviceName(uint deviceId);
    // [DllImport("UnityJackAudio")]
    // private static extern uint getAudioDeviceCount();
    // [DllImport("UnityJackAudio")]
    // private static extern uint getDefaultAudioDevice();
    // [DllImport("UnityJackAudio")]
    // private static extern uint getAudioInputChannels(uint deviceId);
    // [DllImport("UnityJackAudio")]
    // private static extern uint getSampleRate();
    // [DllImport("UnityJackAudio")]
    // private static extern uint getBufferFrames();
    #endregion

}

}
