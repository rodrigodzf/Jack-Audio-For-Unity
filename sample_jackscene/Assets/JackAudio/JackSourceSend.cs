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

namespace JackAudio
{

[RequireComponent (typeof (AudioSource))]
public class JackSourceSend : MonoBehaviour {

    public JackMultiplexer multiplexer;
    public int trackNumber;
	private const int BUF_SIZE = 512;
    private float[] monodata;

    public bool IsMuted = false;

    void Awake() {
        monodata = new float[BUF_SIZE]; //this has to be set from options
        // Check if multiplexer is there, and check if id is unique //
        if (multiplexer == null)
        {
            Debug.LogError("Cannot start without multiplexer, trying to find a multiplexer component in the scene");
            multiplexer = FindObjectOfType<JackMultiplexer>();
        }
    }
    
    // Dont add to the audio filter callback for control, use the AudioSource instead. 
    void OnAudioFilterRead(float[] buffer, int channels)
    {
        if (multiplexer.isRunning())
        {
            /* force to mono*/
            if (channels == 2)
            {
                for (int i = 0,j=0; i < BUF_SIZE; i++,j+=2)
                {
                    monodata[i] = buffer[j] + buffer[j+1];
                }
            }
            System.Array.Copy(monodata, multiplexer.combinedBuffers[trackNumber], BUF_SIZE);
        }
        // We need to zero the buffer after copying it,
        // otherwise it will play in unity as well
        System.Array.Clear(buffer, 0, buffer.Length);
    }
}

}