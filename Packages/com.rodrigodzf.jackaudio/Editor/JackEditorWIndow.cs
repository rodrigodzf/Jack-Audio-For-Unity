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

using UnityEngine;
using UnityEditor;
using System;

namespace JackAudio
{

    public class JackEditorWindow : EditorWindow
    {
        int inputs = 1;
        int outputs = 1;

        private RangeInt inputsRange = new RangeInt(0, 64);
        private RangeInt outputsRange = new RangeInt(0, 64);

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
            }
            else
            {
                GUI.backgroundColor = Color.green;
            }

            GUILayout.BeginVertical(GUI.skin.box);
            GUILayout.EndVertical();
            GUI.backgroundColor = Color.white;

            if (GUILayout.Button("Connect to Jack Server"))
            {
                int numbuffers;
                int bufferSize;
                AudioSettings.GetDSPBufferSize(out bufferSize, out numbuffers);
                AudioConfiguration config = AudioSettings.GetConfiguration();

                JackLogger.Initialize();
                if (JackWrapper.CreateClient((uint)bufferSize, (uint)config.sampleRate))
                {
                    JackWrapper.RegisterPorts((uint)inputs, (uint)outputs);
                    activated = JackWrapper.ActivateClient();
                }
            }

            if (GUILayout.Button("Disconnect from Jack Server"))
            {
                if (JackWrapper.DestroyClient())
                {
                    activated = false;
                }
            }

            GUILayout.Label("Base Settings", EditorStyles.boldLabel);
            inputs = Mathf.Clamp(EditorGUILayout.IntField("Inputs", inputs), inputsRange.start, inputsRange.end);
            outputs = Mathf.Clamp(EditorGUILayout.IntField("Outputs", outputs), outputsRange.start, outputsRange.end);

            // groupEnabled = EditorGUILayout.BeginToggleGroup("Optional Settings", groupEnabled);
            // inputs = EditorGUILayout.IntField("Inputs", inputs);
            // outputs = EditorGUILayout.IntField("Outputs", outputs);
            // EditorGUILayout.EndToggleGroup();
        }
    }
}
