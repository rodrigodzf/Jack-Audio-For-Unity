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

using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using AOT;

namespace JackAudio
{
  public class JackLogger
  {
    public delegate void LogCallback(int level, string log);

    static bool installed = false;

    [MonoPInvokeCallback(typeof(LogCallback))]
    public static void OnLogCallback(int level, string log)
    {
      switch (level)
      {
        case 2:
          {
            Debug.LogError($"[cpp] {log}");
            break;
          }
        case 1:
          {
            Debug.LogWarning($"[cpp] {log}");
            break;
          }
        default:
          Debug.Log($"[cpp] {log}");
          break;
      }
    }

    public static void Initialize()
    {
      if (!installed)
      {
        JackWrapper.RegisterLogCallback(OnLogCallback);
        installed = true;
      }
    }
  }

  public class JackWrapper
  {
    private const string PluginName = "AudioPluginJack";


    [DllImport(PluginName)]
    public static extern bool CreateClient(uint bufferSize, uint sampleRate);
    [DllImport(PluginName)]
    public static extern bool ActivateClient();
    [DllImport(PluginName)]
    public static extern bool DestroyClient();
    [DllImport(PluginName)]
    public static extern bool RegisterPorts(uint inputs, uint outputs);
    [DllImport(PluginName)]
    public static extern void WriteBuffer(int channel, float[] buffer, int size);
    [DllImport(PluginName)]
    public static extern void ReadBuffer(int channel, float[] buffer, int size);
    [DllImport(PluginName)]
    public static extern int GeBufferSize();
    [DllImport(PluginName, ExactSpelling = true, CharSet = CharSet.Ansi)]
    public static extern void RegisterLogCallback(JackLogger.LogCallback callback);
  }

}
