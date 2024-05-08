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
      cutFreqSlider(p.apvts, "Cut Freq"),
      resonanceSlider(p.apvts, "Resonance"),
      deltaBox(*this, p.apvts, "Delta"),
      doubleFilter(*this, p.apvts, "DoubleFilter"),
      cutFreqAtch(p.apvts, "Cut Freq", cutFreqSlider),
      resonanceAtch(p.apvts, "Resonance", resonanceSlider),
      deltaAtch(p.apvts, "Delta", deltaBox),
      doubleAtch(p.apvts, "DoubleFilter", doubleFilter)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    bool delta = deltaBox.getState();
    renderer.addAsset(
        *this,
        BinaryData::title_svg,
        BinaryData::title_svgSize,
        juce::Rectangle<float>(
            23.f  / 476, 16.f / 160,
            323.f / 476, 21.f / 160
        ),
        ColorTable::arr[(0 << 1) + delta],
        0.08f
    );
    renderer.addAsset(
        *this,
        BinaryData::title_hp_svg,
        BinaryData::title_hp_svgSize,
        juce::Rectangle<float>(
            23.f  / 476, 16.f / 160,
            291.f / 476, 21.f / 160
        ),
        ColorTable::arr[(1 << 1) + delta],
        0.08f
    );
    renderer.addAsset(
        *this,
        BinaryData::delta_svg,
        BinaryData::delta_svgSize,
        juce::Rectangle<float>(
            29.f / 36, 641.f / 800,
            1431.f / 16660, 53.f / 800
        ),
        ColorTable::arr[(2 << 1) + delta],
        0.08f
    );
    renderer.addAsset(
        *this,
        BinaryData::double_filter_svg,
        BinaryData::double_filter_svgSize,
        juce::Rectangle<float>(
            421.f / 476, 23.f / 160,
            18.f  / 476, 12.f / 160
        ),
        ColorTable::arr[(3 << 1) + delta],
        0.04f
    );


    addChildComponent(cutFreqSlider);
    addChildComponent(resonanceSlider);
    addChildComponent(deltaBox);
    addChildComponent(doubleFilter);
    cutFreqSlider.setVisible(true);
    resonanceSlider.setVisible(true);
    deltaBox.setVisible(true);
    doubleFilter.setVisible(true);

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

    if (deltaBox.getState() != lastDeltaState)
    {
        deltaSwitch();
        lastDeltaState = deltaBox.getState();
    }

    if (doubleFilter.getState() != lastDoubleState)
    {
        doubleSwitch();
        lastDoubleState = doubleFilter.getState();
    }

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
    juce::Rectangle<int> doubleFilterArea(
        localWid * 412 / 476, localHei * 17 / 160,
        localWid *  36 / 476, localHei * 24 / 160
    );

    cutFreqSlider.setBounds(lSliderArea);
    resonanceSlider.setBounds(rSliderArea);
    deltaBox.setBounds(deltaBoxArea);
    doubleFilter.setBounds(doubleFilterArea);

    renderer.resized(localWid, localHei);
}

void PixeledLowpassAudioProcessorEditor::deltaSwitch()
{
    bool delta = deltaBox.getState();
    bool isdouble = doubleFilter.getState();
    for (int i = 0; i < renderer.size(); i++)
        renderer.setColorAt(i, ColorTable::arr[(i << 1) + delta]);

    if (isdouble)
        renderer.setColorAt(3, delta ? ColorTable::filterx2b_on : ColorTable::filterx2_on);

    repaint();
}

void PixeledLowpassAudioProcessorEditor::doubleSwitch()
{
    bool delta = deltaBox.getState();
    bool isdouble = doubleFilter.getState();
    if (isdouble)
        renderer.setColorAt(3, delta ? ColorTable::filterx2b_on : ColorTable::filterx2_on);
    else
        renderer.setColorAt(3, delta ? ColorTable::filterx2b_off : ColorTable::filterx2_off);

    repaint();
}
