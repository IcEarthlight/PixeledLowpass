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
      cutFreqSlider(p.apvts, "Cut Freq", "Hz"),
      resonanceSlider(p.apvts, "Resonance", "dB"),
      deltaBox(p.apvts, "Delta", "Delta"),
      cutFreqAtch(p.apvts, "Cut Freq", cutFreqSlider),
      resonanceAtch(p.apvts, "Resonance", resonanceSlider),
      deltaAtch(p.apvts, "Delta", deltaBox)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    titleImg = juce::Drawable::createFromSVGFile(
        juce::File("C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Source\\title.svg")
    );
    titleImg->replaceColour(juce::Colours::black, titleColor);

    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    addChildComponent(*titleImg);
    addChildComponent(deltaBox);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);
    titleImg->setVisible(true);
    deltaBox.setVisible(true);

    setSize(defaultWid, defaultHei);
    setResizable(true, false);
    setResizeLimits(defaultWid, defaultHei, INT_MAX, INT_MAX);
    getConstrainer()->setFixedAspectRatio((double)defaultWid / defaultHei);
}

PixeledLowpassAudioProcessorEditor::~PixeledLowpassAudioProcessorEditor()
{
}

//==============================================================================
void PixeledLowpassAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(backColor);

    //g.setColour (juce::Colours::White);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    if (cutFreqSlider.needRepaint())
        cutFreqSlider.repaint();

    if (resonanceSlider.needRepaint() || cutFreqSlider.needRepaint())
        resonanceSlider.repaint();

    g.setColour(juce::Colours::grey);
    g.fillRect(getLocalBounds().getWidth() * 3 / 4, getLocalBounds().getHeight() * 2 / 3, getLocalBounds().getWidth() / 4, getLocalBounds().getHeight() / 3);

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

    juce::Rectangle<float> titleAera(
        localWid * 23.f / 476.f, localHei / 10.f,
        localWid * 323.f / 476.f, localHei * 21.f / 160.f
    );
    titleImg->setTransformToFit(
        titleAera,
        juce::RectanglePlacement::stretchToFit
    );

    bottomArea.removeFromLeft(localWid * 3 / 4);
    deltaBox.setBounds(bottomArea);
}
