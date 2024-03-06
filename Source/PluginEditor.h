/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomComponents.h"

//==============================================================================
/**
*/
class PixeledLowpassAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PixeledLowpassAudioProcessorEditor (PixeledLowpassAudioProcessor&);
    ~PixeledLowpassAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    CutFreqSlider cutFreqSlider;
    ResonanceSlider resonanceSlider;
    CustomToggleButton deltaBox;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PixeledLowpassAudioProcessor& audioProcessor;

    const int defaultWid = 476, defaultHei = 160;
    const juce::Colour titleColor = juce::Colour::fromRGBA(255u, 255u, 255u, 191u);
    const juce::Colour backColor = juce::Colour::fromRGB(44u, 44u, 44u);

    juce::AudioProcessorValueTreeState::SliderAttachment cutFreqAtch, resonanceAtch;
    juce::AudioProcessorValueTreeState::ButtonAttachment deltaAtch;

    std::unique_ptr<juce::Drawable> titleImg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PixeledLowpassAudioProcessorEditor)
};
