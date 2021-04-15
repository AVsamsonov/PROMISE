//
// Created by Alex Samsonov on 14/04/21.
//

#include "PreferencesDialog.h"


//==============================================================================
PreferencesDialog::PreferencesDialog(juce::AudioDeviceManager &manager)
    : selector(manager, 0, 256, 0, 256, true, true, true, false)
{
    addAndMakeVisible(std::addressof(selector));
    setSize(500, 450);
}

void PreferencesDialog::resized()
{
    selector.setBounds(getLocalBounds());
}