/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawLinearSlider(juce::Graphics&,
                          int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle, juce::Slider&) override { }
};

struct MySlider : juce::Slider
{
    MySlider(juce::RangedAudioParameter& rap, const juce::String& suffix)
        : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal,
          juce::Slider::TextEntryBoxPosition::TextBoxAbove),
          param(&rap),
          suffix(suffix)
    {
        setLookAndFeel(&lnf);
    }

    ~MySlider()
    {
        setLookAndFeel(nullptr);
    }

    const int textHei = 14;
    void paint(juce::Graphics& g) override { }
    juce::Rectangle<int> getSliderBounds() const;
    juce::String getDisplayString() const;

private:
    LookAndFeel lnf;
    juce::RangedAudioParameter* param;
    juce::String suffix;
};

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

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PixeledLowpassAudioProcessor& audioProcessor;

    const int defaultWid = 476, defaultHei = 160;
    MySlider cutFreqSlider, resonanceSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment cutFreqAtch, resonanceAtch;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PixeledLowpassAudioProcessorEditor)
};
