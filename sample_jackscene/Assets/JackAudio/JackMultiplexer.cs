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
using UnityEngine.Assertions;
using System.Collections;
using System.Linq;


namespace JackAudio
{

public class JackMultiplexer : MonoBehaviour {

    // Default unity buffer size per mono block
	private int BUFFER_SIZE;
    public bool useEffects;

    [HideInInspector]
    public  float[][] combinedBuffers;
    
    private float[] mixedBufferOut;
    private float[] mixedBufferIn;
    
    private bool started = false;

	public int INPUTS;
	public int OUTPUTS;
    // 
    private JackSourceSend[] outSources;
    private JackSourceReceive[] inSources;

	int GetBufferSize(){
		int bufsize;
		int numbuffers;
		AudioSettings.GetDSPBufferSize(out bufsize, out numbuffers);
		Debug.Log("Unity bufsize " + bufsize + " numbufs " + numbuffers);
		return bufsize;
	}
    void Awake() {
		BUFFER_SIZE = GetBufferSize();
    }

    void OnDestroy() { 
        // if (!useEffects) JackWrapper.DestroyJackClient(); 
        JackWrapper.DestroyJackClient(); 
    }


    void Start() {


        // Check if total OUTPUTS correspond to existing OUTPUTS
        // outSources = FindObjectsOfType<JackSourceSend>();
        // Debug.Assert(outSources.Length == OUTPUTS, "Tracks (" + outSources.Length + ") dont correspond to total multiplexed OUTPUTS, adjusting total OUTPUTS.");
        // OUTPUTS = outSources.Length;

        // TODO: Check if assigned track number is unique
       

        // TODO: Check if source number exceeds OUTPUTS
        /*
        var faultSources = outSources.Where(g => g.trackNumber > OUTPUTS);
        foreach (var src in faultSources) {
            src.enabled = false;
        }
        */
        
        /* Allocate memory for streams */
        combinedBuffers = new float[OUTPUTS][];
        for (int i = 0; i < OUTPUTS; i++)
        {
            combinedBuffers[i] = new float[BUFFER_SIZE];
        }

        mixedBufferOut = new float[OUTPUTS * BUFFER_SIZE];
        mixedBufferIn = new float[INPUTS * BUFFER_SIZE];

        // Start Engine
        JackWrapper.StartJackClient(INPUTS, OUTPUTS);
        started = true;
    }

	public void GetBuffer(int idx, float[] data)
	{
		for (int i = 0, j=0; i < BUFFER_SIZE; i++, j+=2) {
            data[j] = mixedBufferIn[(i * INPUTS) + idx];
            data[j+1] = data[j];
        }
	}

    public void SetBuffer(int idx, float[] data) 
	{
	}



    public bool isRunning() { return started; }

    /// <summary>
    /// Main Audio mixer
    /// </summary>
    /// <param name="buffer"></param>
    /// <param name="channels"></param>
    void OnAudioFilterRead(float[] buffer, int channels)
    {

        if (!started || useEffects) return; 

        // We need to convert the jagged array to a one dimesional array
        // float[] mixedBufferIn = combinedBuffers.SelectMany(x => x).ToArray();
        // JackWrapper.SetAudioBuffer(ref combinedBuffers[0][0]);
        // float[] debugbuffer = combinedBuffers[0];

        // TODO: It is possible to send planar data instead.
        // That would spare iterating over the samples

        for (int j = 0; j < OUTPUTS; j++) {
            for (int i = 0; i < BUFFER_SIZE; i++) {
                mixedBufferOut[(i * OUTPUTS) + j] = combinedBuffers[j][i];
            }
        }
 
        JackWrapper.SetMixedData(mixedBufferOut);
        //JackWrapper.GetMixedData(mixedBufferIn);

        // System.Array.Clear(buffer, 0, buffer.Length);
    }
}


}