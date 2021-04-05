//
// Created by Alex Samsonov on 25/03/21.
//

#pragma once

#include <JuceHeader.h>
#include "TabbedPanel.h"


class SplittablePanel : public juce::Component {
public:
    //==============================================================================
    SplittablePanel();
    ~SplittablePanel() override;

    //==============================================================================
    void resized() override;

    //==============================================================================
    void split(bool vertically=false);
    juce::Component* getPanelA() { return panelA.get(); }
    juce::Component* getPanelB() { return panelB.get(); }

    std::function<void(const SplittablePanel &panel, bool maximizedState)> onMaximizedStateChanged;
private:
    //==============================================================================
    bool verticalSplit = false;
    std::unique_ptr<juce::Component> panelA;
    std::unique_ptr<juce::Component> panelB;
    std::unique_ptr<juce::StretchableLayoutResizerBar> resizer;
    juce::StretchableLayoutManager layout;

    void onTabbedPanelSplitMenuItemClicked(bool splitVertically);
    void onTabbedPanelCloseMenuItemClicked(const TabbedPanel &panel);
    void onTabbedPanelMaximizedStateChanged(bool state) const;
    void onSplittablePanelMaximizedStateChanged(const SplittablePanel &panel, bool state);

    void setupNestedPanel(std::unique_ptr<juce::Component> &panel);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplittablePanel)
};


