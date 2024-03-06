/*
  ==============================================================================

    CustomComponents.cpp
    Created: 6 Mar 2024 9:45:45pm
    Author:  earth

  ==============================================================================
*/

#include "CustomComponents.h"

inline float differenceBetween(juce::Colour& c0, juce::Colour& c1)
{
    return sqrt(
        pow(c0.getFloatRed() - c1.getFloatRed(), 2.f)
        + pow(c0.getFloatGreen() - c1.getFloatGreen(), 2.f)
        + pow(c0.getFloatBlue() - c1.getFloatBlue(), 2.f)
    );
}

inline juce::Colour colorLerp(juce::Colour& c0, juce::Colour& c1, const float k)
{
    //if (k <= 0.f)
    //    return c0;

    //if (k >= 1.f)
    //    return c1;

    return juce::Colour(
        unsigned int(c0.getRed() + k * (c1.getRed() - c0.getRed())),
        unsigned int(c0.getGreen() + k * (c1.getGreen() - c0.getGreen())),
        unsigned int(c0.getBlue() + k * (c1.getBlue() - c0.getBlue()))
    );
}

inline juce::Colour colorApproach(juce::Colour& c0, juce::Colour& c1)
{
    return juce::Colour(
        c0.getRed() == c1.getRed() ? c0.getRed() : c0.getRed() < c1.getRed() ? c0.getRed() + 1 : c0.getRed() - 1,
        c0.getGreen() == c1.getGreen() ? c0.getGreen() : c0.getGreen() < c1.getGreen() ? c0.getGreen() + 1 : c0.getGreen() - 1,
        c0.getBlue() == c1.getBlue() ? c0.getBlue() : c0.getBlue() < c1.getBlue() ? c0.getBlue() + 1 : c0.getBlue() - 1
    );
}

inline void colorGradient(bool& needRepaint, juce::Colour& lastColor, juce::Colour& newColor, const float k)
{
    if (lastColor == newColor)
        needRepaint = false;
    else
    {
        needRepaint = true;
        juce::Colour mixedColor = colorLerp(lastColor, newColor, 0.08f);

        if (lastColor == mixedColor)
            newColor = colorApproach(lastColor, newColor);
        else
            newColor = mixedColor;
    }
}

void CutFreqLookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
    x += 12;
    width -= 24;

    juce::Rectangle<float> bound(x, y + height / 1.5f, width, height / 9.f);
    juce::Rectangle<float> hilightBound = bound.removeFromLeft(sliderPos * width);

    float darkness = 1.f - pow(sliderPos, 8.f);
    g.setColour(juce::Colour(0.5f, 0.1f, 0.15f * darkness, 1.f));
    g.fillRect(bound);

    float hue = 0.8333f * (6 * pow(sliderPos, 5.f) - 15 * pow(sliderPos, 4.f) + 10 * pow(sliderPos, 3.f));
    float saturation = 10.f * pow(-pow(sliderPos, 2.f) + sliderPos, 3.f) + 0.17f;
    float brightness = 0.4f * (1.f - 1.f * pow(4.f * pow(sliderPos, 3.f) - 4.f * pow(sliderPos, 1.5f) + 1.f, 3.f)) + 0.43f;
    juce::Colour newColor(hue, saturation, brightness, 1.f);

    colorGradient(needRepaint, lastColor, newColor, 0.08f);

    g.setColour(newColor);
    g.fillRect(hilightBound);

    lastColor = newColor;
}

void ResonanceLookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
    x += 12;
    width -= 24;

    shakeCount++;
    if (shakeCount >= 2)
    {
        shakeCount = 0;
        shakeOffset.setXY(
            float(std::rand() & 0x7fff) / 0x7fff - 0.5f,
            float(std::rand() & 0x7fff) / 0x7fff - 0.5f
        );
    }
    float k = 1.f - pow(1.f + 400.f / 121.f * (pow(cutFreqSlider.getValue(), 1.5) + 0.1f) * (pow(cutFreqSlider.getValue(), 1.5) - 1.f), 5.f);

    juce::Rectangle<float> bound(x, y + height / 1.5f, width, height / 9.f);
    if (sliderPos > 0.4f)
    {
        float shakeAmp = pow(sliderPos, 4.f) - 0.0256f;
        shakeAmp *= pow(k, 3.f);
        bound += shakeAmp * (height / 9.f) * shakeOffset;
    }
    juce::Rectangle<float> hilightBound = bound.removeFromLeft(sliderPos * width);

    float darkness = 1.f - pow(sliderPos, 8.f);
    g.setColour(juce::Colour(0.5f, 0.1f, 0.15f * darkness, 1.f));
    g.fillRect(bound);

    float hue = 0.61667f - sliderPos * 0.575f;
    float saturation = 0.17f + 0.83f * pow(k * sliderPos, 3.f);
    float brightness = 0.4f + (0.83f * k + 0.17f) * sliderPos * 0.6f;
    juce::Colour newColor(hue, saturation, brightness, 1.f);

    colorGradient(needRepaint, lastColor, newColor, 0.08f);

    g.setColour(newColor);
    g.fillRect(hilightBound);

    lastColor = newColor;
}

void DeltaBoxLookAndFeel::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    float fontSize = (float)button.getHeight() * 0.28125f;
    float tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) * 0.5f,
        tickWidth, tickWidth,
        button.getToggleState(),
        button.isEnabled(),
        shouldDrawButtonAsHighlighted,
        shouldDrawButtonAsDown);

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawFittedText(button.getButtonText(),
        button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10)
        .withTrimmedRight(2),
        juce::Justification::centredLeft, 10);
}
