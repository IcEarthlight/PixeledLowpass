/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PixeledLowpassAudioProcessor::PixeledLowpassAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout()),
       pxlFilter(),
       pxlFilter2()
#endif
{
}

PixeledLowpassAudioProcessor::~PixeledLowpassAudioProcessor()
{
}

//==============================================================================
const juce::String PixeledLowpassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PixeledLowpassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PixeledLowpassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PixeledLowpassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PixeledLowpassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PixeledLowpassAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PixeledLowpassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PixeledLowpassAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PixeledLowpassAudioProcessor::getProgramName (int index)
{
    return {};
}

void PixeledLowpassAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PixeledLowpassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    initRsnFilter(sampleRate, samplesPerBlock);
    initPxlFilter(sampleRate, apvts.getRawParameterValue("Cut Freq")->load());

    FilterParams filterParams = getFilterParams(apvts, sampleRate);
    if (filterParams.peakGain > 1.f)
    {
        updateRsnFilter(filterParams);
    }
}

void PixeledLowpassAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PixeledLowpassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PixeledLowpassAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; i++)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    if (apvts.getParameterAsValue("Noise").getValue())
        for (int channel = 0; channel < totalNumOutputChannels; channel++)
        {
            float* ptr = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                ptr[sample] = float(std::rand() & 0x7fff) / 0x7fff - 0.5f;
            }
        }

    FilterParams filterParams = getFilterParams(apvts, getSampleRate());
    if (filterParams.peakGain > 1.f)
    {
        updateRsnFilter(filterParams);
        applyRsnFilter(buffer);
    }

    updatePxlFilter(apvts.getRawParameterValue("Cut Freq")->load());
    applyPxlFilter(buffer);
}

//==============================================================================
bool PixeledLowpassAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PixeledLowpassAudioProcessor::createEditor()
{
    return new PixeledLowpassAudioProcessorEditor(*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PixeledLowpassAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void PixeledLowpassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);

        FilterParams filterParams = getFilterParams(apvts, getSampleRate());
        if (filterParams.peakGain > 1.f)
            updateRsnFilter(filterParams);

        updatePxlFilter(apvts.getRawParameterValue("Cut Freq")->load());
    }
}

FilterParams getFilterParams(juce::AudioProcessorValueTreeState& apvts, double srate)
{
    FilterParams fp;
    fp.cutFreq = 0.4f * apvts.getRawParameterValue("Cut Freq")->load();
    fp.peakGain = apvts.getRawParameterValue("Resonance")->load();
    fp.quality = 0.714f + fp.peakGain / 8.f;

    //if (fp.cutFreq > 12000.f)
    //    fp.cutFreq = 3.888e12f * srate / (3.888e12f + (srate - 1.2e4f) * pow(3e4 - fp.cutFreq, 2));
    if (fp.cutFreq > 8000.f)
        fp.cutFreq = 1.28e11 * srate / (1.28e11 + (srate - 8e3) * pow(1.2e4 - fp.cutFreq, 2));

    if (fp.cutFreq > srate / 6.)
    {
        if (fp.cutFreq < srate / 2.)
        {
            float k = (srate / 2. - fp.cutFreq) * 3. / srate;
            fp.peakGain *= k * k;
        }
        else
            fp.peakGain = 0.f;
    }

    fp.peakGain = juce::Decibels::decibelsToGain(fp.peakGain);

    return fp;
}

juce::AudioProcessorValueTreeState::ParameterLayout PixeledLowpassAudioProcessor::createParameterLayout() const
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Cut Freq",
        "Cut Freq",
        juce::NormalisableRange<float>(20.f, 30000.f, 0.001f, 0.3f),
        30000.f
    ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Resonance",
        "Resonance",
        juce::NormalisableRange<float>(0.f, 12.f, 0.0001f, 0.4f),
        0.f
    ));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "Delta",
        "Delta",
        false
    ));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "Noise",
        "Noise",
        false
    ));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "DoubleFilter",
        "DoubleFilter",
        false
    ));

    return layout;
}

void PixeledLowpassAudioProcessor::initRsnFilter(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;

    lRsnFilter.prepare(spec);
    rRsnFilter.prepare(spec);
}

void PixeledLowpassAudioProcessor::initPxlFilter(double sampleRate, float cutFreq)
{
    pxlFilter.prepare(cutFreq, sampleRate);
    pxlFilter2.prepare(cutFreq, sampleRate);
}

void PixeledLowpassAudioProcessor::updateRsnFilter(const FilterParams& filterParams)
{
    juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>>
        peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            getSampleRate(),
            filterParams.cutFreq,
            filterParams.quality,
            filterParams.peakGain
        );
    *lRsnFilter.coefficients = *peakCoefficients;
    *rRsnFilter.coefficients = *peakCoefficients;
}

void PixeledLowpassAudioProcessor::updatePxlFilter(float cutFreq)
{
    pxlFilter.delta = apvts.getParameterAsValue("Delta").getValue();
    pxlFilter.setFreq(cutFreq, getSampleRate());

    if (apvts.getParameterAsValue("DoubleFilter").getValue())
    {
        pxlFilter2.setFreq(cutFreq, getSampleRate());
        pxlFilter2.delta = pxlFilter.delta;
    }
    else
    {
        pxlFilter2.setBypassed();
        pxlFilter2.delta = false;
    }
}

void PixeledLowpassAudioProcessor::applyRsnFilter(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);

    juce::dsp::AudioBlock<float> lBlock = block.getSingleChannelBlock(0);
    juce::dsp::AudioBlock<float> rBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> lContext(lBlock);
    juce::dsp::ProcessContextReplacing<float> rContext(rBlock);

    lRsnFilter.process(lContext);
    rRsnFilter.process(rContext);
}

void PixeledLowpassAudioProcessor::applyPxlFilter(juce::AudioBuffer<float>& buffer)
{
    pxlFilter.process(buffer);
    pxlFilter2.process(buffer);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PixeledLowpassAudioProcessor();
}
