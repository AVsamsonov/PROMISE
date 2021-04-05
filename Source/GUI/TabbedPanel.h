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
    std::function<void(const TabbedPanel &panel, bool maximizedState)> onMaximizedStateChanged;

private:
    //==============================================================================
    std::unique_ptr<juce::ArrowButton> topRightMenuButton;
    std::unique_ptr<juce::DrawableButton> maximizeButton;
    std::unique_ptr<juce::Drawable> maximizeButtonIcon;

    bool maximizedState = false;
    void onMaximizeButtonStageChanged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabbedPanel)
};



