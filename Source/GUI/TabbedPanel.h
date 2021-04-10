//
// Created by Alex Samsonov on 31/03/21.
//

#pragma once

#include "JuceHeader.h"


class TabbedPanel : public juce::TabbedComponent {
public:
    //==============================================================================
    TabbedPanel();
    ~TabbedPanel() override = default;

    //==============================================================================
    void resized() override;

    //==============================================================================
    std::function<void(bool splitVertically)> onSplitMenuItemClicked;
    std::function<void(const TabbedPanel &panel)> onCloseMenuItemClicked;
    std::function<void(bool maximizedState)> onMaximizedStateChanged;

private:
    //==============================================================================
    juce::ArrowButton topRightMenuButton = {"PanelMenu", 0.25, juce::Colours::white};
    juce::DrawableButton maximizeButton = {"PanelMaximize", juce::DrawableButton::ButtonStyle::ImageStretched};
    std::unique_ptr<juce::Drawable> maximizeButtonIcon;

    bool maximizedState = false;
    void onMaximizeButtonStageChanged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabbedPanel)
};



