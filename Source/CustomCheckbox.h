/*
  ==============================================================================

    CustomCheckbox.h
    Created: 8 Mar 2024 5:49:49pm
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PixeledLowpassAudioProcessorEditor;

struct DeltaBoxLookAndFeel : juce::LookAndFeel_V4
{
public:
    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;

    void drawTickBox(juce::Graphics& g,
        juce::Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        [[maybe_unused]] const bool isEnabled,
        [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
        [[maybe_unused]] const bool shouldDrawButtonAsDown) override;
};

struct DoubleFilterLookAndFeel : juce::LookAndFeel_V4
{
public:
    void drawToggleButton(juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override { }
};

struct DeltaBoxButton : juce::ToggleButton
{
public:
    DeltaBoxButton(
        PixeledLowpassAudioProcessorEditor& editor,
        juce::AudioProcessorValueTreeState& apvts,
        juce::StringRef parameterID
    )
        : editor(editor),
          juce::ToggleButton(),
          apvts(apvts),
          rap(*apvts.getParameter(parameterID))
    {
        setLookAndFeel(&lnf);
    }

    ~DeltaBoxButton()
    {
        setLookAndFeel(nullptr);
    }

    inline bool getState() const noexcept
    {
        return rap.getValue() > 0.5f;
    }

    //void clicked() override;

private:
    PixeledLowpassAudioProcessorEditor& editor;
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;

    DeltaBoxLookAndFeel lnf;
};

struct DoubleFilterButton : juce::ToggleButton
{
public:
    DoubleFilterButton(
        PixeledLowpassAudioProcessorEditor& editor,
        juce::AudioProcessorValueTreeState& apvts,
        juce::StringRef parameterID
    )
        : editor(editor),
        juce::ToggleButton(),
        apvts(apvts),
        rap(*apvts.getParameter(parameterID))
    {
        setLookAndFeel(&lnf);
    }

    ~DoubleFilterButton()
    {
        setLookAndFeel(nullptr);
    }

    inline bool getState() const noexcept
    {
        return rap.getValue() > 0.5f;
    }

    //void clicked() override;

private:
    PixeledLowpassAudioProcessorEditor& editor;
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;

    DoubleFilterLookAndFeel lnf;
};
