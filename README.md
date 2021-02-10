[![openupm](https://img.shields.io/npm/v/com.rodrigodzf.jackaudioforunity?label=openupm&registry_uri=https://package.openupm.com)](https://openupm.com/packages/com.rodrigodzf.jackaudioforunity/)

# Jack Audio For Unity

## Introduction

This library/plugin enables multichannel audio routing between Unity3D and JackAudio.

## Installation

The prefered method of installation is using [OpenUPM](https://openupm.com/packages/com.rodrigodzf.jackaudioforunity/)

### Windows

Install Jack v1.9.11 by following the instructions on the [Jack Audio website](http://jackaudio.org/faq/jack_on_windows.html).
Make sure the JackRouter.dll is registered in your system.  

Important: If you want to use a different Jack version, you have to compile the plugin from source by yourself.

### macOS

Install jackOSX Version 0.92_b3 binaries from [jackaudio.org/downloads](http://jackaudio.org/downloads/)

### Linux

Install the appropiate package for your plattform from the [website](https://github.com/jackaudio/jack2).

## Use

**Make sure Jack is Running first**.

If you use linux, it is highly recommended you use [Cadence](https://kx.studio/Applications:Cadence) to avoid misconfiguration of Jack.

Your Jack settings might look similar to these.

![Your Jack settings might look similar to these.](https://github.com/rodrigodzf/Jack-Audio-For-Unity/blob/master/settings.png)

Import the JackAudio.unitypackage in your project and make sure Unity audio settings are the default ones.

Some important things to consider while setting up your project:

- **The default audio block size is 'Best Performance' or 1024. This has to match your configured buffer size in Jack. It is recommended to set it to 1024**
- **Your sample rate should also match the sample rate configured in Jack**
- **Jack ports are mono ports, make sure your Unity project settings use "Mono" as the default "Speaker Mode"**

There are two ways of using this plugin. As an audio filter or as an effect.

As an audio filter component, the JackSend script must be attached to a GameObject with an AudioSource.

As an 'effect', you can add the 'Jack Send' effect to the appropiate Mixer Group.

To configure and connect Unity to the Jack server please use the Jack Editor Window under `Window -> Jack`

### Caveats

For Windows: 

- If the Unity window loses focus then the sound will stop. To prevent this you can check the Run in Background checkbox in player settings.

- You might need to recompile jack2 or disable the `Random memory allocations (Bottom-up ASLR)`, if Unity crashes on start. See this issue https://github.com/rodrigodzf/Jack-Audio-For-Unity/issues/15#issuecomment-527897023


## Compiling the plugin

In linux, you can use the included CMakeLists.txt to build the library:

```bash
cd UnityJack
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

In Windows you need to give a hint to where the jack libraries are found, for example:

```bash
cd UnityJack
mkdir build
cd build
cmake .. -A x64 -DJACK_LIB_DIR="C:\Program Files (x86)\Jack\lib" -DJACK_HEADER_DIR="C:\Program Files (x86)\Jack\includes" -DCMAKE_BUILD_TYPE=Release
```

*The default precompiled jack headers are a little old in windows and have to be altered in order to compile, refer to this [commit](https://github.com/jackaudio/jack2/commit/e26f98637ee4603b903599dc2e1862c2f373b864)*




