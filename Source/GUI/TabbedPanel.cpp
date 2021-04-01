//
// Created by Alex Samsonov on 31/03/21.
//

#include "TabbedPanel.h"

//==============================================================================
TabbedPanel::TabbedPanel()
    : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    auto colour = findColour (juce::ResizableWindow::backgroundColourId);
    addTab("tab", colour, nullptr, true);

    // Add top right menu button
    topRightMenuButton.reset(new juce::ArrowButton("PanelMenu", 0.25, juce::Colours::white));
    addAndMakeVisible(topRightMenuButton.get());

    topRightMenuButton->onClick = [&] {
        juce::PopupMenu menu;
        menu.addItem("Split Panel Horizontally", [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(false);
        });
        menu.addItem("Split Panel Vertically", [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(true);
        });
        menu.addItem("Close Panel", [&] () {
            if (onCloseMenuItemClicked) onCloseMenuItemClicked(*this);
        });
        menu.showMenuAsync(juce::PopupMenu::Options{});
    };
}

//==============================================================================
void TabbedPanel::resized() {
    juce::TabbedComponent::resized();

    auto b = getLocalBounds();
    if (!b.isEmpty()) {
        topRightMenuButton->setBounds(b.removeFromTop(20)
                                              .removeFromRight(20)
                                              .reduced(4));
    }
}
