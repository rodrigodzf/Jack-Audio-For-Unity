using UnityEditor;
using UnityEngine;
using System.Runtime.InteropServices;
using JackAudio;
using System;

public class JackSendEffectGUI : IAudioEffectPluginGUI
{
    public override string Name
    {
        get { return "Jack Send"; }
    }

    public override string Description
    {
        get { return "Send audio to jack port"; }
    }

    public override string Vendor
    {
        get { return "rdzf"; }
    }

    private string portName = "Port"; // this needs to match the definition in cpp
    private int portInt = 0;

    public void DrawControl(IAudioEffectPlugin plugin, Rect r, float samplerate)
    {
        Event evt = Event.current;
        int controlID = GUIUtility.GetControlID(FocusType.Passive);
        EventType evtType = evt.GetTypeForControl(controlID);

        r = AudioCurveRendering.BeginCurveFrame(r);


        if (Event.current.type == EventType.Repaint)
        {
            float blend = plugin.IsPluginEditableAndEnabled() ? 1.0f : 0.5f;

            int numsamples = (int)r.width;

            float port; plugin.GetFloatParameter(portName, out port);


            string name = "Port: " + port;
            GUIHelpers.DrawText(r.x + 5, r.y + 5, r.width, name, Color.white);

        }
        AudioCurveRendering.EndCurveFrame();
    }

    public override bool OnGUI(IAudioEffectPlugin plugin)
    {
        // port int field
        plugin.GetFloatParameter(portName, out float port);
        plugin.GetFloatParameterInfo(portName, out float minVal, out float maxVal, out float defVal);

        portInt = (int)port;
        portInt = Mathf.Clamp(EditorGUILayout.IntField("Port number:", portInt), (int)minVal, (int)maxVal);
        plugin.SetFloatParameter(portName, portInt);

        // box
        // GUILayout.Space(5f);
        // Rect r = GUILayoutUtility.GetRect(200, 100, GUILayout.ExpandWidth(true));
        // DrawControl(plugin, r, plugin.GetSampleRate());
        // GUILayout.Space(5f);

        // show/hide default sliders
        return false;
    }
}

   