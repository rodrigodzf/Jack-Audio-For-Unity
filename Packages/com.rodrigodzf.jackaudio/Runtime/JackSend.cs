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

using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace JackAudio
{

    [RequireComponent(typeof(AudioSource))]
    public class JackSend : MonoBehaviour
    {
        public int port = 0;
        private bool started = false;
        private float[] monodata;
        GCHandle monodataHandler;
        private int bufferSize = 0;

        void Start()
        {
            bufferSize = JackWrapper.GeBufferSize();
            if (bufferSize == 0)
            {
                Debug.LogError("Jack is not running");
                return;
            }
            monodata = new float[bufferSize];

            // The buffer passed to jack must be pinned to memory otherwise it is possible that the GC
            // will free the buffer while jack is still using it
            monodataHandler = GCHandle.Alloc(monodata, GCHandleType.Pinned);
            System.Array.Clear(monodata, 0, monodata.Length);
            started = true;
        }

        void OnDestroy()
        {
            if (!started) { return; }
            System.Array.Clear(monodata, 0, bufferSize);
            JackWrapper.WriteBuffer(port, monodata, bufferSize);
            monodataHandler.Free();
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
