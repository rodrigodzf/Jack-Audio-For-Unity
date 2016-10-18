using UnityEngine;
using System.Collections;

namespace JackAudio
{

 public class TestJack : MonoBehaviour {

    void Awake() {
        JackWrapper.StartJackClient(2,2);
    }

    void OnDestroy() {

        JackWrapper.DestroyJackClient();

    }
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnAudioFilterRead(float[] buffer, int channels) {

        // JackWrapper.SetAudioBuffer(ref buffer[0]);
        System.Array.Clear(buffer, 0, buffer.Length);

    }
}

}

