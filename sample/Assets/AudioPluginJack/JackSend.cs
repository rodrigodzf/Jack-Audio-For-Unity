using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace JackAudio
{

  public class JackSend : MonoBehaviour
  {
    public int port = 0;
    private bool started = false;
    private float[] monodata;
    private int bufferSize = 1024;


    void Start()
    {
      monodata = new float[1024]; //this has to be set from options
      System.Array.Clear(monodata, 0, monodata.Length);
      started = true;
    }

    void OnDestroy()
    {
      System.Array.Clear(monodata, 0, 1024);
      JackWrapper.WriteBuffer(port, monodata, 1024);
    }

    void OnAudioFilterRead(float[] buffer, int channels)
    {
      if (!started) { return; }
      if (buffer.Length != bufferSize) { Debug.LogError("buffer size does not match jack"); return; }
      if (channels != 1) { Debug.LogError("jack can only accept mono"); return; }

      // It is neccesary to make a copy here because the data is managed and will be deleted
      // after the call is finished or before
      System.Array.Copy(buffer, monodata, buffer.Length);
      JackWrapper.WriteBuffer(port, monodata, monodata.Length);
      System.Array.Clear(buffer, 0, buffer.Length);
    }
  }
}
