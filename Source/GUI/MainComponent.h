#pragma once

#include <JuceHeader.h>
#include "SplittablePanel.h"

//==============================================================================
juce::ApplicationProperties& getAppProperties();

//==============================================================================
class MainComponent  : public juce::Component, juce::MenuBarModel
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void resized() override;

    //==============================================================================
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
    //==============================================================================
    juce::AudioDeviceManager audioDeviceManager;
    std::unique_ptr<SplittablePanel> centralPanel;

    void showPreferencesDialog();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
