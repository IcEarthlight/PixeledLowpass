/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CutFreqLookAndFeel : juce::LookAndFeel_V4
{
    void drawLinearSlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle sliderStyle,
                          juce::Slider& slider) override;
};

struct ResonanceLookAndFeel : juce::LookAndFeel_V4
{
    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

    int shakeCount = 0;
    juce::Point<float> shakeOffset { 0.f, 0.f };
};

struct CustomSlider : juce::Slider
{
public:
    CustomSlider(juce::RangedAudioParameter& rap, const juce::String& suffix)
        : juce::Slider(
              juce::Slider::SliderStyle::LinearHorizontal,
              juce::Slider::TextEntryBoxPosition::TextBoxAbove
          ),
          param(&rap),
          suffix(suffix)
    { }

    ~CustomSlider()
    {
        setLookAndFeel(nullptr);
    }

    inline float getValue() const noexcept
    {
        return param->getValue();
    }

    const int textHei = 14;
    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    juce::String getDisplayString() const;

private:
    juce::RangedAudioParameter* param;
    juce::String suffix;
};

struct CutFreqSlider : CustomSlider
{
public:
    //using CustomSlider::CustomSlider;
    CutFreqSlider(juce::RangedAudioParameter& rap, const juce::String& suffix)
        : CustomSlider(rap, suffix)
    {
        setLookAndFeel(&lnf);
    }

    ~CutFreqSlider()
    {
        setLookAndFeel(nullptr);
    }

private:
    CutFreqLookAndFeel lnf;
};

struct ResonanceSlider : CustomSlider
{
public:
    //using CustomSlider::CustomSlider;
    ResonanceSlider(juce::RangedAudioParameter& rap, const juce::String& suffix)
        : CustomSlider(rap, suffix)
    {
        setLookAndFeel(&lnf);
    }

    ~ResonanceSlider()
    {
        setLookAndFeel(nullptr);
    }

private:
    ResonanceLookAndFeel lnf;
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
    juce::Colour titleColor = juce::Colour::fromRGBA(255u, 255u, 255u, 191u);
    juce::Colour backColor = juce::Colour::fromRGB(44u, 44u, 44u);

    CutFreqSlider cutFreqSlider;
    ResonanceSlider resonanceSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment cutFreqAtch, resonanceAtch;

    std::unique_ptr<juce::Drawable> titleImg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PixeledLowpassAudioProcessorEditor)
};
