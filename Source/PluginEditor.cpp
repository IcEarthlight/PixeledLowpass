/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void LookAndFeel::drawLinearSlider(
    juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
    x += 12;
    width -= 24;

    juce::Rectangle<float> bound(x, y + height / 1.5f, width, height / 9.f);
    juce::Rectangle<float> hilightBound = bound.removeFromLeft(sliderPos * width);

    g.setColour(juce::Colour(0.5f, 0.25f, 0.1f, 1.f));
    g.fillRect(bound);

    g.setColour(juce::Colour(0.5f, 0.17f, 1.f, 1.f));
    g.fillRect(hilightBound);
}

void MySlider::paint(juce::Graphics& g)
{
    juce::Range<double> range = getRange();
    juce::Rectangle<int> sliderBounds = getSliderBounds();

    getLookAndFeel().drawLinearSlider(
        g,
        sliderBounds.getX(), sliderBounds.getY(),
        sliderBounds.getWidth(), sliderBounds.getHeight(),
        param->getValue(),
        0.f, 1.f,
        juce::Slider::SliderStyle::LinearHorizontal,
        *this
    );
}

juce::Rectangle<int> MySlider::getSliderBounds() const
{
    return getLocalBounds();
}

//==============================================================================
PixeledLowpassAudioProcessorEditor::PixeledLowpassAudioProcessorEditor(PixeledLowpassAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      cutFreqSlider(*audioProcessor.apvts.getParameter("Cut Freq"), "Hz"),
      resonanceSlider(*audioProcessor.apvts.getParameter("Resonance"), "dB"),
      cutFreqAtch(audioProcessor.apvts, "Cut Freq", cutFreqSlider),
      resonanceAtch(audioProcessor.apvts, "Resonance", resonanceSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    titleImg = juce::Drawable::createFromSVGFile(
        juce::File("C:\\Users\\earth\\Documents\\Code\\JUCE_works\\Pixeled Lowpass\\Source\\title.svg")
    );
    titleImg->replaceColour(juce::Colours::black, juce::Colours::whitesmoke);

    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    addChildComponent(*titleImg);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);
    titleImg->setVisible(true);

    setSize(defaultWid, defaultHei);
    setResizable(true, false);
    setResizeLimits(defaultWid, defaultHei, INT_MAX, INT_MAX);
    getConstrainer()->setFixedAspectRatio((double)defaultWid / defaultHei);
}

PixeledLowpassAudioProcessorEditor::~PixeledLowpassAudioProcessorEditor()
{
}

//==============================================================================
void PixeledLowpassAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::fromRGB(33u, 33u, 33u));

    //g.setColour (juce::Colours::White);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

}

void PixeledLowpassAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    juce::Rectangle<int> bounds = getLocalBounds();
    int localWid = bounds.getWidth();
    int localHei = bounds.getHeight();
    
    juce::Rectangle<int> titleArea = bounds.removeFromTop(bounds.getHeight() / 3);
    juce::Rectangle<int> bottomArea = bounds.removeFromBottom(bounds.getHeight() / 2);

    juce::Rectangle<int> lSliderArea = bounds.removeFromLeft(bounds.getWidth() / 2);
    juce::Rectangle<int> rSliderArea = bounds;

    float interval = 0.08f;
    lSliderArea.removeFromLeft(interval * localWid);
    lSliderArea.removeFromRight(interval * localWid / 2.f);
    rSliderArea.removeFromLeft(interval * localWid / 2.f);
    rSliderArea.removeFromRight(interval * localWid);

    cutFreqSlider.setBounds(lSliderArea);
    resonanceSlider.setBounds(rSliderArea);
    titleImg->setTransformToFit(
        juce::Rectangle<float>(
            localWid *  23.f / 476.f, localHei / 10.f,
            localWid * 323.f / 476.f, localHei * 21.f / 160.f
        ),
        juce::RectanglePlacement::stretchToFit
    );
}
