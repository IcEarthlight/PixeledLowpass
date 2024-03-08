/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ColorTable.h"

//==============================================================================
PixeledLowpassAudioProcessorEditor::PixeledLowpassAudioProcessorEditor(PixeledLowpassAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      cutFreqSlider(p.apvts, "Cut Freq", "Hz"),
      resonanceSlider(p.apvts, "Resonance", "dB"),
      deltaBox(*this, p.apvts, "Delta"),
      cutFreqAtch(p.apvts, "Cut Freq", cutFreqSlider),
      resonanceAtch(p.apvts, "Resonance", resonanceSlider),
      deltaAtch(p.apvts, "Delta", deltaBox)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    //titleImg = juce::Drawable::createFromSVGFile(
    //    juce::File()
    //);
    //titleImg->replaceColour(juce::Colours::black, ColorTable::title);
    renderer.addAsset(
        *this,
        "C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Assets\\title.svg",
        juce::Rectangle<float>(
            23.f  / 476, 16.f / 160,
            323.f / 476, 21.f / 160
        ),
        ColorTable::title,
        0.08f
    );
    renderer.addAsset(
        *this,
        "C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Assets\\delta.svg",
        juce::Rectangle<float>(
            29.f / 36, 641.f / 800,
            1431.f / 16660, 53.f / 800
        ),
        ColorTable::text,
        0.08f
    );

    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    //addChildComponent(*titleImg);
    addChildComponent(deltaBox);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);
    //titleImg->setVisible(true);
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

    g.fillAll(deltaBox.getState() ? ColorTable::back : ColorTable::back_);

    if (cutFreqSlider.needRepaint())
        cutFreqSlider.repaint();

    if (resonanceSlider.needRepaint() || cutFreqSlider.needRepaint())
        resonanceSlider.repaint();

    renderer.updateAll();

}

void PixeledLowpassAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    juce::Rectangle<int> bounds = getLocalBounds();
    int localWid = bounds.getWidth();
    int localHei = bounds.getHeight();
    
    //juce::Rectangle<int> titleArea = bounds.removeFromTop(bounds.getHeight() / 3);
    //juce::Rectangle<int> bottomArea = bounds.removeFromBottom(bounds.getHeight() / 2);

    //juce::Rectangle<int> lSliderArea = bounds.removeFromLeft(bounds.getWidth() / 2);
    //juce::Rectangle<int> rSliderArea = bounds;

    float interval = 0.08f;
    //lSliderArea.removeFromLeft(interval * localWid);
    //lSliderArea.removeFromRight(interval * localWid / 2.f);
    //rSliderArea.removeFromLeft(interval * localWid / 2.f);
    //rSliderArea.removeFromRight(interval * localWid);

    juce::Rectangle<int> lSliderArea(
        interval * localWid, localHei / 3,
        (1 - 3 * interval)/2 * localWid, localHei / 3
    );
    juce::Rectangle<int> rSliderArea(
        (0.5f + interval/2) * localWid, localHei / 3,
        (1 - 3 * interval) / 2 * localWid, localHei / 3
    );

    cutFreqSlider.setBounds(lSliderArea);
    resonanceSlider.setBounds(rSliderArea);

    //juce::Rectangle<float> titleAera(
    //    localWid *  23 / 476, localHei / 10,
    //    localWid * 323 / 476, localHei * 21 / 160
    //);
    //titleImg->setTransformToFit(
    //    titleAera,
    //    juce::RectanglePlacement::stretchToFit
    //);
    renderer.resized(localWid, localHei);

    juce::Rectangle<int> deltaBoxArea(
        localWid * 339 / 476, localHei * 119 / 160,
        localWid * 107 / 476, localHei *  29 / 160
    );
    deltaBox.setBounds(deltaBoxArea);
}
