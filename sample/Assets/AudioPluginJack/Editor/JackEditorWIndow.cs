using UnityEngine;
using UnityEditor;
using System;

namespace JackAudio
{

  public class JackEditorWindow : EditorWindow
  {
    int inputs = 1;
    int outputs = 1;
    bool activated = false;

    [MenuItem("Window/Jack")]
    static void Init()
    {
      // Get existing open window or if none, make a new one:
      JackEditorWindow window = (JackEditorWindow)EditorWindow.GetWindow(typeof(JackEditorWindow));
      window.Show();
    }

    void OnGUI()
    {
      if (!activated)
      {
        GUI.backgroundColor = Color.red;
        GUILayout.BeginVertical(GUI.skin.box);
        GUILayout.EndVertical();
        GUI.backgroundColor = Color.white;
      }
      else
      { 
        GUILayout.Space(8);
      }

      if (GUILayout.Button("Connect to Jack Server"))
      {
        int numbuffers;
        int bufferSize;
        AudioSettings.GetDSPBufferSize(out bufferSize, out numbuffers);
        AudioConfiguration config = AudioSettings.GetConfiguration();

        JackLogger.Initialize();
        JackWrapper.CreateClient((uint)bufferSize,
                                 (uint)config.sampleRate);
        JackWrapper.RegisterPorts((uint)inputs, (uint)outputs);
        activated = JackWrapper.ActivateClient();
      }

      if (GUILayout.Button("Disconnect from Jack Server"))
      {
        JackWrapper.DestroyClient();
      }

      GUILayout.Label("Base Settings", EditorStyles.boldLabel);
      inputs = EditorGUILayout.IntField("Inputs", inputs);
      outputs = EditorGUILayout.IntField("Outputs", outputs);

      // groupEnabled = EditorGUILayout.BeginToggleGroup("Optional Settings", groupEnabled);
      // inputs = EditorGUILayout.IntField("Inputs", inputs);
      // outputs = EditorGUILayout.IntField("Outputs", outputs);
      // EditorGUILayout.EndToggleGroup();
    }
  }
}
