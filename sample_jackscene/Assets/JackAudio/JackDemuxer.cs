// using UnityEngine;
// using System.Collections;
// using System.Runtime.InteropServices;

// public class JackDemuxer : MonoBehaviour {

// 	private float[] mixedBuffer;
// 	private int BUFSIZE;
// 	// mixedBuffer contains 512 samples per channel
// 	bool init = false;
// 	public int INPUTS;
// 	public int OUTPUTS;
// 	// Use this for initialization
// 	void Awake (){
// 		CreateClient(INPUTS, OUTPUTS);
// 		int n;
// 		AudioSettings.GetDSPBufferSize(out BUFSIZE, out n);
// 		mixedBuffer = new float[INPUTS * 512];
// 		init = true;
		
// 	}

// 	void Start () {
	
// 	}
	
// 	// Update is called once per frame
// 	void Update () {
	
// 	}

// 	void OnDestroy (){
// 		DestroyClient();
// 	}

// 	public void GetBuffer(int idx, float[] data)
// 	{
// 		for (int i = 0; i < BUFSIZE; i++) {
//             data[i] = mixedBuffer[(i * INPUTS) + idx];
//         }
// 	}
// 	void OnAudioFilterRead (float[] data, int channels) {
// 		// Debug.Log(data.Length);

// 		// System.Array.Clear(data, 0, data.Length);
// 		// System.Array.Clear(mixedBuffer, 0, mixedBuffer.Length);

//         if (init)
//         {
//             GetAllData(mixedBuffer);
//             // for (int i = 0, j = 0; j < data.Length; i++, j += 2)
//             // {
//             //     data[j] = mixedBuffer[j];
//             //     data[j + 1] = data[j];
//             // }
//             // System.Array.Copy(mixedBuffer, data, data.Length);
//         }
// 	}

// }
