/*
  ==============================================================================

    CustomComponents.cpp
    Created: 6 Mar 2024 9:45:45pm
    Author:  earth

  ==============================================================================
*/

#include "CustomSliders.h"
#include "ColorTable.h"
#include "ColorUtilities.h"


void CustomSlider::paint(juce::Graphics& g)
{
    juce::Range<double> range = getRange();
    juce::Rectangle<int> sliderBounds = getLocalBounds();

    getLookAndFeel().drawLinearSlider(
        g,
        sliderBounds.getX(), sliderBounds.getY(),
        sliderBounds.getWidth(), sliderBounds.getHeight(),
        rap.getValue(),
        0.f, 1.f,
        juce::Slider::SliderStyle::LinearHorizontal,
        *this
    );
}

juce::Slider::SliderLayout CustomLookAndFeel::getSliderLayout(juce::Slider& slider)
{
    // 1. compute the actually visible textBox size from the slider textBox size and some additional constraints

    int minXSpace = 0;
    int minYSpace = 0;

    juce::Slider::TextEntryBoxPosition textBoxPos = slider.getTextBoxPosition();

    if (textBoxPos == juce::Slider::TextBoxLeft || textBoxPos == juce::Slider::TextBoxRight)
        minXSpace = 30;
    else
        minYSpace = 15;

    juce::Rectangle<int> localBounds = slider.getLocalBounds();

    int textBoxWidth = 0.4 * localBounds.getWidth();
    int textBoxHeight = 0.3 * localBounds.getHeight();

    juce::Slider::SliderLayout layout;

    // 2. set the textBox bounds

    if (textBoxPos != juce::Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth(textBoxWidth);
            layout.textBoxBounds.setHeight(textBoxHeight);

            if (textBoxPos == juce::Slider::TextBoxLeft)           layout.textBoxBounds.setX(0);
            else if (textBoxPos == juce::Slider::TextBoxRight)     layout.textBoxBounds.setX(localBounds.getWidth() - textBoxWidth);
            else /* above or below -> centre horizontally */ layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);

            if (textBoxPos == juce::Slider::TextBoxAbove)          layout.textBoxBounds.setY(0.25 * localBounds.getHeight());
            else if (textBoxPos == juce::Slider::TextBoxBelow)     layout.textBoxBounds.setY(localBounds.getHeight() - textBoxHeight);
            else /* left or right -> centre vertically */    layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }

    // 3. set the slider bounds

    layout.sliderBounds = localBounds;

    if (slider.isBar())
    {
        layout.sliderBounds.reduce(1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == juce::Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft(textBoxWidth);
        else if (textBoxPos == juce::Slider::TextBoxRight) layout.sliderBounds.removeFromRight(textBoxWidth);
        else if (textBoxPos == juce::Slider::TextBoxAbove) layout.sliderBounds.removeFromTop(textBoxHeight);
        else if (textBoxPos == juce::Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom(textBoxHeight);

        const int thumbIndent = getSliderThumbRadius(slider);

        if (slider.isHorizontal())    layout.sliderBounds.reduce(thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce(0, thumbIndent);
    }

    return layout;
}

void CustomLookAndFeel::drawLabelwithText(juce::Graphics& g, juce::Label& label, juce::String text, juce::String suffix, int digitLen)
{
    juce::Colour newColor;
    if (deltaParam.getValue() > 0.5f)
        newColor = ColorTable::textb;
    else
        newColor = ColorTable::text;

    colorGradient(labelNeedRepaint, labelColor, newColor, 0.08f);

    if (mouseDownCounter > 0)
        text = label.getText().substring(0, digitLen) + suffix;

    if (!label.isBeingEdited())
    {
        const juce::Font font(getLabelFont(label));

        g.setColour(labelColor);
        g.setFont(font);
        g.setFont(label.getLocalBounds().getHeight());

        g.drawFittedText(text, label.getLocalBounds(), label.getJustificationType(),
            1, label.getMinimumHorizontalScale());
    }
}

void CutFreqLookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
    if (slider.isMouseButtonDown())
        mouseDownCounter = 20;

    x += 12;
    width -= 24;

    juce::Rectangle<float> bound(x, y + height / 1.5f, width, height / 9.f);
    juce::Rectangle<float> hilightBound = bound.removeFromLeft(sliderPos * width);

    float darkness = 1.f - pow(sliderPos, 8.f);
    if (deltaParam.getValue() > 0.5f)
        g.setColour(juce::Colour(0.5f, 0.f, 1.f - 0.2f * darkness, 1.f));
    else
        g.setColour(juce::Colour(0.5f, 0.1f, 0.15f * darkness, 1.f));
    g.fillRect(bound);

    float hue = 0.8333 * (6 * pow(sliderPos, 5) - 15 * pow(sliderPos, 4) + 10 * pow(sliderPos, 3));
    float saturation = 10 * pow(-pow(sliderPos, 2) + sliderPos, 3) + 0.17;
    float brightness = 0.4 * (1 - pow(4 * pow(sliderPos, 3) - 4 * pow(sliderPos, 1.5) + 1, 3)) + 0.43;
    if (deltaParam.getValue() > 0.5f)
        brightness = 1.f - 0.5 * brightness;

    juce::Colour newColor(hue, saturation, brightness, 1.f);

    colorGradient(sliderNeedRepaint, lastColor, newColor, 0.08f);

    g.setColour(lastColor);
    g.fillRect(hilightBound);
}

void ResonanceLookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
    if (slider.isMouseButtonDown())
        mouseDownCounter = 20;

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
    float mappedCutFreq = pow(cutFreqParam.getValue(), 1.5f);
    float k = 1 - pow(1.f + 400.f / 121.f * (mappedCutFreq + 0.1f) * (mappedCutFreq - 1.f), 5);

    juce::Rectangle<float> bound(x, y + height / 1.5f, width, height / 9.f);
    if (sliderPos > 0.4f)
    {
        float shakeAmp = pow(sliderPos, 4) - 0.0256;
        shakeAmp *= pow(k, 3);
        bound += shakeAmp * (height / 9.f) * shakeOffset;
    }
    juce::Rectangle<float> hilightBound = bound.removeFromLeft(sliderPos * width);

    float darkness = 1. - pow(sliderPos, 8);
    if (deltaParam.getValue() > 0.5f)
        g.setColour(juce::Colour(0.5f, 0.f, 1.f - 0.2f * darkness, 1.f));
    else
        g.setColour(juce::Colour(0.5f, 0.1f, 0.15f * darkness, 1.f));
    g.fillRect(bound);

    float hue = 0.61667f - sliderPos * 0.575f;
    float saturation = 0.17 + 0.83 * pow(k * sliderPos, 3);
    float brightness = 0.4f + (0.83f * k + 0.17f) * sliderPos * 0.6f;
    if (deltaParam.getValue() > 0.5f)
        brightness = 1.f - 0.5 * brightness;

    juce::Colour newColor(hue, saturation, brightness, 1.f);

    colorGradient(sliderNeedRepaint, lastColor, newColor, 0.08f);

    g.setColour(lastColor);
    g.fillRect(hilightBound);
}
