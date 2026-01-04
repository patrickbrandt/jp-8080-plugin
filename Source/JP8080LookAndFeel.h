#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Custom LookAndFeel for JP-8080 Controller
 *
 * Provides custom rendering for rotary knobs with a hardware-inspired design
 */
class JP8080LookAndFeel : public juce::LookAndFeel_V4
{
public:
    JP8080LookAndFeel()
    {
        // Set custom color scheme
        setColour(juce::Slider::thumbColourId, juce::Colour(0xffff6600));  // Orange indicator
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff6600));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff404040));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff404040));
        setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPosProportional, float rotaryStartAngle,
                         float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        // Draw outer rim
        g.setColour(juce::Colour(0xff303030));
        g.fillEllipse(bounds);

        // Draw inner knob body
        auto innerBounds = bounds.reduced(3.0f);
        juce::ColourGradient gradient(juce::Colour(0xff505050), innerBounds.getCentreX(), innerBounds.getY(),
                                     juce::Colour(0xff2a2a2a), innerBounds.getCentreX(), innerBounds.getBottom(),
                                     false);
        g.setGradientFill(gradient);
        g.fillEllipse(innerBounds);

        // Draw track (background arc)
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                   arcRadius, arcRadius,
                                   0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colour(0xff1a1a1a));
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Draw value arc
        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                  arcRadius, arcRadius,
                                  0.0f, rotaryStartAngle, toAngle, true);
            g.setColour(findColour(juce::Slider::rotarySliderFillColourId));
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Draw pointer indicator
        juce::Path pointer;
        auto pointerLength = radius * 0.6f;
        auto pointerThickness = 3.0f;
        pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        g.setColour(slider.isEnabled() ? findColour(juce::Slider::thumbColourId) : juce::Colour(0xff404040));
        g.fillPath(pointer, juce::AffineTransform::rotation(toAngle).translated(bounds.getCentreX(), bounds.getCentreY()));
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto* label = new juce::Label();
        label->setJustificationType(juce::Justification::centred);
        label->setColour(juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
        label->setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
        label->setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
        label->setFont(juce::Font(11.0f));
        return label;
    }
};
