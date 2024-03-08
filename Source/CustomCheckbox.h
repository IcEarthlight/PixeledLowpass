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

struct CustomToggleButton : juce::ToggleButton
{
public:
    CustomToggleButton(
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

    ~CustomToggleButton()
    {
        setLookAndFeel(nullptr);
    }

    inline bool getState() const noexcept
    {
        return rap.getValue() > 0.5f;
    }

    void clicked() override;

private:
    PixeledLowpassAudioProcessorEditor& editor;
    juce::AudioProcessorValueTreeState& apvts;
    juce::RangedAudioParameter& rap;

    DeltaBoxLookAndFeel lnf;
};
