using UnityEngine;
using UnityEditor;
public class JackEditorWindow : EditorWindow
{
    string myString = "Hello World";
    bool groupEnabled;
    bool myBool = true;
    float myFloat = 1.23f;

    static bool bInitialized = false;
    // Add menu named "My Window" to the Window menu
    [MenuItem("Window/Jack")]
    static void Init()
    {
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
        if( GUILayout.Button("Connect to Jack Server")){
            // try to connect
            if (!bInitialized){
                // setup
            }

            // bInitialized = sucess
            // check state
        }
        Debug.Log(bInitialized);
        if (bInitialized){
            
        }
        GUILayout.Label("Base Settings", EditorStyles.boldLabel);
        myString = EditorGUILayout.TextField("Text Field", myString);

        groupEnabled = EditorGUILayout.BeginToggleGroup("Optional Settings", groupEnabled);
        myBool = EditorGUILayout.Toggle("Toggle", myBool);
        myFloat = EditorGUILayout.Slider("Slider", myFloat, -3, 3);
        EditorGUILayout.EndToggleGroup();
    }
}