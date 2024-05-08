/*
  ==============================================================================

    ColorTable.h
    Created: 8 Mar 2024 12:14:20am
    Author:  earth

  ==============================================================================
*/

#pragma once

namespace ColorTable
{
    const juce::Colour back          { 0xFF'2C2C2Cu };
    const juce::Colour title         { 0xCC'FFFFFFu };
    const juce::Colour titlehp       { 0x00'2C2C2Cu };
    const juce::Colour text          { 0x99'FFFFFFu };
    const juce::Colour filterx2_on   { 0x88'FFFFFFu };
    const juce::Colour filterx2_off  { 0x22'FFFFFFu };

    const juce::Colour backb         { 0xFF'BBBBBBu };
    const juce::Colour titleb        { 0x00'BBBBBBu };
    const juce::Colour titlehpb      { 0x99'000000u };
    const juce::Colour textb         { 0x77'000000u };
    const juce::Colour filterx2b_on  { 0x88'000000u };
    const juce::Colour filterx2b_off { 0x22'000000u };

    const juce::Colour arr[] {
        title, titleb, titlehp, titlehpb, text, textb, filterx2_off, filterx2b_off
    };
}
