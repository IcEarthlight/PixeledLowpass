/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

inline float differenceBetween(juce::Colour& c0, juce::Colour& c1);
inline juce::Colour colorLerp(juce::Colour& c0, juce::Colour& c1, const float k);
inline juce::Colour colorApproach(juce::Colour& c0, juce::Colour& c1);

class CutFreqSlider;
class ResonanceSlider;

struct CutFreqLookAndFeel : juce::LookAndFeel_V4
{
public:
    CutFreqLookAndFeel()
        : juce::LookAndFeel_V4()
    { }

    void drawLinearSlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle sliderStyle,
                          juce::Slider& slider) override;

    bool needRepaint = true;

private:
    juce::Colour lastColor { 44u, 44u, 44u };
};

struct ResonanceLookAndFeel : juce::LookAndFeel_V4
{
public:
    ResonanceLookAndFeel(CutFreqSlider& cutFreqSlider)
        : juce::LookAndFeel_V4(), cutFreqSlider(cutFreqSlider)
    { }

    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

    bool needRepaint = true;

private:
    int shakeCount = 0;
    juce::Point<float> shakeOffset { 0.f, 0.f };
    CutFreqSlider& cutFreqSlider;

    juce::Colour lastColor{ 44u, 44u, 44u };
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

    inline bool needRepaint() const noexcept
    {
        return lnf.needRepaint;
    }

private:
    CutFreqLookAndFeel lnf;
};

struct ResonanceSlider : CustomSlider
{
public:
    //using CustomSlider::CustomSlider;
    ResonanceSlider(juce::RangedAudioParameter& rap, const juce::String& suffix, CutFreqSlider& cutFreqSlider)
        : CustomSlider(rap, suffix), cutFreqSlider(cutFreqSlider), lnf(cutFreqSlider)
    {
        setLookAndFeel(&lnf);
    }

    ~ResonanceSlider()
    {
        setLookAndFeel(nullptr);
    }

    inline bool needRepaint() const noexcept
    {
        return lnf.needRepaint || getValue() > 0.4f;
    }

private:
    ResonanceLookAndFeel lnf;
    CutFreqSlider& cutFreqSlider;
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

    CutFreqSlider cutFreqSlider;
    ResonanceSlider resonanceSlider;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PixeledLowpassAudioProcessor& audioProcessor;

    const int defaultWid = 476, defaultHei = 160;
    const juce::Colour titleColor = juce::Colour::fromRGBA(255u, 255u, 255u, 191u);
    const juce::Colour backColor = juce::Colour::fromRGB(44u, 44u, 44u);
    juce::AudioProcessorValueTreeState::SliderAttachment cutFreqAtch, resonanceAtch;

    std::unique_ptr<juce::Drawable> titleImg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PixeledLowpassAudioProcessorEditor)
};
