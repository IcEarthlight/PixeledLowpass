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

    inline SampleData operator+ (SampleData sd) const
    {
        return { l + sd.l, r + sd.r };
    }

    inline SampleData operator+= (SampleData sd)
    {
        l += sd.l;
        r += sd.r;
        return *this;
    }

    inline SampleData operator- () const
    {
        return { -l, -r };
    }

    inline SampleData operator- (SampleData sd) const
    {
        return { l - sd.l, r - sd.r };
    }

    inline SampleData operator-= (SampleData sd)
    {
        l -= sd.l;
        r -= sd.r;
        return *this;
    }

    inline SampleData operator* (float x) const
    {
        return { l * x, r * x };
    }

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

//struct SampleSum
//{
//    long l;
//    long r;
//
//    SampleSum() : l(0), r(0) { }
//    SampleSum(long l, long r) : l(l), r(r) { }
//    SampleSum(SampleData sd) : l(sd.l * 8388608l), r(sd.r * 8388608l) { }
//
//    inline SampleSum operator+ (SampleSum& sd) const
//    {
//        return { l + sd.l, r + sd.r };
//    }
//
//    inline SampleSum operator+= (SampleSum& ss)
//    {
//        l += ss.l;
//        r += ss.r;
//        return *this;
//    }
//
//    inline SampleSum operator+= (SampleData& sd)
//    {
//        l += sd.l * 8388608l;
//        r += sd.r * 8388608l;
//        return *this;
//    }
//
//    inline SampleSum operator- () const
//    {
//        return { -l, -r };
//    }
//
//    inline SampleSum operator-= (SampleSum& ss)
//    {
//        l -= ss.l;
//        r -= ss.r;
//        return *this;
//    }
//};

class PixeledFilter
{
public:
    bool delta;

    inline void setFreq(float freq, double srate)
    {
        convAmt = freq <= 12000.f ?
            srate / freq :
            1.f + (srate / 1.2e4f - 1.f) * pow((3.e4f - freq) / 1.8e4f, 2);
    }
    inline void setBypassed() { convAmt = 1.f; }

    void prepare(float freq, double srate);
    void process(juce::AudioBuffer<float>& buffer);

private:
    float convAmt;
    float prevAmt;

    std::vector<SampleData> buf;
    int nextPos;

    float lastAmt;
    SampleData lastSum;

    inline void processFrame(float& lspl, float& rspl, const float amt);
    SampleData intervalSum(int val0, int val1);
    SampleData intervalSum(int val0, float val1);
    SampleData intervalSum(float val0, int val1);
    SampleData intervalSum(float val0, float val1);
};
