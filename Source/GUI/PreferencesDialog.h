//
// Created by Alex Samsonov on 14/04/21.
//

#pragma once

#include "JuceHeader.h"


class PreferencesDialog : public juce::Component
{
public:
    //==============================================================================
    explicit PreferencesDialog(juce::AudioDeviceManager &manager);
    ~PreferencesDialog() override = default;

private:
    //==============================================================================
    juce::AudioDeviceSelectorComponent selector;

    //==============================================================================
    void resized() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PreferencesDialog)
};



