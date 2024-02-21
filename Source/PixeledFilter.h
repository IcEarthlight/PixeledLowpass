/*
  ==============================================================================

    PixeledFilter.h
    Created: 20 Feb 2024 10:36:07pm
    Author:  earth

  ==============================================================================
*/

#pragma once

#include <juceHeader.h>

struct SampleData
{
    float l;
    float r;

    SampleData() : l(0.f), r(0.f) { }
    SampleData(float l, float r) : l(l), r(r) { }

    //inline SampleData operator+ (SampleData& sd) const
    //{
    //    return { l + sd.l, r + sd.r };
    //}

    //inline SampleData operator+= (SampleData& sd)
    //{
    //    l += sd.l;
    //    r += sd.r;
    //    return *this;
    //}

    //inline SampleData operator- (SampleData& sd) const
    //{
    //    return { l - sd.l, r - sd.r };
    //}

    //inline SampleData operator-= (SampleData& sd)
    //{
    //    l -= sd.l;
    //    r -= sd.r;
    //    return *this;
    //}

    //inline SampleData operator* (float x) const
    //{
    //    return { l * x, r * x };
    //}

    //inline SampleData operator*= (float x)
    //{
    //    l *= x;
    //    r *= x;
    //    return *this;
    //}

    //inline SampleData operator* (SampleData& sd) const
    //{
    //    return { l * sd.l, r * sd.r };
    //}

    //inline SampleData operator*= (SampleData& sd)
    //{
    //    l *= sd.l;
    //    r *= sd.r;
    //    return *this;
    //}

    //inline SampleData operator/ (float x) const
    //{
    //    return { l / x, r / x };
    //}

    //inline SampleData operator/= (float x)
    //{
    //    l /= x;
    //    r /= x;
    //    return *this;
    //}

    //inline bool operator== (SampleData& sd) const
    //{
    //    return l == sd.l && r == sd.r;
    //}
};

class PixeledFilter
{
public:
    bool delta;

    inline void setFreq(float freq, double srate);
    void prepare(float freq, double srate);
    void process(juce::AudioBuffer<float>& buffer);

private:
    float cutFreq;
    float convAmt;
    float prevAmt;

    std::vector<SampleData> buf;
    int nextPos;

    inline void processFrame(float& lspl, float& rspl, float amt);
};
