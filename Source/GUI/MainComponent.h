#pragma once

#include <JuceHeader.h>
#include "SplittablePanel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override = default;

    //==============================================================================
    void resized() override;

    //==============================================================================


private:
    //==============================================================================
    std::unique_ptr<SplittablePanel> centralPanel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
