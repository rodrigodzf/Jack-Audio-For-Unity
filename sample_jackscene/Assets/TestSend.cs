using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestSend : MonoBehaviour {

	// const int MAX_SOURCES = 50;
	public AudioClip clip;
	public JackAudio.JackMultiplexer mainMultiplexer;
	// Use this for initialization
	IEnumerator Start () {
		
		yield return null;
		for (int i = 0; i < mainMultiplexer.OUTPUTS; i++)
		{
			GameObject source = new GameObject("Source" + i);	
			source.AddComponent<AudioSource>().clip = clip;
			JackAudio.JackSourceSend send = source.AddComponent<JackAudio.JackSourceSend>();
			send.trackNumber = i;
			send.multiplexer = mainMultiplexer;
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
