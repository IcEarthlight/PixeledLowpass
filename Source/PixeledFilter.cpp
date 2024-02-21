/*
  ==============================================================================

    PixeledFilter.cpp
    Created: 20 Feb 2024 10:36:07pm
    Author:  earth

  ==============================================================================
*/

#include "PixeledFilter.h"

inline void PixeledFilter::setFreq(float freq, double srate)
{
    cutFreq = freq;
    convAmt = cutFreq <= 12000 ?
        srate / cutFreq :
        1.f + (srate / 1.2e4f - 1.f) * pow((3.e4f - cutFreq) / 1.8e4f, 2);
}

void PixeledFilter::prepare(float freq, double srate)
{
    setFreq(freq, srate);
    prevAmt = convAmt;

    buf = std::vector<SampleData>((int)(srate / 20.), SampleData());

    nextPos = 0;
}

void PixeledFilter::process(juce::AudioBuffer<float>& buffer)
{
    int samplesPerBlock = buffer.getNumSamples();
    float* lptr = buffer.getWritePointer(0);
    float* rptr = buffer.getWritePointer(1);

    for (int sample = 0; sample < samplesPerBlock; sample++)
    {
        processFrame(
            lptr[sample],
            rptr[sample],
            prevAmt + (convAmt - prevAmt) * sample / samplesPerBlock
        );
    }

    prevAmt = convAmt;
}

void PixeledFilter::processFrame(float& lspl, float& rspl, float amt)
{
    SampleData sd = SampleData(lspl, rspl);
    SampleData sum = sd;
    float restAmt = amt - 1.f;

    if (amt > 1)
    {
        int index = nextPos;
        while (restAmt > 1.f)
        {
            if (index == 0)
                index = buf.size();
            index--;

            const SampleData& bufData = buf[index];
            sum.l += bufData.l;
            sum.r += bufData.r;
            restAmt -= 1.f;
        }

        if (index == 0)
            index = buf.size();
        index--;

        const SampleData& bufData = buf[index];
        sum.l += bufData.l * restAmt;
        sum.r += bufData.r * restAmt;

        sum.l /= amt;
        sum.r /= amt;
    }

    if (nextPos == buf.size())
        nextPos = 0;

    buf[nextPos] = sd;
    nextPos++;

    if (delta)
    {
        lspl = sd.l - sum.l;
        rspl = sd.r - sum.r;
    }
    else
    {
        lspl = sum.l;
        rspl = sum.r;
    }
}
