//
// Created by Alex Samsonov on 27/03/21.
//

#pragma once

#include "JuceHeader.h"


class BasePanel : public juce::Component {
public:
    //==============================================================================
    BasePanel();
    ~BasePanel() override = default;

    //==============================================================================
    void resized() override;
    void paint(juce::Graphics& g) override;

    //==============================================================================
    std::function<void(const BasePanel &panel, bool splitVertically)> onSplitMenuItemClicked;
    std::function<void(const BasePanel &panel)> onCloseMenuItemClicked;

private:
    std::unique_ptr<juce::ArrowButton> topRightMenuButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasePanel)
};
