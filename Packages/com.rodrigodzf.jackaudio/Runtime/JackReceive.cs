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
using System.Runtime.InteropServices;

namespace JackAudio
{

    [RequireComponent(typeof(AudioSource))]
    public class JackReceive : MonoBehaviour
    {

        public int port;
        private bool started = false;
        private int sampleRate = 44100;
        private int bufferSize = 0;

        private float[] monodata;
        GCHandle monodataHandler;

        // This is a hack, and should probaly not be used
        // but this is the only reliable way to get audio into unity
        // before the dsp chain. Using a callback  on the clip 
        // does not consistenty call a fixed block size
        void generateDummyClip()
        {
            AudioClip clip = AudioClip.Create("clip", bufferSize, 1, sampleRate, false);
            AudioSource source = GetComponent<AudioSource>();

            float[] samples = new float[bufferSize];
            System.Array.Clear(samples, 0, samples.Length);
            clip.SetData(samples, 0);

            source.clip = clip;
            source.loop = true;
            source.Play();
        }
        void Start()
        {
            bufferSize = JackWrapper.GeBufferSize();
            if (bufferSize == 0)
            {
                Debug.LogError("Jack is not running");
                return;
            }

            monodata = new float[bufferSize]; //this has to be set from options
            monodataHandler = GCHandle.Alloc(monodata, GCHandleType.Pinned);
            generateDummyClip();
            started = true;
        }

        void OnDestroy()
        {
            if (!started) { return; }
            System.Array.Clear(monodata, 0, monodata.Length);
            monodataHandler.Free();
        }

        void OnAudioRead(float[] buffer)
        {
            if (!started) { return; }
            Debug.Log(buffer.Length);
            // System.Array.Clear(buffer, 0, buffer.Length);

            // to find the number of samples per channel, divide data.Length by channels
            // JackWrapper.ReadBuffer(port, buffer, buffer.Length);
        }

        void OnAudioFilterRead(float[] buffer, int channels)
        {
            if (!started) { return; }
            if (buffer.Length != bufferSize) { Debug.LogError("buffer size does not match jack"); return; }
            if (channels != 1) { Debug.LogError("jack can only accept mono"); return; }

            JackWrapper.ReadBuffer(port, buffer, bufferSize);
            // System.Array.Copy(monodata,buffer,bufferSize);

        }
    }
}