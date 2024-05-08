# PixeledLowpass

Creative open-source lowpass/highpass filter built with [JUCE](https://juce.com/). The filtering algorithm with unique spectrum curve can add flavor to your fills and build-ups. Also the post level would never be higher than the input (then the Resonance is 0 dB), so it's safe to put it after a heavy distortion or waveshaping effect.

All parameters can be automated smoothly, and the GUI is resizeable and compatible with any DPI.

<div align="center">
    <img width="357" alt="PixeledLowpass Lowpass Mode" src="https://raw.githubusercontent.com/IcEarthlight/PixeledLowpass/master/screenshot_lowpass.png">
    <img width="357" alt="PixeledLowpass Highpass Mode" src="https://raw.githubusercontent.com/IcEarthlight/PixeledLowpass/master/screenshot_hipass.png">

</div>

## Parameters

- **Cut Freq**: the cut frequency of the filter
  - The displayed value is just for reference, as the real cutoff frequency is lower than the displayed one (approximately 0.4x)
  - When this parameter is at its maximum value, the plugin is totally bypassed and won't do anything to the audio. (when delta is on, it would be absolute silence)
- **Resonance**: add a peak resonance to the cutoff point
- **Delta**: switch between lowpass filter and highpass filter
- **x2**: double the effect of lowpass/highpass filter

## Installation

1. Download the .vst3 file from [release](https://github.com/IcEarthlight/PixeledLowpass/releases).
2. Copy the file to the vst3 directory in your system. (Windows: `C:\Program Files\Common Files\VST3`, MAC: `/Library/Audio/Plug-Ins/VST3`)
3. Rescan for new plugins in your respective DAW.