/*
  ==============================================================================

    CustomComponents.h
    Created: 6 Mar 2024 9:45:45pm
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColorTable.h"


struct CustomLookAndFeel : juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(const juce::AudioProcessorValueTreeState& apvts)
        : juce::LookAndFeel_V4(),
          deltaParam(*apvts.getParameter("Delta"))
    { }

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
    void CustomLookAndFeel::drawLabelwithText(juce::Graphics& g,
        juce::Label& label, juce::String text,
        juce::String suffix, int digitLen);

    int mouseDownCounter = 0;
    bool sliderNeedRepaint = true;
    bool labelNeedRepaint = true;

    inline bool needRepaint() const noexcept
    {
        return sliderNeedRepaint || labelNeedRepaint;
    }

protected:
    juce::RangedAudioParameter& deltaParam;
    juce::Colour lastColor{ ColorTable::back };
    
private:
    juce::Colour labelColor{ ColorTable::text };
};

struct CutFreqLookAndFeel : CustomLookAndFeel
{
public:
    CutFreqLookAndFeel(const juce::AudioProcessorValueTreeState& apvts)
        : CustomLookAndFeel(apvts)
    { }

    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

    inline void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        drawLabelwithText(g, label, "Cut Freq", " Hz", 6);
    }
};

struct ResonanceLookAndFeel : CustomLookAndFeel
{
public:
    ResonanceLookAndFeel(const juce::AudioProcessorValueTreeState& apvts)
        : CustomLookAndFeel(apvts),
          cutFreqParam(*apvts.getParameter("Cut Freq"))
    { }

    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle sliderStyle,
        juce::Slider& slider) override;

private:
    juce::RangedAudioParameter& cutFreqParam;

    int shakeCount = 0;
    juce::Point<float> shakeOffset { 0.f, 0.f };

    inline void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        drawLabelwithText(g, label, "Resonance", " dB", 4);
    }
};

struct CustomSlider : juce::Slider
{
public:
    CustomSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID)
        : juce::Slider(
            juce::Slider::SliderStyle::LinearHorizontal,
            juce::Slider::TextEntryBoxPosition::TextBoxAbove
        ),
        apvts(apvts),
        rap(*apvts.getParameter(parameterID))
    { }

    ~CustomSlider()
    {
        setLookAndFeel(nullptr);
    }

    inline void update() noexcept
    {
        if (lnf->mouseDownCounter > 0)
        {
            repaint();
            lnf->mouseDownCounter--;
        }
    }

    const int textHei = 14;
    void paint(juce::Graphics& g) override;
    //juce::String getDisplayString() const;

protected:
    std::unique_ptr<CustomLookAndFeel> lnf;

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;
};

struct CutFreqSlider : CustomSlider
{
public:
    CutFreqSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID)
        : CustomSlider(apvts, parameterID)
    {
        lnf = std::make_unique<CutFreqLookAndFeel>(apvts);
        setLookAndFeel(lnf.get());
    }

    inline bool needRepaint() const noexcept
    {
        return lnf->needRepaint();
    }
};

struct ResonanceSlider : CustomSlider
{
public:
    ResonanceSlider(juce::AudioProcessorValueTreeState& apvts, juce::StringRef parameterID)
        : CustomSlider(apvts, parameterID)
    {
        lnf = std::make_unique<ResonanceLookAndFeel>(apvts);
        setLookAndFeel(lnf.get());
    }

    inline bool needRepaint() const noexcept
    {
        return lnf->needRepaint() || getValue() > 0.4f;
    }
};
