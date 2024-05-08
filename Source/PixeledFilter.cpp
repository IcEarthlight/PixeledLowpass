/*
  ==============================================================================

    PixeledFilter.cpp
    Created: 20 Feb 2024 10:36:07pm
    Author:  earth

  ==============================================================================
*/

#include "PixeledFilter.h"

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

    //logger.logMessage(logMsg);
    //logMsg.clear();
}

inline void PixeledFilter::processFrame(float& lspl, float& rspl, const float amt)
{
    SampleData sd = SampleData(lspl, rspl);
    SampleData sum = sd;

    if (amt > 1.f)
    {
        if (isnan(lastAmt) || 1 + ceil(lastAmt) - int(amt) >= ceil(amt))
            sum += intervalSum(nextPos + 1 - amt, nextPos);
        else
        {
            sum -= intervalSum(nextPos - lastAmt, nextPos + 1 - amt);
            sum += lastSum;

            //SampleData diff = sum - sd - intervalSum(nextPos + 1 - amt, nextPos);
            //logMsg.append(
            //      std::to_string(diff.l) + "\t"
            //    + std::to_string(diff.r) + "\t"
            //    + std::to_string(lastAmt) + "\t"
            //    + std::to_string(amt)
            //    + "\t" + std::to_string(nextPos)
            //    + "\n",
            //    INT_MAX
            //);
        }
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
    //logMsg.append("intervalSum(int   " + std::to_string(val0) + ", int   " + std::to_string(val1) + ");\n", INT_MAX);
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
    //logMsg.append("intervalSum(int   " + std::to_string(val0) + ", float " + std::to_string(val1) + ");\n", INT_MAX);
    if (val1 >= buf.size())
        return intervalSum(val0, (int)buf.size()) + intervalSum(0, val1 - buf.size());

    if (val0 == int(val1))
        return buf[val0] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

    SampleData sum = SampleData();

    for (int i = val0; i < int(val1); i++)
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
    //logMsg.append("intervalSum(float " + std::to_string(val0) + ", int   " + std::to_string(val1) + ");\n", INT_MAX);
    if (val0 < 0.f)
        return intervalSum(val0 + buf.size(), (int)buf.size()) + intervalSum(0, val1);

    if (int(val0) == val1 - 1)
        return buf[int(val0)] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

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
    //logMsg.append("intervalSum(float " + std::to_string(val0) + ", float " + std::to_string(val1) + ");\n", INT_MAX);
    if (val0 < 0.f && val1 < 0.f)
    {
        val0 += buf.size();
        val1 += buf.size();
    }

    if (val0 < 0.f)
        return intervalSum(val0 + buf.size(), (int)buf.size()) + intervalSum(0, val1);

    if (val1 >= buf.size())
        return intervalSum(val0, (int)buf.size()) + intervalSum(0, val1 - buf.size());

    if (int(val0) == int(val1))
        return buf[int(val0)] * (val1 - val0);

    if (val0 > val1)
        return -intervalSum(val1, val0);

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
