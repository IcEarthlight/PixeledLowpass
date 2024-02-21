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
    convAmt = cutFreq <= 12000.f ?
        srate / cutFreq :
        1.f + (srate / 1.2e4f - 1.f) * pow((3.e4f - cutFreq) / 1.8e4f, 2);
}

void PixeledFilter::prepare(float freq, double srate)
{
    setFreq(freq, srate);
    prevAmt = convAmt;

    buf = std::vector<SampleData>((int)(srate / 20.), SampleData());

    nextPos = 0;
    lastAmt = NAN;
    lastSum = SampleData(NAN, NAN);
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

inline void PixeledFilter::processFrame(float& lspl, float& rspl, float amt)
{
    SampleData sd = SampleData(lspl, rspl);
    SampleData sum = sd;

    if (amt > 1.f)
    {
        sum += intervalSum(nextPos - amt + 1.f, nextPos);
    }

    if (nextPos == buf.size())
        nextPos = 0;

    buf[nextPos] = sd;
    nextPos++;

    if (delta)
    {
        lspl = sd.l - sum.l / amt;
        rspl = sd.r - sum.r / amt;
    }
    else
    {
        lspl = sum.l / amt;
        rspl = sum.r / amt;
    }

    lastSum = sum;
    lastAmt = amt;
}

SampleData PixeledFilter::intervalSum(int val0, int val1)
{
    if (val0 == val1)
        return SampleData();

    if (val0 > val1)
        return -intervalSum(val1, val0);

    SampleData sum = SampleData();

    for (int i = val0; i < val1; i++)
    {
        SampleData& bufData = buf[i];
        sum.l += bufData.l;
        sum.r += bufData.r;
    }

    return sum;
}

SampleData PixeledFilter::intervalSum(int val0, float val1)
{
    if (val0 == int(val1))
        return buf[val0] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

    if (val1 > buf.size())
        return intervalSum(val0, (int)buf.size()) + intervalSum(0, val1 - buf.size());

    SampleData sum = SampleData();

    for (int i = int(val0) + 1; i < val1; i++)
    {
        SampleData& bufData = buf[i];
        sum.l += bufData.l;
        sum.r += bufData.r;
    }

    sum += buf[int(val1)] * (val1 - int(val1));

    return sum;
}

SampleData PixeledFilter::intervalSum(float val0, int val1)
{
    if (int(val0) == val1 - 1)
        return buf[int(val0)] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

    if (val0 < 0.f)
        return intervalSum(val0 + buf.size(), (int)buf.size()) + intervalSum(0, val1);

    SampleData sum = buf[int(val0)] * (int(val0) + 1 - val0);

    for (int i = int(val0) + 1; i < val1; i++)
    {
        SampleData& bufData = buf[i];
        sum.l += bufData.l;
        sum.r += bufData.r;
    }

    return sum;
}

SampleData PixeledFilter::intervalSum(float val0, float val1)
{
    if (int(val0) == int(val1))
        return buf[int(val0)] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

    if (val0 < 0.f)
        return intervalSum(val0 + buf.size(), (int)buf.size()) + intervalSum(0, val1);

    if (val1 > buf.size())
        return intervalSum(val0, (int)buf.size()) + intervalSum(0, val1 - buf.size());

    SampleData sum = buf[int(val0)] * (int(val0) + 1 - val0);

    for (int i = int(val0) + 1; i < int(val1); i++)
    {
        SampleData& bufData = buf[i];
        sum.l += bufData.l;
        sum.r += bufData.r;
    }

    sum += buf[int(val1)] * (val1 - int(val1));

    return sum;
}
