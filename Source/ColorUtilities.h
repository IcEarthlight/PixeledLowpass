/*
  ==============================================================================

    ColorUtilities.h
    Created: 8 Mar 2024 10:48:57am
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

inline float differenceBetween(juce::Colour& c0, juce::Colour& c1)
{
    return sqrt(
        pow(c0.getFloatRed() - c1.getFloatRed(), 2.f)
        + pow(c0.getFloatGreen() - c1.getFloatGreen(), 2.f)
        + pow(c0.getFloatBlue() - c1.getFloatBlue(), 2.f)
        + pow(c0.getFloatAlpha() - c1.getFloatAlpha(), 2.f)
    );
}

inline bool isSimilar(juce::Colour& c0, juce::Colour& c1, const int threshold = 2)
{
    return (
        abs(c0.getRed() - c1.getRed())
        + abs(c0.getGreen() - c1.getGreen())
        + abs(c0.getBlue() - c1.getBlue())
        + abs(c0.getAlpha() - c1.getAlpha())
        ) <= threshold;
}

inline juce::Colour colorLerp(juce::Colour& c0, juce::Colour& c1, const float k)
{
    //if (k <= 0.f)
    //    return c0;

    //if (k >= 1.f)
    //    return c1;

    return juce::Colour(
        juce::uint8(c0.getRed()   + k * (c1.getRed()   - c0.getRed())),
        juce::uint8(c0.getGreen() + k * (c1.getGreen() - c0.getGreen())),
        juce::uint8(c0.getBlue()  + k * (c1.getBlue()  - c0.getBlue())),
        juce::uint8(c0.getAlpha() + k * (c1.getAlpha() - c0.getAlpha()))
    );
}

inline juce::Colour colorApproach(juce::Colour& c0, juce::Colour& c1)
{
    return juce::Colour(
        c0.getRed()   == c1.getRed()   ? c0.getRed()   : juce::uint8(c0.getRed()   < c1.getRed()   ? c0.getRed()+1   : c0.getRed()-1),
        c0.getGreen() == c1.getGreen() ? c0.getGreen() : juce::uint8(c0.getGreen() < c1.getGreen() ? c0.getGreen()+1 : c0.getGreen()-1),
        c0.getBlue()  == c1.getBlue()  ? c0.getBlue()  : juce::uint8(c0.getBlue()  < c1.getBlue()  ? c0.getBlue()+1  : c0.getBlue()-1),
        c0.getAlpha() == c1.getAlpha() ? c0.getAlpha() : juce::uint8(c0.getAlpha() < c1.getAlpha() ? c0.getAlpha()+1 : c0.getAlpha()-1)
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

        if (isSimilar(lastColor, mixedColor))
            lastColor = colorApproach(lastColor, newColor);
        else
            lastColor = mixedColor;
    }
}
