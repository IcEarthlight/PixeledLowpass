/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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

    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);

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
    g.fillAll(juce::Colour::fromRGB(33, 33, 33));

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
}
