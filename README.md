# Jack Audio For Unity

## Introduction

This library/plugin enables multichannel audio routing between Unity3D and JackAudio. Simply import the compiled bundle/dll to your project.

## Installation

### Windows

Install Jack v1.9.11 by following the instructions on the [Jack Audio website](http://jackaudio.org/faq/jack_on_windows.html). 
Make sure the JackRouter.dll is registered in your system.  

Important: If you want to use a different Jack version, you have to compile the plugin from source by yourself.

### macOS and Linux

Install the appropiate package for your plattform from the [website](https://github.com/jackaudio/jack2). 

## Use

**Make sure Jack is Running first**.

Your Jack settings might look similar to these.

![Your Jack settings might look similar to these.](https://github.com/rodrigodzf/Jack-Audio-For-Unity/blob/master/settings.png) 

Import the JackAudio.unitypackage in your project and make sure Unity audio settings are the default ones. **The default audio block size is 'Best Performance' or 1024.**

There are two ways of using this plugin. As an audio filter or as an effect.

As an audio filter component, the JackSourceSend script must be attached to a GameObject with an AudioSource. 

As an 'effect', you can add the 'Jack Send' effect to the appropiate Mixer Group.

Additionally an instance of JackMultiplexer must be present in the Scene at all times.

### Caveats

If the Unity window loses focus then the sound will stop. To prevent this you can check the *Run in Background* checkbox in player settings.


