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
    juce::Colour lastColor{ 44u, 44u, 44u };
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
    juce::Point<float> shakeOffset{ 0.f, 0.f };
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
    CustomToggleButton(juce::RangedAudioParameter& rap, const juce::String& buttonText)
        : juce::ToggleButton(buttonText),
          params(rap)
    {
        setLookAndFeel(&lnf);
    }

    ~CustomToggleButton()
    {
        setLookAndFeel(nullptr);
    }

private:
    juce::RangedAudioParameter& params;
    DeltaBoxLookAndFeel lnf;
};
