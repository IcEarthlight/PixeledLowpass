/*
  ==============================================================================

    CustomComponents.h
    Created: 6 Mar 2024 9:45:45pm
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
inline float differenceBetween(juce::Colour& c0, juce::Colour& c1);
inline juce::Colour colorLerp(juce::Colour& c0, juce::Colour& c1, const float k);
inline juce::Colour colorApproach(juce::Colour& c0, juce::Colour& c1);


//==============================================================================
class CutFreqSlider;
class ResonanceSlider;

struct CutFreqLookAndFeel : juce::LookAndFeel_V4
{
public:
    CutFreqLookAndFeel(const juce::AudioProcessorValueTreeState& apvts)
        : juce::LookAndFeel_V4()
    { }

    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

    bool needRepaint = true;

private:
    juce::Colour lastColor{ 44u, 44u, 44u };
};

struct ResonanceLookAndFeel : juce::LookAndFeel_V4
{
public:
    ResonanceLookAndFeel(const juce::AudioProcessorValueTreeState& apvts)
        : juce::LookAndFeel_V4(), cutFreqParam(*apvts.getParameter("Cut Freq"))
    { }

    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

    bool needRepaint = true;

private:
    juce::RangedAudioParameter& cutFreqParam;

    int shakeCount = 0;
    juce::Point<float> shakeOffset{ 0.f, 0.f };

    juce::Colour lastColor{ 44u, 44u, 44u };
};

struct CustomSlider : juce::Slider
{
public:
    CustomSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID, const juce::String& suffix)
        : juce::Slider(
            juce::Slider::SliderStyle::LinearHorizontal,
            juce::Slider::TextEntryBoxPosition::TextBoxAbove
        ),
        apvts(apvts),
        rap(*apvts.getParameter(parameterID)),
        suffix(suffix)
    { }

    ~CustomSlider()
    {
        setLookAndFeel(nullptr);
    }

    const int textHei = 14;
    void paint(juce::Graphics& g) override;
    //juce::String getDisplayString() const;

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;
    juce::String suffix;
};

struct CutFreqSlider : CustomSlider
{
public:
    CutFreqSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID, const juce::String& suffix)
        : CustomSlider(apvts, parameterID, suffix), lnf(apvts)
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
    ResonanceSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID, const juce::String& suffix)
        : CustomSlider(apvts, parameterID, suffix), lnf(apvts)
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
};


//==============================================================================
struct DeltaBoxLookAndFeel : juce::LookAndFeel_V4
{
public:
    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};

struct CustomToggleButton : juce::ToggleButton
{
public:
    CustomToggleButton(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID, const juce::String& buttonText)
        : juce::ToggleButton(buttonText),
          apvts(apvts),
          rap(*apvts.getParameter(parameterID))
    {
        setLookAndFeel(&lnf);
    }

    ~CustomToggleButton()
    {
        setLookAndFeel(nullptr);
    }

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;
    DeltaBoxLookAndFeel lnf;
};
