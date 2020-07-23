using System.Collections;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using UnityEngine;
using AOT;
using System;

namespace JackAudio
{

public class JackSend : MonoBehaviour
{
    private float[] monodata;

    [MonoPInvokeCallback(typeof(LogCallback))]
    static void OnLogCallback(int level, IntPtr log)
    {
        string debug_string = Marshal.PtrToStringAuto(log);
        UnityEngine.Debug.Log(debug_string);
    }

    void OnEnable()
    {
        JackWrapper.RegisterLogCallback(OnLogCallback);
    }
    
    // Start is called before the first frame update
    void Start()
    {
        monodata = new float[1024]; //this has to be set from options
        StartJack();
    }

    void OnDestroy()
    {
        // JackWrapper.DestroyClient();
    }

    void OnAudioFilterRead(float[] buffer, int channels)
    {
        // Debug.Log(buffer.Length);
        // to find the number of samples per channel, divide data.Length by channels
        int bufferSize = buffer.Length / channels;
        if (channels == 2)
        {
            for (int i = 0,j=0; i < bufferSize; i++,j+=2)
            {
                monodata[i] = buffer[j];
            }
        }

        JackWrapper.WriteBuffer(0, monodata, bufferSize);
        System.Array.Clear(buffer, 0, buffer.Length);
    }

}

}
