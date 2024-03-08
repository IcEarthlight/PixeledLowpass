/*
  ==============================================================================

    CustomCheckbox.cpp
    Created: 8 Mar 2024 5:49:49pm
    Author:  earth

  ==============================================================================
*/

#include "PluginEditor.h"
#include "CustomCheckbox.h"
#include "ColorTable.h"
//#include "ColorUtilities.h"

void DeltaBoxLookAndFeel::drawTickBox(juce::Graphics& g,
    juce::Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    [[maybe_unused]] const bool isEnabled,
    [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
    [[maybe_unused]] const bool shouldDrawButtonAsDown)
{
    juce::Rectangle<float> tickBounds(x, y, w, h);

    g.setColour(ColorTable::text);

    if (ticked)
    {
        g.fillRoundedRectangle(x + w/5, y + h/5, w * 3/5, h * 3/5, w/4);
        g.drawRoundedRectangle(x + w/12, y + h/12, w * 5/6, h * 5/6, w/3, w/8);
    }
    else
        g.drawRoundedRectangle(x, y, w, h, w/3, w/8);
}

void DeltaBoxLookAndFeel::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    float tickWidth = (float)button.getHeight() * 212 / 725;

    drawTickBox(
        g, button,
        (float)button.getWidth() * 2/9, ((float)button.getHeight() - tickWidth) / 2,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown
    );
}

void CustomToggleButton::clicked()
{
    editor.repaint();
}
