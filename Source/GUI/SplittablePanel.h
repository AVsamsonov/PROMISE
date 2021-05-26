//
// Created by Alex Samsonov on 25/03/21.
//

#pragma once

#include <JuceHeader.h>
#include "BasePanel.h"
#include "TabbedPanel.h"

class SplittablePanel : public juce::Component, public BasePanel {
public:
    //==============================================================================
    SplittablePanel();
    ~SplittablePanel() override = default;

    //==============================================================================
    void resized() override;

    //==============================================================================
    std::unique_ptr<juce::XmlElement> createXml() const override;
    void initializeFromXml(const juce::XmlElement &xml) override;
    void initialize();
    void reset();

private:
    //==============================================================================
    friend class TabbedPanel;

    OwnedArray<BasePanel> children;

    bool verticalSplit = false;
    std::unique_ptr<juce::StretchableLayoutResizerBar> resizer;
    juce::StretchableLayoutManager layout;

    bool maximized = false;

    void addChild(BasePanel *panel, int index=-1);
    int removeChild(BasePanel *panel, bool deletePanel=true);

    void splitChild(BasePanel *panel, bool vertically=false);
    void closeChild(BasePanel *panel);
    void moveChildFrom(SplittablePanel *other);
    void setChildMaximized(BasePanel *panel, bool maximized);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplittablePanel)
};


