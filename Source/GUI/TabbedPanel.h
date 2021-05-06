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
    std::unique_ptr<juce::XmlElement> createXml() const;
    void initializeFromXml(const juce::XmlElement &xml);

    std::function<void(bool splitVertically)> onSplitMenuItemClicked;
    std::function<void(const TabbedPanel &panel)> onCloseMenuItemClicked;
    std::function<void(bool maximizedState)> onMaximizedStateChanged;

private:
    //==============================================================================
    juce::ArrowButton topRightMenuButton = {"PanelMenu", 0.25, juce::Colours::white};
    juce::DrawableButton maximizeButton = {"PanelMaximize", juce::DrawableButton::ButtonStyle::ImageStretched};
    juce::DrawableButton addTabButton = {"AddTab", juce::DrawableButton::ButtonStyle::ImageStretched};
    std::unique_ptr<juce::Drawable> maximizeButtonIcon;
    std::unique_ptr<juce::Drawable> addTabButtonIcon;
    std::unique_ptr<juce::Drawable> closeTabButtonIcon;

    bool maximizedState = false;
    void onTopRightMenuButtonClicked();
    void onMaximizeButtonStageChanged();
    void onAddTabButtonClicked();
    void onCloseTabButtonClicked(juce::TabBarButton* tabBarButton);
    void addPanelTab(const juce::String& tabName);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabbedPanel)
};



