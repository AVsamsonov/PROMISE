#pragma once

#include <JuceHeader.h>
#include "SplittablePanel.h"

//==============================================================================
const juce::String getAppName();
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
    void setCurrentDesktop(const juce::String& desktopName);

private:
    //==============================================================================
    juce::AudioDeviceManager audioDeviceManager;
    juce::TextButton desktopSelector;
    juce::String currentDesktopName;
    SplittablePanel centralPanel;

    void showPreferencesDialog();
    static juce::StringArray getDesktopNamesList() ;
    static juce::String getDesktopsDir();

    void onDesktopSelectorClicked();
    void saveCurrentDesktop(const juce::String& desktopName);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
