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

public class JackSourceReceive : MonoBehaviour {

	public JackMultiplexer multiplexer;
	public int IN_PORT;
	// Use this for initialization

	// Update is called once per frame
	void Update () {
	
	}
	void OnAudioFilterRead (float[] data, int channels) {
		System.Array.Clear(data, 0, data.Length);
		if (multiplexer.isRunning()) {
			multiplexer.GetBuffer(IN_PORT, data);
		}
	}
}

}