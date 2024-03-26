/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomSliders.h"
#include "CustomCheckbox.h"
#include "SVGRenderer.h"

//==============================================================================
/**
*/
class PixeledLowpassAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PixeledLowpassAudioProcessorEditor (PixeledLowpassAudioProcessor&);
    ~PixeledLowpassAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void deltaSwitch();

    CutFreqSlider cutFreqSlider;
    ResonanceSlider resonanceSlider;
    CustomToggleButton deltaBox;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PixeledLowpassAudioProcessor& audioProcessor;

    const int defaultWid = 476, defaultHei = 160;

    juce::AudioProcessorValueTreeState::SliderAttachment cutFreqAtch, resonanceAtch;
    juce::AudioProcessorValueTreeState::ButtonAttachment deltaAtch;

    bool lastState = false;
    SVGRenderer renderer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PixeledLowpassAudioProcessorEditor)
};
