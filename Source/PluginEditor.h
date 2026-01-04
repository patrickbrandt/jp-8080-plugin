#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * JP-8080 Controller Plugin Editor
 *
 * GUI for the JP-8080 Controller plugin.
 * This will contain the interface for controlling JP-8080 parameters.
 */
class JP8080ControllerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JP8080ControllerAudioProcessorEditor (JP8080ControllerAudioProcessor&);
    ~JP8080ControllerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JP8080ControllerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JP8080ControllerAudioProcessorEditor)
};
