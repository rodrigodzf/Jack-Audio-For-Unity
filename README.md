# Unity Jack

## Introduction

This library/plugin enables the multichannel audio routing between Unity3D and JackAudio. Simply import the compiled bundle/dll to your project.

## Installation

### Windows

Install Jack and follow the instructions in the [Jack Audio website](http://jackaudio.org/faq/jack_on_windows.html).
Make sure the JackRouter.dll is registered in your system.

### macOS and Linux

Install the appropiate package for your plattform from the [website](https://github.com/jackaudio/jack2). 

## Use

Import the JackAudio.unitypackage in your project and make sure Unity audio settings are the default ones. **The default audio block size is 'Best Performance' or 1024.**

There are two ways of using this plugin. As an audio filter or as an effect.

As an audio filter component, the JackSourceSend script must be attached to a GameObject with an AudioSource. 

As an 'effect', you can add the 'Jack Send' effect to the appropiate Mixer Group.

Additionally an instance of JackMultiplexer must be present in the Scene at all times.


