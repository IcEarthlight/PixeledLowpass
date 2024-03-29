/*
  ==============================================================================

    SVGRenderer.cpp
    Created: 8 Mar 2024 1:02:52am
    Author:  earth

  ==============================================================================
*/

#include "SVGRenderer.h"
#include "ColorUtilities.h"

SVGAsset::SVGAsset(
    juce::Component& parentComponent,
    const char* svg_src,
    const int svg_srcSize,
    juce::Rectangle<float> area,
    juce::Colour& color,
    float gradient
)   : parentComponent(parentComponent),
      area(area),
      color(color),
      targetColor(color),
      gradient(gradient),
      needUpdate(false)
{
    img = juce::Drawable::createFromImageData(svg_src, svg_srcSize);
    img->replaceColour(juce::Colours::black, targetColor);
    parentComponent.addChildComponent(*img);
    img->setVisible(true);
}

SVGAsset::~SVGAsset()
{
    parentComponent.removeChildComponent(img.get());
    img.reset();
    img = nullptr;
}

void SVGAsset::resized(float localWei, float localHei)
{
    juce::Rectangle<float> newAera(
        localWei * area.getX(), localHei * area.getY(),
        localWei * area.getWidth(), localHei * area.getHeight()
    );
    img->setTransformToFit(
        newAera,
        juce::RectanglePlacement::stretchToFit
    );
}

void SVGAsset::setColor(const juce::Colour& newColor)
{
    targetColor = newColor;
    needUpdate = true;
}

void SVGAsset::updateColor()
{
    if (!needUpdate)
        return;

    if (gradient <= 0.f)
    {
        img->replaceColour(color, targetColor);
        color = targetColor;
        needUpdate = false;
    }
    else
    {
        juce::Colour lastColor = color;
        colorGradient(needUpdate, color, targetColor, 0.08f);
        img->replaceColour(lastColor, color);
    }

    img->repaint();
}

SVGRenderer::~SVGRenderer()
{
    clear();
}

void SVGRenderer::addAsset(
    juce::Component& parentComponent,
    const char* svg_src,
    const int svg_srcSize,
    juce::Rectangle<float> area,
    juce::Colour color,
    float gradient
)
{
    assets.emplace_back(new SVGAsset(
        parentComponent, svg_src, svg_srcSize, area, color, gradient
    ));
}

void SVGRenderer::resized(float localWei, float localHei)
{
    for (SVGAsset* aptr : assets)
        aptr->resized(localWei, localHei);
}

void SVGRenderer::updateAll()
{
    for (SVGAsset* aptr : assets)
        aptr->updateColor();
}

void SVGRenderer::clear()
{
    for (std::vector<SVGAsset*>::iterator it = assets.begin(); it != assets.end(); it++)
    {
        if (*it != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
        }
    }
    assets.clear();
    assets.shrink_to_fit();
}
