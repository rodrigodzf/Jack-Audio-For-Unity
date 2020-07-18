using UnityEngine;
using UnityEditor;
using System;

namespace JackAudio
{

public class JackEditorWindow : EditorWindow
{
    bool groupEnabled;
    int inputs = 2;
    int outputs = 2;

    static int numbuffers;
    static int buffersize;
    static AudioConfiguration config;

    static bool bInitialized = false;
    // Add menu named "My Window" to the Window menu
    [MenuItem("Window/Jack")]
    static void Init()
    {

        AudioSettings.GetDSPBufferSize(out buffersize, out numbuffers);
        config = AudioSettings.GetConfiguration();

        // Get existing open window or if none, make a new one:
        JackEditorWindow window = (JackEditorWindow)EditorWindow.GetWindow(typeof(JackEditorWindow));
        window.Show();
    }

    void OnGUI()
    {
        if (!bInitialized) {
                GUI.backgroundColor = Color.red;
                GUILayout.BeginVertical(GUI.skin.box);
                // GUILayout.Label("DOTWEEN SETUP REQUIRED", EditorGUIUtils.setupLabelStyle);
                GUILayout.EndVertical();
                GUI.backgroundColor = Color.white;
        } else GUILayout.Space(8);
        if( GUILayout.Button("Connect to Jack Server"))
        {   
            Debug.Log("buffersize: " + buffersize);
            bInitialized = JackWrapper.StartJackClient((uint)buffersize, (uint)inputs, (uint)outputs);
        }

        GUILayout.Label("Base Settings", EditorStyles.boldLabel);
        inputs = EditorGUILayout.IntField("Inputs", inputs);
        outputs = EditorGUILayout.IntField("Outputs", outputs);

        groupEnabled = EditorGUILayout.BeginToggleGroup("Optional Settings", groupEnabled);
        // inputs = EditorGUILayout.IntField("Inputs", inputs);
        // outputs = EditorGUILayout.IntField("Outputs", outputs);
        EditorGUILayout.EndToggleGroup();
    }
}

}