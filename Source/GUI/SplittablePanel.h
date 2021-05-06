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
    explicit SplittablePanel(TabbedPanel *srcTabbedPanel);

    //==============================================================================
    void resized() override;

    //==============================================================================
    std::unique_ptr<juce::XmlElement> createXml() const;
    void initializeFromXml(const juce::XmlElement &xml);

    std::function<void(const SplittablePanel &panel, bool maximizedState)> onMaximizedStateChanged;
private:
    //==============================================================================
    bool verticalSplit = false;
    std::unique_ptr<juce::Component> panelA;
    std::unique_ptr<juce::Component> panelB;
    std::unique_ptr<juce::StretchableLayoutResizerBar> resizer;
    juce::StretchableLayoutManager layout;

    juce::Component* getPanelA() const { return panelA.get(); }
    juce::Component* getPanelB() const { return panelB.get(); }
    void split(bool vertically=false);
    bool isSplit() const { return panelB and resizer; }
    void onTabbedPanelSplitMenuItemClicked(bool splitVertically);
    void onTabbedPanelCloseMenuItemClicked(const TabbedPanel &panel);
    void onTabbedPanelMaximizedStateChanged(bool state) const;
    void onSplittablePanelMaximizedStateChanged(const SplittablePanel &panel, bool state);

    void setupNestedPanel(std::unique_ptr<juce::Component> &panel, bool movePanelA = false);
    void setupTabbedPanel();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplittablePanel)
};


