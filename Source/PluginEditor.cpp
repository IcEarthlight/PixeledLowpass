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

    bool delta = deltaBox.getState();
    renderer.addAsset(
        *this,
        "C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Assets\\title.svg",
        juce::Rectangle<float>(
            23.f  / 476, 16.f / 160,
            323.f / 476, 21.f / 160
        ),
        ColorTable::arr[(0 << 1) + delta],
        0.08f
    );
    renderer.addAsset(
        *this,
        "C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Assets\\title_hp.svg",
        juce::Rectangle<float>(
            23.f / 476, 16.f / 160,
            291.f / 476, 21.f / 160
        ),
        ColorTable::arr[(1 << 1) + delta],
        0.08f
    );
    renderer.addAsset(
        *this,
        "C:\\Users\\earth\\Works\\Code\\JUCE_works\\Pixeled Lowpass\\Assets\\delta.svg",
        juce::Rectangle<float>(
            29.f / 36, 641.f / 800,
            1431.f / 16660, 53.f / 800
        ),
        ColorTable::arr[(2 << 1) + delta],
        0.08f
    );

    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    addChildComponent(deltaBox);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);
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

    g.fillAll(deltaBox.getState() ? ColorTable::backb : ColorTable::back);

    cutFreqSlider.update();
    resonanceSlider.update();

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
    const int localWid = bounds.getWidth();
    const int localHei = bounds.getHeight();

    float interval = 0.08f;
    juce::Rectangle<int> lSliderArea(
        interval * localWid, 0.34f * localHei,
        (1 - 3 * interval)/2 * localWid, localHei / 3
    );
    juce::Rectangle<int> rSliderArea(
        (0.5f + interval/2) * localWid, 0.34f * localHei,
        (1 - 3 * interval) / 2 * localWid, localHei / 3
    );
    juce::Rectangle<int> deltaBoxArea(
        localWid * 339 / 476, localHei * 119 / 160,
        localWid * 107 / 476, localHei *  29 / 160
    );

    cutFreqSlider.setBounds(lSliderArea);
    resonanceSlider.setBounds(rSliderArea);
    deltaBox.setBounds(deltaBoxArea);

    renderer.resized(localWid, localHei);
}

void PixeledLowpassAudioProcessorEditor::deltaSwitch()
{
    repaint();

    bool delta = !deltaBox.getState();
    for (int i = 0; i < renderer.size(); i++)
        renderer.setColorAt(i, ColorTable::arr[(i << 1) + delta]);
}
