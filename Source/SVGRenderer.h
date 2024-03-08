/*
  ==============================================================================

    SVGRenderer.h
    Created: 8 Mar 2024 1:02:52am
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SVGAsset
{
public:
    SVGAsset(
        juce::Component& parentComponent,
        const juce::String& path,
        juce::Rectangle<float> area,
        juce::Colour& color,
        float gradient = 0.f
    );

    void resized(float localWei, float localHei);
    void setColor(juce::Colour& newColor);
    void updateColor();

private:
    juce::Component& parentComponent;
    std::unique_ptr<juce::Drawable> img;
    juce::Rectangle<float> area;
    float gradient;
    juce::Colour color;
    juce::Colour targetColor;
    bool needUpdate;
};

class SVGRenderer
{
public:
    ~SVGRenderer();
    void addAsset(
        juce::Component& parentComponent,
        const juce::String& path,
        juce::Rectangle<float> area,
        juce::Colour color,
        float gradient = 0.f
    );
    void resized(float localWei, float localHei);
    void updateAll();
    void clear();

private:
    std::vector<SVGAsset*> assets;
};
